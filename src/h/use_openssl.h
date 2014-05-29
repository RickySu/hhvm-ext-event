/*
 * File:   use_openssl.h
 * Author: ricky
 *
 * Created on 2014年5月28日, 下午 5:35
 */

#ifndef USE_OPENSSL_H
#define	USE_OPENSSL_H

#ifdef OPENSSL_FOUND
    #include <openssl/rand.h>
    #include <openssl/err.h>
#endif

#endif	/* USE_OPENSSL_H */

