#ifndef COMMON_H
#define	COMMON_H

#define UNIX_PATH_MAX    108

namespace HPHP
{
    const StaticString
        s_domain_socket_prefix("unix:"),
        s_internal_resource("__internal_resource"),
        s_event_base("EventBase"),
        s_event_config("EventConfig"),
        s_event_bufferevent("EventBufferEvent"),
        s_event_bufferevent_input("input"),
        s_event_bufferevent_output("output"),
        s_event_buffer("EventBuffer"),
        s_event("Event"),
        s_event_ssl_context("EventSSLContext"),
        s_event_http("EventHttp"),
        s_event_http_request("EventHttpRequest"),
        s_event_dns_base("EventDNSBase")
    ;
}
#endif	/* COMMON_H */

