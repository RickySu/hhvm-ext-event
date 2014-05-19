#include "ext_event.h"
#include <stdio.h>
namespace HPHP
{
    const StaticString
        s_eventbase("EventBase"),
        s_event_base("__event_base")
    ;
    static void HHVM_METHOD(EventBase, __construct, const Object config)
    {
        Resource resource = Resource(NEWOBJ(EventBaseResource(event_base_new())));
        this_->o_set(s_event_base, resource, s_eventbase.get());
        printf("1111\n");
    }

    static void HHVM_METHOD(EventBase, __destruct)
    {
        auto var = this_->o_get(s_event_base, true, s_eventbase.get());

        if (var.isNull()) {
            return;
        }

        delete var.asCResRef().getTyped<EventBaseResource>();
        printf("release\n");
    }

    static void HHVM_METHOD(EventBase, dispatch)
    {
        auto var = this_->o_get(s_event_base, true, s_eventbase.get());

        if (var.isNull()) {
            return;
        }

        EventBaseResource *EBResource = var.asCResRef().getTyped<EventBaseResource>();
        event_base_dispatch((struct event_base *) EBResource->getInternalResource());
    }

    static bool HHVM_METHOD(EventBase, priorityInit, int64_t prio)
    {
        auto var = this_->o_get(s_event_base, true, s_eventbase.get());

        if (var.isNull()) {
            return false;
        }

        EventBaseResource *EBResource = var.asCResRef().getTyped<EventBaseResource>();
        return event_base_priority_init((struct event_base *) EBResource->getInternalResource(), prio) == 0 ?true:false;
    }

    void eventExtension::_initEventBaseClass()
    {
        HHVM_ME(EventBase, __construct);
        HHVM_ME(EventBase, __destruct);
        HHVM_ME(EventBase, dispatch);
        HHVM_ME(EventBase, priorityInit);
    }
}
