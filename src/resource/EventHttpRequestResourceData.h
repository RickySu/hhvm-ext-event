/*
 * File:   EventHttpRequestResourceData.h
 * Author: ricky
 *
 * Created on 2014年5月19日, 上午 9:48
 */

#ifndef EventHttpRequestResourceData_H
#define	EventHttpRequestResourceData_H

#include <event2/event.h>
#include "InternalResourceData.h"

namespace HPHP {

    class EventHttpRequestResourceData : public InternalResourceData {
    public:
        DECLARE_RESOURCE_ALLOCATION(EventHttpRequestResourceData)
        CLASSNAME_IS("EventHttpRequestResourceData")
        EventHttpRequestResourceData();
        virtual ~EventHttpRequestResourceData();
        void setCallbackArg(const Variant &arg);
        const Variant &getCallbackArg();
        void setCallback(ObjectData *cb);
        ObjectData *getCallback();
    private:
        ObjectData *cb;
        const Variant *arg;
    };
}

#endif	/* EventHttpRequestResourceData_H */

