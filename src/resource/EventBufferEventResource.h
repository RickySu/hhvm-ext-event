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

typedef struct bufferevent event_buffer_event_t;

namespace HPHP {

    class EventBufferEventResource : public InternalResource {
    public:
        EventBufferEventResource(event_buffer_event_t *event_buffer_event);
        void setCallback(const Variant &readcb, const Variant &writecb, const Variant &eventcb);
        const Variant *getReadCB();
        const Variant *getWriteCB();
        const Variant *getEventCB();
        void setObject(const Object *obj);
        const Object *getObject();
        void setArg(const Variant *arg);
        const Variant *getArg();
        virtual ~EventBufferEventResource();
    private:
        const Variant *readcb, *writecb, *eventcb, *arg;
        const Object *object;
    };
}

#endif	/* EVENTBUFFEREVENTRESOURCE_H */

