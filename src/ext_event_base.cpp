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

    static bool HHVM_METHOD(EventBase, loopexit, double timeout)
    {
        int res;
        struct timeval tv;
        auto var = this_->o_get(s_event_base, true, s_eventbase.get());

        if (var.isNull()) {
            return false;
        }

        EventBaseResource *EBResource = var.asCResRef().getTyped<EventBaseResource>();

        printf("timeout = %lf\n", timeout);

        if(timeout == 0){
            res = event_base_loopexit((struct event_base *) EBResource->getInternalResource(), NULL);
        }
        else{
            TIMEVAL_SET(tv, timeout);
            res = event_base_loopexit((struct event_base *) EBResource->getInternalResource(), &tv);
        }
        return res==0?true:false;
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

    static bool HHVM_METHOD(EventBase, reInit)
    {
        auto var = this_->o_get(s_event_base, true, s_eventbase.get());

        if (var.isNull()) {
            return false;
        }

        EventBaseResource *EBResource = var.asCResRef().getTyped<EventBaseResource>();
        return event_reinit((struct event_base *) EBResource->getInternalResource())==0?true:false;
    }

    static bool HHVM_METHOD(EventBase, stop)
    {
        auto var = this_->o_get(s_event_base, true, s_eventbase.get());

        if (var.isNull()) {
            return false;
        }

        EventBaseResource *EBResource = var.asCResRef().getTyped<EventBaseResource>();
        return event_base_loopbreak((struct event_base *) EBResource->getInternalResource())==0?true:false;
    }

    static bool HHVM_METHOD(EventBase, loop, int64_t flags)
    {
        int res;

        auto var = this_->o_get(s_event_base, true, s_eventbase.get());

        if (var.isNull()) {
            return false;
        }

        EventBaseResource *EBResource = var.asCResRef().getTyped<EventBaseResource>();

        if(flags == 0){
            res = event_base_dispatch((struct event_base *) EBResource->getInternalResource());
        }
        else{
            res = event_base_loop((struct event_base *) EBResource->getInternalResource(), flags);
        }
        return res == 0?true:false;
    }

    void eventExtension::_initEventBaseClass()
    {
        HHVM_ME(EventBase, __construct);
        HHVM_ME(EventBase, __destruct);
        HHVM_ME(EventBase, dispatch);
        HHVM_ME(EventBase, loopexit);
        HHVM_ME(EventBase, priorityInit);
        HHVM_ME(EventBase, reInit);
        HHVM_ME(EventBase, stop);
        HHVM_ME(EventBase, loop);
    }
}
