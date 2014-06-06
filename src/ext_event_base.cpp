#include "ext.h"

namespace HPHP {
    using JIT::VMRegAnchor;
    static void HHVM_METHOD(EventBase, __construct, const Variant &config) {
        Resource resource;
        if (config.isNull()) {
            resource = Resource(NEWOBJ(InternalResourceData(event_base_new())));
        } else {
            resource = Resource(NEWOBJ(InternalResourceData(event_base_new_with_config((event_config_t *) FETCH_RESOURCE(config.toObject(), InternalResourceData, s_event_config)->getInternalResourceData()))));
        }
        SET_RESOURCE(this_, resource, s_event_base);
    }

    static void HHVM_METHOD(EventBase, __destruct) {
        InternalResourceData *event_base_resource_data = FETCH_RESOURCE(this_, InternalResourceData, s_event_base);
        event_base_free((event_base_t *) event_base_resource_data->getInternalResourceData());
    }

    static void HHVM_METHOD(EventBase, dispatch) {
        VMRegAnchor _;
        InternalResourceData *event_base_resource_data = FETCH_RESOURCE(this_, InternalResourceData, s_event_base);
        event_base_dispatch((event_base_t *) event_base_resource_data->getInternalResourceData());
    }

    static bool HHVM_METHOD(EventBase, loopexit, double timeout) {
        int res;
        struct timeval tv;

        InternalResourceData *event_base_resource_data = FETCH_RESOURCE(this_, InternalResourceData, s_event_base);

        if (timeout == 0) {
            res = event_base_loopexit((event_base_t *) event_base_resource_data->getInternalResourceData(), NULL);
        } else {
            TIMEVAL_SET(tv, timeout);
            res = event_base_loopexit((event_base_t *) event_base_resource_data->getInternalResourceData(), &tv);
        }
        return res == 0 ? true : false;
    }

    static bool HHVM_METHOD(EventBase, priorityInit, int64_t prio) {
        InternalResourceData *event_base_resource_data = FETCH_RESOURCE(this_, InternalResourceData, s_event_base);
        return event_base_priority_init((event_base_t *) event_base_resource_data->getInternalResourceData(), prio) == 0 ? true : false;
    }

    static bool HHVM_METHOD(EventBase, reInit) {
        InternalResourceData *event_base_resource_data = FETCH_RESOURCE(this_, InternalResourceData, s_event_base);
        return event_reinit((event_base_t *) event_base_resource_data->getInternalResourceData()) == 0 ? true : false;
    }

    static bool HHVM_METHOD(EventBase, stop) {
        InternalResourceData *event_base_resource_data = FETCH_RESOURCE(this_, InternalResourceData, s_event_base);
        return event_base_loopbreak((event_base_t *) event_base_resource_data->getInternalResourceData()) == 0 ? true : false;
    }

    static bool HHVM_METHOD(EventBase, loop, int64_t flags) {
        VMRegAnchor _;
        int res;
        InternalResourceData *event_base_resource_data = FETCH_RESOURCE(this_, InternalResourceData, s_event_base);

        if (flags == 0) {
            res = event_base_dispatch((event_base_t *) event_base_resource_data->getInternalResourceData());
        } else {
            res = event_base_loop((event_base_t *) event_base_resource_data->getInternalResourceData(), flags);
        }
        return res == 0 ? true : false;
    }

    static int64_t HHVM_METHOD(EventBase, getFeatures) {
        InternalResourceData *event_base_resource_data = FETCH_RESOURCE(this_, InternalResourceData, s_event_base);
        return event_base_get_features((event_base_t *) event_base_resource_data->getInternalResourceData());
    }

    static Variant HHVM_METHOD(EventBase, getMethod) {
        InternalResourceData *event_base_resource_data = FETCH_RESOURCE(this_, InternalResourceData, s_event_base);
        String ret((char *) event_base_get_method((event_base_t *) event_base_resource_data->getInternalResourceData()));
        return ret;
    }

    static double HHVM_METHOD(EventBase, getTimeOfDayCached) {
        struct timeval tv;
        InternalResourceData *event_base_resource_data = FETCH_RESOURCE(this_, InternalResourceData, s_event_base);
        event_base_gettimeofday_cached((event_base_t *) event_base_resource_data->getInternalResourceData(), &tv);
        return TIMEVAL_TO_DOUBLE(tv);
    }

    static bool HHVM_METHOD(EventBase, gotExit) {
        InternalResourceData *event_base_resource_data = FETCH_RESOURCE(this_, InternalResourceData, s_event_base);
        return event_base_got_exit((event_base_t *) event_base_resource_data->getInternalResourceData()) != 0;
    }

    static bool HHVM_METHOD(EventBase, gotStop) {
        InternalResourceData *event_base_resource_data = FETCH_RESOURCE(this_, InternalResourceData, s_event_base);
        return event_base_got_break((event_base_t *) event_base_resource_data->getInternalResourceData()) != 0;
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
