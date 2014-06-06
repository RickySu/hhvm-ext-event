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
    #include <event2/keyvalq_struct.h>
    typedef struct evhttp evhttp_t;
    typedef struct evhttp_request evhttp_request_t;
    typedef struct evkeyvalq evkeyvalq_t;
    typedef struct evkeyval evkeyval_t;
    typedef struct evhttp_connection evhttp_connection_t;
    #define EVENT_REQ_HEADER_INPUT 1
    #define EVENT_REQ_HEADER_OUTPUT 2
#endif

#ifdef HAVE_LIBEVENT_DNS_SUPPORT
    #include <event2/dns.h>
    typedef struct evdns_base evdns_base_t;
#endif

typedef struct event_base event_base_t;
typedef struct event_config event_config_t;
typedef struct bufferevent event_buffer_event_t;
typedef struct evbuffer event_buffer_t;
typedef struct event event_t;

#endif	/* USE_EVENT_H */

