/*
 * File:   EventBaseResource.cpp
 * Author: ricky
 *
 * Created on 2014年5月19日, 上午 9:48
 */

#include "EventBaseResource.h"

namespace HPHP {

    EventBaseResource::EventBaseResource(event_base_t *event_base):InternalResource((void*) event_base) {
    }

    EventBaseResource::~EventBaseResource() {
    }

}