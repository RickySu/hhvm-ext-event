/*
 * File:   EventConfigResource.h
 * Author: ricky
 *
 * Created on 2014年5月19日, 下午 3:15
 */

#ifndef EVENTCONFIGRESOURCE_H
#define	EVENTCONFIGRESOURCE_H

#include <event2/event.h>
#include <event2/event-config.h>
#include "InternalResource.h"

typedef struct event_config event_config_t;

namespace HPHP {

    class EventConfigResource : public InternalResource {
    public:
        EventConfigResource(event_config_t *event_config);
        virtual ~EventConfigResource();
    };
}

#endif	/* EVENTCONFIGRESOURCE_H */

