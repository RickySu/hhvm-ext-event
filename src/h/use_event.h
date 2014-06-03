/*
 * File:   use_event.h
 * Author: ricky
 *
 * Created on 2014年5月28日, 下午 5:35
 */

#ifndef USE_EVENT_H
#define	USE_EVENT_H

#include <event2/event.h>
#include <event2/util.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/thread.h>

#ifdef HAVE_LIBEVENT_SSL_SUPPORT
    #include <event2/bufferevent_ssl.h>
#endif

#ifdef HAVE_LIBEVENT_HTTP_SUPPORT
    #include <event2/http.h>
    typedef struct evhttp evhttp_t;
    typedef struct evhttp_request evhttp_request_t;
#endif

typedef struct event_base event_base_t;
typedef struct event_config event_config_t;
typedef struct bufferevent event_buffer_event_t;
typedef struct evbuffer event_buffer_t;
typedef struct event event_t;

#endif	/* USE_EVENT_H */

