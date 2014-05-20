#include "ext_event.h"

namespace HPHP
{

    static void HHVM_METHOD(EventConfig, __construct)
    {
        Resource resource = Resource(NEWOBJ(EventConfigResource(event_config_new())));
        this_->o_set(s_event_config, resource, s_eventconfig.get());
    }

    static void HHVM_METHOD(EventConfig, __destruct)
    {
        auto var = this_->o_get(s_event_config, true, s_eventconfig.get());

        if (var.isNull()) {
            return;
        }

        delete var.asCResRef().getTyped<EventConfigResource>();
    }

    static bool HHVM_METHOD(EventConfig, avoidMethod, const String& method)
    {
        auto var = this_->o_get(s_event_config, true, s_eventconfig.get());

        if (var.isNull()) {
            return false;
        }

        EventConfigResource *ECResource = var.asCResRef().getTyped<EventConfigResource>();

        return event_config_avoid_method((event_config_t *) ECResource->getInternalResource(), method.c_str()) == 0?true:false;
    }

    static bool HHVM_METHOD(EventConfig, requireFeatures, int64_t feature)
    {
        auto var = this_->o_get(s_event_config, true, s_eventconfig.get());

        if (var.isNull()) {
            return false;
        }

        EventConfigResource *ECResource = var.asCResRef().getTyped<EventConfigResource>();

        return event_config_require_features((event_config_t *) ECResource->getInternalResource(), feature) == 0?true:false;
    }
    
    static void HHVM_METHOD(EventConfig, setMaxDispatchInterval, double max_interval, int64_t max_callbacks, int64_t min_priority)
    {
#ifdef HAVE_DISPATCH_INTERVAL_FUNCTION    
        struct timeval tv;
        auto var = this_->o_get(s_event_config, true, s_eventconfig.get());

        if (var.isNull()) {
            return;
        }

        TIMEVAL_SET(tv, max_interval);
        EventConfigResource *ECResource = var.asCResRef().getTyped<EventConfigResource>();

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
