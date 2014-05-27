/*
 * File:   EventBufferEventResource.h
 * Author: ricky
 *
 * Created on 2014年5月19日, 上午 9:48
 */

#ifndef EVENTBUFFEREVENTRESOURCE_H
#define	EVENTBUFFEREVENTRESOURCE_H

#include <event2/event.h>
#include <event2/util.h>
#include <event2/bufferevent.h>
#include "InternalResource.h"

namespace HPHP {

    class EventBufferEventResource : public InternalResource {
    public:
        DECLARE_RESOURCE_ALLOCATION(EventBufferEventResource)
        CLASSNAME_IS("EventBufferEventResource")
        EventBufferEventResource(event_buffer_event_t *event_buffer_event, ObjectData *object);
        virtual ~EventBufferEventResource();
        void setCallback(ObjectData *readcb, ObjectData *writecb, ObjectData *eventcb);
        ObjectData *getReadCB();
        ObjectData *getWriteCB();
        ObjectData *getEventCB();
        void setArg(const Variant *arg);
        const Variant *getArg();
        ObjectData *getObjectData();
    private:
        ObjectData *readcb, *writecb, *eventcb, *object;
        const Variant *arg;
    };
}

#endif	/* EVENTBUFFEREVENTRESOURCE_H */

