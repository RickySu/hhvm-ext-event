/*
 * File:   use_openssl.h
 * Author: ricky
 *
 * Created on 2014年5月28日, 下午 5:35
 */

#ifndef USE_OPENSSL_H
#define	USE_OPENSSL_H

#ifdef OPENSSL_FOUND
    #include <openssl/ssl.h>
    #include <openssl/rand.h>
    #include <openssl/err.h>
#endif

#define EVENT_SSLv2_CLIENT_METHOD  1
#define EVENT_SSLv3_CLIENT_METHOD  2
#define EVENT_SSLv23_CLIENT_METHOD 3
#define EVENT_TLS_CLIENT_METHOD    4
#define EVENT_SSLv2_SERVER_METHOD  5
#define EVENT_SSLv3_SERVER_METHOD  6
#define EVENT_SSLv23_SERVER_METHOD 7
#define EVENT_TLS_SERVER_METHOD    8

#define EVENT_OPT_LOCAL_CERT        1
#define EVENT_OPT_LOCAL_PK          2
#define EVENT_OPT_PASSPHRASE        3
#define EVENT_OPT_CA_FILE           4
#define EVENT_OPT_CA_PATH           5
#define EVENT_OPT_ALLOW_SELF_SIGNED 6
#define EVENT_OPT_VERIFY_PEER       7
#define EVENT_OPT_VERIFY_DEPTH      8
#define EVENT_OPT_CIPHERS           9


#endif	/* USE_OPENSSL_H */

