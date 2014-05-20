/*
 * File:   EventBaseResource.h
 * Author: ricky
 *
 * Created on 2014年5月14日, 下午 12:13
 */

#ifndef INTERNALRESOURCE_H
#define	INTERNALRESOURCE_H

#include "hphp/runtime/base/base-includes.h"
#include <event2/event.h>

namespace HPHP {

    class InternalResource : public SweepableResourceData {
    public:
        InternalResource(void *resource);
        ~InternalResource();
        DECLARE_RESOURCE_ALLOCATION(InternalResource)
        CLASSNAME_IS("InternalResource")
        virtual const String& o_getClassNameHook() const { return classnameof(); }
        void * getInternalResource();
    private:
        void *resource;
    };
}
#endif	/* EVENTBASERESOURCE_H */

