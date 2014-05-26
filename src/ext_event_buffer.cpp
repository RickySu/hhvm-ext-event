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

    static int64_t HHVM_METHOD(EventBuffer, appendFrom, const Object &buf, int64_t len) {
        InternalResource *resource_dst = FETCH_RESOURCE(this_, InternalResource, s_eventbuffer);
        InternalResource *resource_src = FETCH_RESOURCE(buf, InternalResource, s_eventbuffer);
        return evbuffer_remove_buffer((event_buffer_t *) resource_dst->getInternalResource(), (event_buffer_t *) resource_src->getInternalResource(), len);
    }

    static int64_t HHVM_METHOD(EventBuffer, copyout, VRefParam data, int64_t max_bytes) {
        char *tmp_data = new char[max_bytes+1];
        ev_ssize_t read_size;
        InternalResource *resource = FETCH_RESOURCE(this_, InternalResource, s_eventbuffer);
        read_size = evbuffer_copyout((event_buffer_t *) resource->getInternalResource(), tmp_data, max_bytes);
        if(read_size > 0){
            data = StringData::Make(tmp_data, read_size, CopyString);
        }
        tmp_data[max_bytes+1] = 0;
        delete tmp_data;
        return read_size;


    }

    void eventExtension::_initEventBufferClass()
    {
        HHVM_ME(EventBuffer, __construct);
        HHVM_ME(EventBuffer, add);
        HHVM_ME(EventBuffer, addBuffer);
        HHVM_ME(EventBuffer, appendFrom);
        HHVM_ME(EventBuffer, copyout);
    }

}
