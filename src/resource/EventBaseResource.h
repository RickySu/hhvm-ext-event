/*
 * File:   EventBaseResource.h
 * Author: ricky
 *
 * Created on 2014年5月19日, 上午 9:48
 */

#ifndef EVENTBASERESOURCE_H
#define	EVENTBASERESOURCE_H

#include <event2/event.h>
#include "InternalResource.h"

namespace HPHP {

    class EventBaseResource : public InternalResource {
    public:
        EventBaseResource(struct event_base *event_base);
        virtual ~EventBaseResource();
    private:

    };
}

#endif	/* EVENTBASERESOURCE_H */

