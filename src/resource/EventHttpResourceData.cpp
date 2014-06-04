/*
 * File:   EventHttpResourceData.cpp
 * Author: ricky
 *
 * Created on 2014年5月19日, 上午 9:48
 */

#include "EventHttpResourceData.h"

namespace HPHP {

    IMPLEMENT_OBJECT_ALLOCATION(EventHttpResourceData)

    EventHttpResourceData::EventHttpResourceData(evhttp_t *event):InternalResourceData((void*) event) {
    }

    void EventHttpResourceData::setCallback(ObjectData *cb)
    {
        this->cb = cb;
    }

    void EventHttpResourceData::setCallbackArg(const Variant &arg) {
        this->arg = new Variant(arg);
    }

    const Variant &EventHttpResourceData::getCallbackArg() {
        return *arg;
    }

    void EventHttpResourceData::setDefaultCallbackArg(const Variant &arg) {
        this->defaultArg = new Variant(arg);
    }

    const Variant &EventHttpResourceData::getDefaultCallbackArg() {
        return *defaultArg;
    }

    void EventHttpResourceData::setDefaultCallback(ObjectData *cb) {
        defaultCb = cb;
    }

    ObjectData* EventHttpResourceData::getDefaultCallback() {
        return defaultCb;
    }

    ObjectData* EventHttpResourceData::getCallback()
    {
        return cb;
    }

    EventHttpResourceData::~EventHttpResourceData() {
    }

}