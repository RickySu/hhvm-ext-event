/*
 * File:   EventResourceData.cpp
 * Author: ricky
 *
 * Created on 2014年5月19日, 上午 9:48
 */

#include "EventResourceData.h"

namespace HPHP {

    IMPLEMENT_OBJECT_ALLOCATION(EventResourceData)

    EventResourceData::EventResourceData(event_t *event, ObjectData *object_data):InternalResourceData((void*) event) {
        this->object_data = object_data;
    }

    ObjectData *EventResourceData::getObjectData(){
        return this->object_data;
    }

    void EventResourceData::setCallback(ObjectData *cb)
    {
        this->cb = cb;
    }

    void EventResourceData::setArg(const Variant &arg) {
        this->arg = new Variant(arg);
    }

    const Variant &EventResourceData::getArg() {
        return *arg;
    }

    void EventResourceData::setFd(const Resource &fd) {
        this->fd = new Resource(fd);
    }

    const Resource &EventResourceData::getFd() {
        return *fd;
    }

    ObjectData* EventResourceData::getCallback()
    {
        return cb;
    }

    EventResourceData::~EventResourceData() {
    }

}