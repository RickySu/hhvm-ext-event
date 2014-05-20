#include "ext_event.h"

namespace HPHP
{

    static void HHVM_METHOD(EventConfig, __construct)
    {
        Resource resource = Resource(NEWOBJ(EventConfigResource(event_config_new())));
        SET_RESOURCE(this_, resource, s_eventconfig);
    }

    static void HHVM_METHOD(EventConfig, __destruct)
    {
        delete FETCH_RESOURCE(this_, EventConfigResource, s_eventconfig);
    }

    static bool HHVM_METHOD(EventConfig, avoidMethod, const String& method)
    {
        EventConfigResource *ECResource = FETCH_RESOURCE(this_, EventConfigResource, s_eventconfig);
        return event_config_avoid_method((event_config_t *) ECResource->getInternalResource(), method.c_str()) == 0?true:false;
    }

    static bool HHVM_METHOD(EventConfig, requireFeatures, int64_t feature)
    {
        EventConfigResource *ECResource = FETCH_RESOURCE(this_, EventConfigResource, s_eventconfig);
        return event_config_require_features((event_config_t *) ECResource->getInternalResource(), feature) == 0?true:false;
    }

    static void HHVM_METHOD(EventConfig, setMaxDispatchInterval, double max_interval, int64_t max_callbacks, int64_t min_priority)
    {
#ifdef HAVE_DISPATCH_INTERVAL_FUNCTION
        struct timeval tv;
        EventConfigResource *ECResource = FETCH_RESOURCE(this_, EventConfigResource, s_eventconfig);

        TIMEVAL_SET(tv, max_interval);

        if (max_interval > 0) {
            event_config_set_max_dispatch_interval((event_config_t *) ECResource->getInternalResource(), &tv, max_callbacks, min_priority);
        }
        else{
            event_config_set_max_dispatch_interval((event_config_t *) ECResource->getInternalResource(), NULL, max_callbacks, min_priority);
        }
#else
        throw NotImplementedException("Available since libevent 2.1.0-alpha");
#endif
    }

    void eventExtension::_initEventConfigClass()
    {
        HHVM_ME(EventConfig, __construct);
        HHVM_ME(EventConfig, __destruct);
        HHVM_ME(EventConfig, avoidMethod);
        HHVM_ME(EventConfig, requireFeatures);
        HHVM_ME(EventConfig, setMaxDispatchInterval);
    }
}
