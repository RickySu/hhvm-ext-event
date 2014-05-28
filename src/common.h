#ifndef COMMON_H
#define	COMMON_H

#define UNIX_PATH_MAX    108

namespace HPHP
{
    const StaticString
        s_domain_socket_prefix("unix:"),
        s_internal_resource("__internal_resource"),
        s_eventbase("EventBase"),
        s_eventconfig("EventConfig"),
        s_eventbufferevent("EventBufferEvent"),
        s_eventbufferevent_input("input"),
        s_eventbufferevent_output("output"),
        s_eventbuffer("EventBuffer"),
        s_event("Event")
    ;
}
#endif	/* COMMON_H */

