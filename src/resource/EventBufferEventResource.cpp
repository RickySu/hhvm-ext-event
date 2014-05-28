/*
 * File:   EventBufferEventResource.cpp
 * Author: ricky
 *
 * Created on 2014年5月19日, 上午 9:48
 */

#include "EventBufferEventResource.h"

namespace HPHP {
    IMPLEMENT_OBJECT_ALLOCATION(EventBufferEventResource)

    EventBufferEventResource::EventBufferEventResource(event_buffer_event_t *event_buffer_event, ObjectData *object):InternalResource((void*) event_buffer_event) {
        this->object = object;
    }

    ObjectData *EventBufferEventResource::getObjectData(){
        return this->object;
    }

    void EventBufferEventResource::setCallback(ObjectData *readcb, ObjectData *writecb, ObjectData *eventcb)
    {
        this->readcb = readcb;
        this->writecb = writecb;
        this->eventcb = eventcb;
    }

    void EventBufferEventResource::setArg(const Variant &arg) {
        this->arg = &arg;
    }

    const Variant &EventBufferEventResource::getArg() {
        return *arg;
    }

    ObjectData* EventBufferEventResource::getReadCB()
    {
        return readcb;
    }

    ObjectData* EventBufferEventResource::getWriteCB()
    {
        return writecb;
    }

    ObjectData* EventBufferEventResource::getEventCB()
    {
        return eventcb;
    }

    EventBufferEventResource::~EventBufferEventResource() {
    }

}