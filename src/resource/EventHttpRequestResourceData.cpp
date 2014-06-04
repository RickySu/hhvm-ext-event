/*
 * File:   EventHttpRequestResourceData.cpp
 * Author: ricky
 *
 * Created on 2014年5月19日, 上午 9:48
 */

#include "EventHttpRequestResourceData.h"

namespace HPHP {

    IMPLEMENT_OBJECT_ALLOCATION(EventHttpRequestResourceData)

    EventHttpRequestResourceData::EventHttpRequestResourceData():InternalResourceData() {
    }

    void EventHttpRequestResourceData::setCallback(ObjectData *cb)
    {
        this->cb = cb;
    }

    void EventHttpRequestResourceData::setCallbackArg(const Variant &arg) {
        this->arg = new Variant(arg);
    }

    const Variant &EventHttpRequestResourceData::getCallbackArg() {
        return *arg;
    }

    ObjectData* EventHttpRequestResourceData::getCallback()
    {
        return cb;
    }

    EventHttpRequestResourceData::~EventHttpRequestResourceData() {
    }

}