/*
 * File:   EventHttpResource.cpp
 * Author: ricky
 *
 * Created on 2014年5月19日, 上午 9:48
 */

#include "EventHttpResource.h"

namespace HPHP {

    IMPLEMENT_OBJECT_ALLOCATION(EventHttpResource)

    EventHttpResource::EventHttpResource(evhttp_t *event):InternalResource((void*) event) {
    }

    void EventHttpResource::setCallback(ObjectData *cb)
    {
        this->cb = cb;
    }

    void EventHttpResource::setCallbackArg(const Variant &arg) {
        this->arg = new Variant(arg);
    }

    const Variant &EventHttpResource::getCallbackArg() {
        return *arg;
    }

    void EventHttpResource::setDefaultCallbackArg(const Variant &arg) {
        this->defaultArg = new Variant(arg);
    }

    const Variant &EventHttpResource::getDefaultCallbackArg() {
        return *defaultArg;
    }

    void EventHttpResource::setDefaultCallback(ObjectData *cb) {
        defaultCb = cb;
    }

    ObjectData* EventHttpResource::getDefaultCallback() {
        return defaultCb;
    }

    ObjectData* EventHttpResource::getCallback()
    {
        return cb;
    }

    EventHttpResource::~EventHttpResource() {
    }

}