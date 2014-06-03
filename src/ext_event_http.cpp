#include "ext.h"

#ifdef HAVE_EVENT_HTTP_SUPPORT
namespace HPHP {
    //using JIT::VMRegAnchor;
    static void HHVM_METHOD(EventHttp, __construct, const Object &base, const Object &ctx) {
        Resource resource;
      /*  if (config.isNull()) {
            resource = Resource(NEWOBJ(InternalResource(event_base_new())));
        } else {
            resource = Resource(NEWOBJ(InternalResource(event_base_new_with_config((event_config_t *) FETCH_RESOURCE(config.toObject(), InternalResource, s_eventconfig)->getInternalResource()))));
        }
        SET_RESOURCE(this_, resource, s_eventbase);*/
    }

    void eventExtension::_initEventHttpClass() {
        HHVM_ME(EventHttp, __construct);
    }
}
#endif