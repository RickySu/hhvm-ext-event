#include "ext.h"
#include "hphp/runtime/base/thread-init-fini.h"
namespace HPHP
{
    static void HHVM_METHOD(Event, __construct, const Object &eventcb)
    {
        //Resource resource = Resource(NEWOBJ(EventResource(event_config_new())));
        //SET_RESOURCE(this_, resource, s_eventconfig);
    }

    static void HHVM_METHOD(Event, __destruct)
    {
        //EventResource *ECResource = FETCH_RESOURCE(this_, EventResource, s_eventconfig);
        //event_config_free((event_config_t *) ECResource->getInternalResource());
    }

    void eventExtension::_initEventClass()
    {
        HHVM_ME(Event, __construct);
        HHVM_ME(Event, __destruct);
    }
}
