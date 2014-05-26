#include "ext.h"

namespace HPHP
{

    static void HHVM_METHOD(EventBuffer, __construct)
    {
        Resource resource = Resource(NEWOBJ(InternalResource(evbuffer_new())));
        SET_RESOURCE(this_, resource, s_eventbuffer);
    }

    static bool HHVM_METHOD(EventBuffer, add, const String &data) {
        InternalResource *resource = FETCH_RESOURCE(this_, InternalResource, s_eventbuffer);
        return evbuffer_add((event_buffer_t *) resource->getInternalResource(), data.c_str(), data.size()) == 0?true:false;
    }

    static bool HHVM_METHOD(EventBuffer, addBuffer, const Object &buf) {
        InternalResource *resource_dst = FETCH_RESOURCE(this_, InternalResource, s_eventbuffer);
        InternalResource *resource_src = FETCH_RESOURCE(buf, InternalResource, s_eventbuffer);
        return evbuffer_add_buffer((event_buffer_t *) resource_dst->getInternalResource(), (event_buffer_t *) resource_src->getInternalResource()) == 0?true:false;
    }

    void eventExtension::_initEventBufferClass()
    {
        HHVM_ME(EventBuffer, __construct);
        HHVM_ME(EventBuffer, add);
        HHVM_ME(EventBuffer, addBuffer);
    }

}
