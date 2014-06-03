/*
 * File:   EventHttpResource.h
 * Author: ricky
 *
 * Created on 2014年5月19日, 上午 9:48
 */

#ifndef EVENTHTTPRESOURCE_H
#define	EVENTHTTPRESOURCE_H

#include <event2/event.h>
#include "InternalResource.h"

namespace HPHP {

    class EventHttpResource : public InternalResource {
    public:
        DECLARE_RESOURCE_ALLOCATION(EventHttpResource)
        CLASSNAME_IS("EventHttpResource")
        EventHttpResource(evhttp_t *event);
        virtual ~EventHttpResource();
        void setCallbackArg(const Variant &arg);
        const Variant &getCallbackArg();
        void setDefaultCallbackArg(const Variant &arg);
        const Variant &getDefaultCallbackArg();
        void setCallback(ObjectData *cb);
        ObjectData *getCallback();
        void setDefaultCallback(ObjectData *cb);
        ObjectData *getDefaultCallback();
    private:
        ObjectData *cb, *defaultCb;
        const Variant *arg, *defaultArg;
    };
}

#endif	/* EVENTHTTPRESOURCE_H */

