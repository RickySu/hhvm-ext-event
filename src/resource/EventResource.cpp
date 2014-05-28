/*
 * File:   EventResource.cpp
 * Author: ricky
 *
 * Created on 2014年5月19日, 上午 9:48
 */

#include "EventResource.h"

namespace HPHP {

    void EventResource::sweep() {
        delete arg;
        delete fd;
        delete this;
    }

    EventResource::EventResource(event_t *event, ObjectData *object):InternalResource((void*) event) {
        this->object = object;
    }

    ObjectData *EventResource::getObjectData(){
        return this->object;
    }

    void EventResource::setCallback(ObjectData *cb)
    {
        this->cb = cb;
    }

    void EventResource::setArg(const Variant &arg) {
        this->arg = new Variant(arg);
    }

    const Variant &EventResource::getArg() {
        return *arg;
    }

    void EventResource::setFd(const Resource &fd) {
        this->fd = new Resource(fd);
    }

    const Resource &EventResource::getFd() {
        return *fd;
    }

    ObjectData* EventResource::getCallback()
    {
        return cb;
    }

    EventResource::~EventResource() {
    }

}