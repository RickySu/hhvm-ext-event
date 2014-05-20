#include "ext_event.h"
namespace HPHP {

    static void HHVM_METHOD(EventBase, __construct, const Object &config) {
        Resource resource;
        if (config.isNull()) {
            resource = Resource(NEWOBJ(EventBaseResource(event_base_new())));
        } else {
            resource = Resource(NEWOBJ(EventBaseResource(event_base_new_with_config((event_config_t *) FETCH_RESOURCE(config, EventConfigResource, s_eventconfig)->getInternalResource()))));
        }
        SET_RESOURCE(this_, resource, s_eventbase);
    }

    static void HHVM_METHOD(EventBase, __destruct) {
        EventBaseResource *EBResource = FETCH_RESOURCE(this_, EventBaseResource, s_eventbase);
        event_base_free((event_base_t *) EBResource->getInternalResource());
    }

    static void HHVM_METHOD(EventBase, dispatch) {
        EventBaseResource *EBResource = FETCH_RESOURCE(this_, EventBaseResource, s_eventbase);
        event_base_dispatch((event_base_t *) EBResource->getInternalResource());
    }

    static bool HHVM_METHOD(EventBase, loopexit, double timeout) {
        int res;
        struct timeval tv;

        EventBaseResource *EBResource = FETCH_RESOURCE(this_, EventBaseResource, s_eventbase);

        if (timeout == 0) {
            res = event_base_loopexit((event_base_t *) EBResource->getInternalResource(), NULL);
        } else {
            TIMEVAL_SET(tv, timeout);
            res = event_base_loopexit((event_base_t *) EBResource->getInternalResource(), &tv);
        }
        return res == 0 ? true : false;
    }

    static bool HHVM_METHOD(EventBase, priorityInit, int64_t prio) {
        EventBaseResource *EBResource = FETCH_RESOURCE(this_, EventBaseResource, s_eventbase);
        return event_base_priority_init((event_base_t *) EBResource->getInternalResource(), prio) == 0 ? true : false;
    }

    static bool HHVM_METHOD(EventBase, reInit) {
        EventBaseResource *EBResource = FETCH_RESOURCE(this_, EventBaseResource, s_eventbase);
        return event_reinit((event_base_t *) EBResource->getInternalResource()) == 0 ? true : false;
    }

    static bool HHVM_METHOD(EventBase, stop) {
        EventBaseResource *EBResource = FETCH_RESOURCE(this_, EventBaseResource, s_eventbase);
        return event_base_loopbreak((event_base_t *) EBResource->getInternalResource()) == 0 ? true : false;
    }

    static bool HHVM_METHOD(EventBase, loop, int64_t flags) {
        int res;

        EventBaseResource *EBResource = FETCH_RESOURCE(this_, EventBaseResource, s_eventbase);

        if (flags == 0) {
            res = event_base_dispatch((event_base_t *) EBResource->getInternalResource());
        } else {
            res = event_base_loop((event_base_t *) EBResource->getInternalResource(), flags);
        }
        return res == 0 ? true : false;
    }

    static int64_t HHVM_METHOD(EventBase, getFeatures) {
        EventBaseResource *EBResource = FETCH_RESOURCE(this_, EventBaseResource, s_eventbase);
        return event_base_get_features((event_base_t *) EBResource->getInternalResource());
    }

    static Variant HHVM_METHOD(EventBase, getMethod) {
        EventBaseResource *EBResource = FETCH_RESOURCE(this_, EventBaseResource, s_eventbase);
        String ret((char *) event_base_get_method((event_base_t *) EBResource->getInternalResource()));
        return ret;
    }

    static Variant HHVM_METHOD(EventBase, getTimeOfDayCached) {
        struct timeval tv;
        EventBaseResource *EBResource = FETCH_RESOURCE(this_, EventBaseResource, s_eventbase);
        event_base_gettimeofday_cached((event_base_t *) EBResource->getInternalResource(), &tv);
        return (double) TIMEVAL_TO_DOUBLE(tv);
    }

    static bool HHVM_METHOD(EventBase, gotExit) {
        EventBaseResource *EBResource = FETCH_RESOURCE(this_, EventBaseResource, s_eventbase);
        return event_base_got_exit((event_base_t *) EBResource->getInternalResource()) != 0;
    }

    static bool HHVM_METHOD(EventBase, gotStop) {
        EventBaseResource *EBResource = FETCH_RESOURCE(this_, EventBaseResource, s_eventbase);
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
