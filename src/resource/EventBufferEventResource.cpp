/*
 * File:   EventBufferEventResource.cpp
 * Author: ricky
 *
 * Created on 2014年5月19日, 上午 9:48
 */

#include "EventBufferEventResource.h"

namespace HPHP {

    EventBufferEventResource::EventBufferEventResource(event_buffer_event_t *event_buffer_event):InternalResource((void*) event_buffer_event) {
    }

    void EventBufferEventResource::setCallback(const Variant &readcb, const Variant &writecb, const Variant &eventcb)
    {
        this->readcb = &readcb;
        this->writecb = &writecb;
        this->eventcb = &eventcb;
    }

    void EventBufferEventResource::setObject(const Object *obj) {
        this->object = obj;
    }

    const Object *EventBufferEventResource::getObject() {
        return object;
    }

    void EventBufferEventResource::setArg(const Variant *arg) {
        this->arg = arg;
    }

    const Variant *EventBufferEventResource::getArg() {
        return arg;
    }

    const Variant* EventBufferEventResource::getReadCB()
    {
        return readcb;
    }

    const Variant* EventBufferEventResource::getWriteCB()
    {
        return writecb;
    }

    const Variant* EventBufferEventResource::getEventCB()
    {
        return eventcb;
    }

    EventBufferEventResource::~EventBufferEventResource() {
    }

}