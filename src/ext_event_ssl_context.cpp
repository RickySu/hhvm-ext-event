#include "ext.h"

#ifdef HAVE_LIBEVENT_SSL_SUPPORT
namespace HPHP {

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

        Resource resource = Resource(NEWOBJ(InternalResource(ssl_ctx)));
        SET_RESOURCE(this_, resource, s_event_ssl_context);

        SSL_CTX_set_options(ssl_ctx, ssl_option);
    }

    void eventExtension::_initEventSslContextClass() {
        HHVM_ME(EventSslContext, __construct);
    }
}
#endif