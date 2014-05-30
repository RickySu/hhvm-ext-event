/*
 * File:   EventBaseResource.h
 * Author: ricky
 *
 * Created on 2014年5月14日, 下午 12:13
 */

#ifndef INTERNALRESOURCE_H
#define	INTERNALRESOURCE_H

#include "../../config.h"
#include "hphp/runtime/base/base-includes.h"
#include <event2/event.h>

typedef struct event_base event_base_t;
typedef struct event_config event_config_t;
typedef struct bufferevent event_buffer_event_t;
typedef struct evbuffer event_buffer_t;
typedef struct event event_t;

namespace HPHP {

    class InternalResource: public SweepableResourceData {
    public:
        InternalResource(void *resource);
        ~InternalResource();
        DECLARE_RESOURCE_ALLOCATION(InternalResource)
        CLASSNAME_IS("InternalResource")
        virtual const String& o_getClassNameHook() const { return classnameof(); }
        void * getInternalResource();
        void setInternalResource(void *resource);
        void setObject(const Object &obj);
        const Object *getObject();
    private:
        void *resource;
        const Object *object;
    };
}
#endif	/* EVENTBASERESOURCE_H */

