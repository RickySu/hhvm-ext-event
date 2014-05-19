/*
 * File:   EventConfigResource.cpp
 * Author: ricky
 *
 * Created on 2014年5月19日, 下午 3:15
 */

#include "EventConfigResource.h"

namespace HPHP {

    EventConfigResource::EventConfigResource(event_config_t *event_config):InternalResource((void *)event_config) {
    }

    EventConfigResource::~EventConfigResource() {
    }

}