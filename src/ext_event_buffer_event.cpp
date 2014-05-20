#include "ext_event.h"
namespace HPHP {

    static void bevent_read_cb(event_buffer_event_t *bevent, void *data)
    {
        Array param;
        EventBufferEventResource *EBEResource = (EventBufferEventResource *) data;
        param.append(*EBEResource->getObject());
        param.append(*EBEResource->getArg());
        f_call_user_func_array(*EBEResource->getReadCB(), param);
    }

    static void bevent_write_cb(event_buffer_event_t *bevent, void *data)
    {
        Array param;
        EventBufferEventResource *EBEResource = (EventBufferEventResource *) data;
        param.append(*EBEResource->getObject());
        param.append(*EBEResource->getArg());
        f_call_user_func_array(*EBEResource->getWriteCB(), param);
    }

    static void bevent_event_cb(event_buffer_event_t *bevent, short events, void *data)
    {
        Array param;
        EventBufferEventResource *EBEResource = (EventBufferEventResource *) data;
        param.append(*EBEResource->getObject());
        param.append(events);
        param.append(*EBEResource->getArg());
        f_call_user_func_array(*EBEResource->getEventCB(), param);
    }

    static void HHVM_METHOD(EventBufferEvent, __construct, const Object &event_base, const Resource &socket, int64_t options, const Variant &readcb, const Variant &writecb, const Variant &eventcb, const Variant &arg) {
        evutil_socket_t fd;
        event_buffer_event_t *bevent;
        bufferevent_data_cb  read_cb;
        bufferevent_data_cb  write_cb;
        bufferevent_event_cb event_cb;
        Resource resource;

        EventBaseResource *EBResource = FETCH_RESOURCE(event_base, EventBaseResource, s_eventbase);
        if(socket.isNull()){
            fd = -1;
            options |= BEV_OPT_CLOSE_ON_FREE;
        }
        bevent = bufferevent_socket_new((event_base_t *)EBResource->getInternalResource(), fd, options);
        resource = Resource(NEWOBJ(EventBufferEventResource(bevent)));
        SET_RESOURCE(this_, resource, s_eventbufferevent);

        EventBufferEventResource *EBEResource = FETCH_RESOURCE(this_, EventBufferEventResource, s_eventbufferevent);

        EBEResource->setCallback(readcb, writecb, eventcb);
        EBEResource->setObject(&this_);
        EBEResource->setArg(&eventcb);

        read_cb = readcb.isNull()?(bufferevent_data_cb)NULL:bevent_read_cb;
        write_cb = writecb.isNull()?(bufferevent_data_cb)NULL:bevent_write_cb;
        event_cb = eventcb.isNull()?(bufferevent_event_cb)NULL:bevent_event_cb;

        if(read_cb || write_cb || event_cb ){
            bufferevent_setcb(bevent, read_cb, write_cb, event_cb, (void *) EBEResource);
        }
    }

    static void HHVM_METHOD(EventBufferEvent, free) {
        EventBufferEventResource *EBEResource = FETCH_RESOURCE(this_, EventBufferEventResource, s_eventbufferevent);
        bufferevent_free((event_buffer_event_t*) EBEResource->getInternalResource());
    }

    void eventExtension::_initEventBufferEventClass() {
        HHVM_ME(EventBufferEvent, __construct);
        HHVM_ME(EventBufferEvent, free);
    }
}
