/*
 * File:   EventBufferEventResourceData.cpp
 * Author: ricky
 *
 * Created on 2014年5月19日, 上午 9:48
 */

#include "EventBufferEventResourceData.h"

namespace HPHP {
    IMPLEMENT_OBJECT_ALLOCATION(EventBufferEventResourceData)

    EventBufferEventResourceData::EventBufferEventResourceData(event_buffer_event_t *event_buffer_event, ObjectData *object):InternalResourceData((void*) event_buffer_event) {
        this->object = object;
    }

    ObjectData *EventBufferEventResourceData::getObjectData(){
        return this->object;
    }

    void EventBufferEventResourceData::setCallback(ObjectData *readcb, ObjectData *writecb, ObjectData *eventcb)
    {
        this->readcb = readcb;
        this->writecb = writecb;
        this->eventcb = eventcb;
    }

    void EventBufferEventResourceData::setArg(const Variant &arg) {
        this->arg = new Variant(arg);
    }

    const Variant &EventBufferEventResourceData::getArg() {
        return *arg;
    }

    ObjectData* EventBufferEventResourceData::getReadCB()
    {
        return readcb;
    }

    ObjectData* EventBufferEventResourceData::getWriteCB()
    {
        return writecb;
    }

    ObjectData* EventBufferEventResourceData::getEventCB()
    {
        return eventcb;
    }

    EventBufferEventResourceData::~EventBufferEventResourceData() {
    }

}