#include "ext.h"

#ifdef HAVE_LIBEVENT_SSL_SUPPORT
int event_ssl_data_index;

namespace HPHP {

    static int verify_callback(int preverify_ok, X509_STORE_CTX *ctx) {
        int ret = preverify_ok, err, depth;
        SSL *ssl;
        ssl  = (SSL *) X509_STORE_CTX_get_ex_data(ctx, SSL_get_ex_data_X509_STORE_CTX_idx());
        EventSSLContextResourceData *ssl_context_resource_data = (EventSSLContextResourceData *) SSL_get_ex_data(ssl, event_ssl_data_index);
        X509_STORE_CTX_get_current_cert(ctx);
        err      = X509_STORE_CTX_get_error(ctx);
        depth    = X509_STORE_CTX_get_error_depth(ctx);

        if(err == X509_V_ERR_DEPTH_ZERO_SELF_SIGNED_CERT && ssl_context_resource_data->allow_self_signed){
            return -1;
        }

        if(depth > ssl_context_resource_data->verify_depth){
            X509_STORE_CTX_set_error(ctx, X509_V_ERR_CERT_CHAIN_TOO_LONG);
            return 0;
        }
        return ret;
    }

    static int passwd_callback(char *buf, int num, int rwflag, void *data){
        const String *pass = (const String *) data;
        if(pass->size() <= num){
            memcpy((void *) buf, (void *) pass->c_str(), pass->size());
            return pass->size();
        }
        return 0;
    }

    ALWAYS_INLINE void set_ca(SSL_CTX *ctx, String &cafile, String &capath){
         if (!SSL_CTX_load_verify_locations(ctx, cafile.isNull()?NULL:cafile.c_str(), capath.isNull()?NULL:capath.c_str())) {
             raise_warning("Unable to set verify ca locations");
             return;
         }
    }

    ALWAYS_INLINE void set_ciphers(SSL_CTX *ctx, String &ciphers){
        if (SSL_CTX_set_cipher_list(ctx, ciphers.c_str()) != 1) {
            raise_warning("Failed setting cipher list");
        }
    }

    ALWAYS_INLINE bool ssl_ctx_set_private_key(SSL_CTX *ctx, const String &privatekey){
        char resolved_path[PATH_MAX];

        if(privatekey.isNull()){
           return false;
        }

        if(SSL_CTX_use_PrivateKey_file(ctx, resolved_path, SSL_FILETYPE_PEM) != 1){
            raise_warning("Unable to set private key file");
            return false;
        }

        return true;
    }

    ALWAYS_INLINE bool ssl_ctx_set_local_cert(SSL_CTX *ctx, const String &certfile, const String &privatekey){
        char resolved_path[PATH_MAX];

        if(realpath(certfile.c_str(), resolved_path)) {

            if (SSL_CTX_use_certificate_chain_file(ctx, resolved_path) != 1){
               raise_warning("SSL_CTX_use_certificate_chain_file failed");
               return false;
            }

            if(privatekey.isNull()){
               if(SSL_CTX_use_PrivateKey_file(ctx, resolved_path, SSL_FILETYPE_PEM) != 1){
                   raise_warning("Unable to set private key file");
                   return false;
               }
            }
            else{
               return ssl_ctx_set_private_key(ctx, privatekey);
            }
            return true;
        }

        return false;
    }

    ALWAYS_INLINE SSL_METHOD *get_ssl_method(int64_t method){
        switch(method){
            case EVENT_SSLv2_CLIENT_METHOD:
#ifdef OPENSSL_NO_SSL2
                raise_warning("SSLv2 support is not compiled into the OpenSSL library");
                return NULL;
#else
                return (SSL_METHOD *) SSLv2_client_method();
#endif
            case EVENT_SSLv3_CLIENT_METHOD:
                return (SSL_METHOD *) SSLv3_client_method();
            case EVENT_SSLv23_CLIENT_METHOD:
                return (SSL_METHOD *) SSLv23_client_method();
            case EVENT_TLS_CLIENT_METHOD:
                return (SSL_METHOD *) TLSv1_client_method();
            case EVENT_SSLv2_SERVER_METHOD:
#ifdef OPENSSL_NO_SSL2
                raise_warning("SSLv2 support is not compiled into the OpenSSL library");
                return NULL;
#else
                return (SSL_METHOD *) SSLv2_server_method();
#endif
            case EVENT_SSLv3_SERVER_METHOD:
                return (SSL_METHOD *) SSLv3_server_method();
            case EVENT_SSLv23_SERVER_METHOD:
                return (SSL_METHOD *) SSLv23_server_method();
            case EVENT_TLS_SERVER_METHOD:
                return (SSL_METHOD *) TLSv1_server_method();
            default:
                raise_warning("Unknow method");
        }
        return NULL;
    }

