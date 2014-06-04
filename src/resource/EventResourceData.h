/*
 * File:   EventResourceData.h
 * Author: ricky
 *
 * Created on 2014年5月19日, 上午 9:48
 */

#ifndef EventResourceData_H
#define	EventResourceData_H

#include <event2/event.h>
#include <event2/util.h>
#include <event2/bufferevent.h>
#include "InternalResourceData.h"

namespace HPHP {

    class EventResourceData : public InternalResourceData {
    public:
        DECLARE_RESOURCE_ALLOCATION(EventResourceData)
        CLASSNAME_IS("EventResourceData")
        EventResourceData(event_t *event, ObjectData *object_data);
        virtual ~EventResourceData();
        void setCallback(ObjectData *cb);
        ObjectData *getCallback();
        void setArg(const Variant &arg);
        const Variant &getArg();
        void setFd(const Resource &fd);
        const Resource &getFd();
        ObjectData *getObjectData();
    private:
        ObjectData *cb, *object_data;
        const Variant *arg;
        const Resource *fd;
    };
}

#endif	/* EventResourceData_H */

