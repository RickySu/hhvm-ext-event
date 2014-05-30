/*
 * File:   EventSSLContextResource.cpp
 * Author: ricky
 *
 * Created on 2014年5月19日, 上午 9:48
 */

#include "EventSSLContextResource.h"

namespace HPHP {

    IMPLEMENT_OBJECT_ALLOCATION(EventSSLContextResource)

    EventSSLContextResource::EventSSLContextResource(SSL_CTX *ssl_ctx):InternalResource((void*) ssl_ctx) {
        allow_self_signed = false;
        verify_depth = 0;
    }

    EventSSLContextResource::~EventSSLContextResource() {
    }

}