/*
 * File:   EventSSLContextResourceData.h
 * Author: ricky
 *
 * Created on 2014年5月19日, 上午 9:48
 */

#ifndef EventSSLContextResourceData_H
#define	EventSSLContextResourceData_H

#include "InternalResourceData.h"
#include "../h/use_openssl.h"

namespace HPHP {

    class EventSSLContextResourceData : public InternalResourceData {
    public:
        DECLARE_RESOURCE_ALLOCATION(EventSSLContextResourceData)
        CLASSNAME_IS("EventSSLContextResourceData")
        EventSSLContextResourceData(SSL_CTX *ssl_ctx);
        virtual ~EventSSLContextResourceData();

        bool allow_self_signed;
        int verify_depth;
    };
}

#endif	/* EventSSLContextResourceData_H */

