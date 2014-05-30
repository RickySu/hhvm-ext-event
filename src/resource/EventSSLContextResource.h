/*
 * File:   EventSSLContextResource.h
 * Author: ricky
 *
 * Created on 2014年5月19日, 上午 9:48
 */

#ifndef EVENTSSLCONTEXTRESOURCE_H
#define	EVENTSSLCONTEXTRESOURCE_H

#include "InternalResource.h"
#include "../h/use_openssl.h"

namespace HPHP {

    class EventSSLContextResource : public InternalResource {
    public:
        DECLARE_RESOURCE_ALLOCATION(EventSSLContextResource)
        CLASSNAME_IS("EventSSLContextResource")
        EventSSLContextResource(SSL_CTX *ssl_ctx);
        virtual ~EventSSLContextResource();

        bool allow_self_signed;
        int verify_depth;
    };
}

#endif	/* EVENTSSLCONTEXTRESOURCE_H */

