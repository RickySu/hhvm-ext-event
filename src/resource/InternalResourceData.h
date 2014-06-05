/*
 * File:   EventBaseResource.h
 * Author: ricky
 *
 * Created on 2014年5月14日, 下午 12:13
 */

#ifndef InternalResourceData_H
#define	InternalResourceData_H

#include "../../config.h"
#include "../h/use_event.h"
#include "hphp/runtime/base/base-includes.h"

namespace HPHP {

    class InternalResourceData: public SweepableResourceData {
    public:
        InternalResourceData(void *resource = NULL);
        ~InternalResourceData();
        DECLARE_RESOURCE_ALLOCATION(InternalResourceData)
        CLASSNAME_IS("InternalResourceData")
        virtual const String& o_getClassNameHook() const { return classnameof(); }
        void * getInternalResourceData();
        void setInternalResourceData(void *resource);
        void setObject(const Object &obj);
        const Object *getObject();
        bool isInternal = false;
    private:
        void *resource;
        const Object *object_data;
    };
}
#endif	/* EVENTBASERESOURCE_H */

