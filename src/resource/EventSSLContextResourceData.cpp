/*
 * File:   EventSSLContextResourceData.cpp
 * Author: ricky
 *
 * Created on 2014年5月19日, 上午 9:48
 */

#include "EventSSLContextResourceData.h"

namespace HPHP {

    IMPLEMENT_OBJECT_ALLOCATION(EventSSLContextResourceData)

    EventSSLContextResourceData::EventSSLContextResourceData(SSL_CTX *ssl_ctx):InternalResourceData((void*) ssl_ctx) {
        allow_self_signed = false;
        verify_depth = 0;
    }

    EventSSLContextResourceData::~EventSSLContextResourceData() {
    }

}