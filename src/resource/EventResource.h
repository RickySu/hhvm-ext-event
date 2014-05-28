/*
 * File:   EventResource.h
 * Author: ricky
 *
 * Created on 2014年5月19日, 上午 9:48
 */

#ifndef EVENTRESOURCE_H
#define	EVENTRESOURCE_H

#include <event2/event.h>
#include <event2/util.h>
#include <event2/bufferevent.h>
#include "InternalResource.h"

namespace HPHP {

    class EventResource : public InternalResource {
    public:
        DECLARE_RESOURCE_ALLOCATION(EventResource)
        CLASSNAME_IS("EventResource")
        EventResource(event_t *event, ObjectData *object);
        virtual ~EventResource();
        void setCallback(ObjectData *cb);
        ObjectData *getCallback();
        void setArg(const Variant &arg);
        const Variant &getArg();
        void setFd(const Resource &fd);
        const Resource &getFd();
        ObjectData *getObjectData();
    private:
        ObjectData *cb, *object;
        const Variant *arg;
        const Resource *fd;
    };
}

#endif	/* EVENTRESOURCE_H */

