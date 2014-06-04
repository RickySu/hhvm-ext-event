/*
 * File:   EventBufferEventResourceData.h
 * Author: ricky
 *
 * Created on 2014年5月19日, 上午 9:48
 */

#ifndef EventBufferEventResourceData_H
#define	EventBufferEventResourceData_H

#include <event2/event.h>
#include <event2/util.h>
#include <event2/bufferevent.h>
#include "InternalResourceData.h"

namespace HPHP {

    class EventBufferEventResourceData : public InternalResourceData {
    public:
        DECLARE_RESOURCE_ALLOCATION(EventBufferEventResourceData)
        CLASSNAME_IS("EventBufferEventResourceData")
        EventBufferEventResourceData(event_buffer_event_t *event_buffer_event, ObjectData *object_data);
        virtual ~EventBufferEventResourceData();
        void setCallback(ObjectData *readcb, ObjectData *writecb, ObjectData *eventcb);
        ObjectData *getReadCB();
        ObjectData *getWriteCB();
        ObjectData *getEventCB();
        void setArg(const Variant &arg);
        const Variant &getArg();
        ObjectData *getObjectData();
        bool isInternal = false;
    private:
        ObjectData *readcb, *writecb, *eventcb, *object_data;
        const Variant *arg;
    };
}

#endif	/* EventBufferEventResourceData_H */

