#include "ext_event.h"
#include <stdio.h>
namespace HPHP {

    static void HHVM_METHOD(EventBase, __construct, const Object config) {
        Resource resource;
        if (config.isNull()) {
            printf("event_base_new\n");
            resource = Resource(NEWOBJ(EventBaseResource(event_base_new())));
        } else {
            printf("event_base_new_with_config\n");
            auto var = config->o_get(s_event_config, true, s_eventconfig.get());
            resource = Resource(NEWOBJ(EventBaseResource(event_base_new_with_config((event_config_t *) var.asCResRef().getTyped<EventConfigResource>()->getInternalResource()))));
        }
        this_->o_set(s_event_base, resource, s_eventbase.get());
    }

    static void HHVM_METHOD(EventBase, __destruct) {
        auto var = this_->o_get(s_event_base, true, s_eventbase.get());

        if (var.isNull()) {
            return;
        }

        delete var.asCResRef().getTyped<EventBaseResource>();
        printf("release\n");
    }

    static void HHVM_METHOD(EventBase, dispatch) {
        auto var = this_->o_get(s_event_base, true, s_eventbase.get());

        if (var.isNull()) {
            return;
        }

        EventBaseResource *EBResource = var.asCResRef().getTyped<EventBaseResource>();
        event_base_dispatch((event_base_t *) EBResource->getInternalResource());
    }

    static bool HHVM_METHOD(EventBase, loopexit, double timeout) {
        int res;
        struct timeval tv;
        auto var = this_->o_get(s_event_base, true, s_eventbase.get());

        if (var.isNull()) {
            return false;
        }

        EventBaseResource *EBResource = var.asCResRef().getTyped<EventBaseResource>();

        printf("timeout = %lf\n", timeout);

        if (timeout == 0) {
            res = event_base_loopexit((event_base_t *) EBResource->getInternalResource(), NULL);
        } else {
            TIMEVAL_SET(tv, timeout);
            res = event_base_loopexit((event_base_t *) EBResource->getInternalResource(), &tv);
        }
        return res == 0 ? true : false;
    }

    static bool HHVM_METHOD(EventBase, priorityInit, int64_t prio) {
        auto var = this_->o_get(s_event_base, true, s_eventbase.get());

        if (var.isNull()) {
            return false;
        }

        EventBaseResource *EBResource = var.asCResRef().getTyped<EventBaseResource>();
        return event_base_priority_init((event_base_t *) EBResource->getInternalResource(), prio) == 0 ? true : false;
    }

    static bool HHVM_METHOD(EventBase, reInit) {
        auto var = this_->o_get(s_event_base, true, s_eventbase.get());

        if (var.isNull()) {
            return false;
        }

        EventBaseResource *EBResource = var.asCResRef().getTyped<EventBaseResource>();
        return event_reinit((event_base_t *) EBResource->getInternalResource()) == 0 ? true : false;
    }

    static bool HHVM_METHOD(EventBase, stop) {
        auto var = this_->o_get(s_event_base, true, s_eventbase.get());

        if (var.isNull()) {
            return false;
        }

        EventBaseResource *EBResource = var.asCResRef().getTyped<EventBaseResource>();
        return event_base_loopbreak((event_base_t *) EBResource->getInternalResource()) == 0 ? true : false;
    }

    static bool HHVM_METHOD(EventBase, loop, int64_t flags) {
        int res;

        auto var = this_->o_get(s_event_base, true, s_eventbase.get());

        if (var.isNull()) {
            return false;
        }

        EventBaseResource *EBResource = var.asCResRef().getTyped<EventBaseResource>();

        if (flags == 0) {
            res = event_base_dispatch((event_base_t *) EBResource->getInternalResource());
        } else {
            res = event_base_loop((event_base_t *) EBResource->getInternalResource(), flags);
        }
        return res == 0 ? true : false;
    }

    static int64_t HHVM_METHOD(EventBase, getFeatures) {
        auto var = this_->o_get(s_event_base, true, s_eventbase.get());

        if (var.isNull()) {
            return -1;
        }

        EventBaseResource *EBResource = var.asCResRef().getTyped<EventBaseResource>();

        return event_base_get_features((event_base_t *) EBResource->getInternalResource());
    }

    static Variant HHVM_METHOD(EventBase, getMethod) {
        auto var = this_->o_get(s_event_base, true, s_eventbase.get());

        if (var.isNull()) {
            return null_variant;
        }

        EventBaseResource *EBResource = var.asCResRef().getTyped<EventBaseResource>();

        String ret((char *) event_base_get_method((event_base_t *) EBResource->getInternalResource()));
        return ret;
    }

    static Variant HHVM_METHOD(EventBase, getTimeOfDayCached) {
        struct timeval tv;
        auto var = this_->o_get(s_event_base, true, s_eventbase.get());

        if (var.isNull()) {
            return null_variant;
        }

        EventBaseResource *EBResource = var.asCResRef().getTyped<EventBaseResource>();
        event_base_gettimeofday_cached((event_base_t *) EBResource->getInternalResource(), &tv);
        return (double) TIMEVAL_TO_DOUBLE(tv);
    }

    static bool HHVM_METHOD(EventBase, gotExit) {
        auto var = this_->o_get(s_event_base, true, s_eventbase.get());

        if (var.isNull()) {
            return false;
        }

        EventBaseResource *EBResource = var.asCResRef().getTyped<EventBaseResource>();
        return event_base_got_exit((event_base_t *) EBResource->getInternalResource()) != 0;
    }

    static bool HHVM_METHOD(EventBase, gotStop) {
        auto var = this_->o_get(s_event_base, true, s_eventbase.get());

        if (var.isNull()) {
            return false;
        }

        EventBaseResource *EBResource = var.asCResRef().getTyped<EventBaseResource>();
        return event_base_got_break((event_base_t *) EBResource->getInternalResource()) != 0;
    }

    void eventExtension::_initEventBaseClass() {
        HHVM_ME(EventBase, __construct);
        HHVM_ME(EventBase, __destruct);
        HHVM_ME(EventBase, dispatch);
        HHVM_ME(EventBase, loopexit);
        HHVM_ME(EventBase, getFeatures);
        HHVM_ME(EventBase, getMethod);
        HHVM_ME(EventBase, getTimeOfDayCached);
        HHVM_ME(EventBase, gotExit);
        HHVM_ME(EventBase, gotStop);
        HHVM_ME(EventBase, loop);
        HHVM_ME(EventBase, priorityInit);
        HHVM_ME(EventBase, reInit);
        HHVM_ME(EventBase, stop);
    }
}
