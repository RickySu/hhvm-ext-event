/*
 * File:   EventHttpResourceData.h
 * Author: ricky
 *
 * Created on 2014年5月19日, 上午 9:48
 */

#ifndef EventHttpResourceData_H
#define	EventHttpResourceData_H

#include <event2/event.h>
#include "InternalResourceData.h"

namespace HPHP {

    class EventHttpResourceData : public InternalResourceData {
    public:
        DECLARE_RESOURCE_ALLOCATION(EventHttpResourceData)
        CLASSNAME_IS("EventHttpResourceData")
        EventHttpResourceData(evhttp_t *event);
        virtual ~EventHttpResourceData();
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

#endif	/* EventHttpResourceData_H */