    ALWAYS_INLINE void set_ssl_ctx_options(SSL_CTX *ssl_ctx, const Array &options, EventSSLContextResourceData *ssl_context_resource_data){
        String cafile, capath, ciphers = "DEFAULT";
        for (ArrayIter iter(options); iter; ++iter) {
            Variant key(iter.first());
            int64_t idx = key.toInt64Val();
            switch(idx){
                case EVENT_OPT_LOCAL_CERT: {
                    if(options.exists(EVENT_OPT_LOCAL_PK)){
                        ssl_ctx_set_local_cert(ssl_ctx, options.rvalAt(key).toString(), options.rvalAt(EVENT_OPT_LOCAL_PK).toString());
                    }
                    else{
                        ssl_ctx_set_local_cert(ssl_ctx, options.rvalAt(key).toString(), String());
                    }
                    break;
                }
                case EVENT_OPT_ALLOW_SELF_SIGNED:
                    ssl_context_resource_data->allow_self_signed = options.rvalAt(key).toBooleanVal();
                    break;
                case EVENT_OPT_LOCAL_PK:
                    /* Skip. SSL_CTX_use_PrivateKey_file is applied in "local_cert". */
                    break;
                case EVENT_OPT_PASSPHRASE: {
                    String pass = options.rvalAt(key).toStrRef();
                    SSL_CTX_set_default_passwd_cb_userdata(ssl_ctx, &pass);
                    SSL_CTX_set_default_passwd_cb(ssl_ctx, passwd_callback);
                    break;
                }
                case EVENT_OPT_CA_FILE:
                    cafile = options.rvalAt(key).toCStrRef();
                    break;
                case EVENT_OPT_CA_PATH:
                    capath = options.rvalAt(key).toCStrRef();
                    break;
                case EVENT_OPT_VERIFY_PEER:
                    if(options.rvalAt(key).toBooleanVal()){
                        SSL_CTX_set_verify(ssl_ctx, SSL_VERIFY_PEER, verify_callback);
                    }
                    else{
                        SSL_CTX_set_verify(ssl_ctx, SSL_VERIFY_NONE, NULL);
                    }
                    break;
                case EVENT_OPT_VERIFY_DEPTH:
                    SSL_CTX_set_verify_depth(ssl_ctx, options.rvalAt(key).toInt64Val());
                    ssl_context_resource_data->verify_depth = options.rvalAt(key).toInt64Val();
                    break;
                case EVENT_OPT_CIPHERS:
                    ciphers = options.rvalAt(key).toStrRef();
                    break;
            }
            set_ciphers(ssl_ctx, ciphers);
            if(!cafile.isNull() || !capath.isNull()){
                set_ca(ssl_ctx, cafile, capath);
            }
        }
    }

    static void HHVM_METHOD(EventSslContext, __construct, int64_t method, const Array &options) {
        SSL_METHOD *ssl_method;
        SSL_CTX *ssl_ctx;
        long ssl_option = SSL_OP_ALL;
        ssl_method = get_ssl_method(method);

        if (ssl_method == NULL) {
            raise_error("Invalid method");
            return;
        }

        ssl_ctx = SSL_CTX_new(ssl_method);
        if (ssl_ctx == NULL) {
            raise_error("Creation of a new SSL_CTX object failed");
            return;
        }

        Resource resource = Resource(NEWOBJ(EventSSLContextResourceData(ssl_ctx)));
        SET_RESOURCE(this_, resource, s_event_ssl_context);
        EventSSLContextResourceData *ssl_context_resource_data = FETCH_RESOURCE(this_, EventSSLContextResourceData, s_event_ssl_context);

        SSL_CTX_set_options(ssl_ctx, ssl_option);
        set_ssl_ctx_options(ssl_ctx, options, ssl_context_resource_data);
    }

    void eventExtension::_initEventSslContextClass() {
        event_ssl_data_index = SSL_get_ex_new_index(0, (void *) "HHVM event extension EventSslContext index", NULL, NULL, NULL);
        HHVM_ME(EventSslContext, __construct);
    }
}
#endif