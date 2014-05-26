#include "ext.h"
#ifdef HAVE_SYS_UN_H
    #include <sys/un.h>
#endif

namespace HPHP {

    static void bevent_read_cb(event_buffer_event_t *bevent, void *data)
    {
        EventBufferEventResource *EBEResource = (EventBufferEventResource *) data;
        vm_call_user_func(Object(EBEResource->getReadCB()), make_packed_array(Object(EBEResource->getObjectData()), *EBEResource->getArg()));
    }

    static void bevent_write_cb(event_buffer_event_t *bevent, void *data)
    {
        EventBufferEventResource *EBEResource = (EventBufferEventResource *) data;
        vm_call_user_func(Object(EBEResource->getWriteCB()), make_packed_array(Object(EBEResource->getObjectData()), *EBEResource->getArg()));
    }

    static void bevent_event_cb(event_buffer_event_t *bevent, short events, void *data)
    {
        EventBufferEventResource *EBEResource = (EventBufferEventResource *) data;
        vm_call_user_func(Object(EBEResource->getEventCB()), make_packed_array(Object(EBEResource->getObjectData()), events, *EBEResource->getArg()));
    }

    static void HHVM_METHOD(EventBufferEvent, __construct, const Object &event_base, const Resource &socket, int64_t options, const Object &readcb, const Object &writecb, const Object &eventcb, const Variant &arg) {
        evutil_socket_t fd;
        event_buffer_event_t *bevent;
        bufferevent_data_cb  read_cb;
        bufferevent_data_cb  write_cb;
        bufferevent_event_cb event_cb;
        Resource resource;
        InternalResource *EBResource = FETCH_RESOURCE(event_base, InternalResource, s_eventbase);
        event_base_t *base = (event_base_t *)EBResource->getInternalResource();
        if(socket.isNull()){
            fd = -1;
            options |= BEV_OPT_CLOSE_ON_FREE;
        }
        else{
            Socket *sock = socket.getTyped<Socket>();
            fd = sock->fd();
            if(fd < 0){
                raise_error("valid PHP socket resource expected");
            }
            evutil_make_socket_nonblocking(fd);
        }
        bevent = bufferevent_socket_new(base, fd, options);
        if(bevent == NULL){
            raise_error("Failed to allocate bufferevent for socket");
        }
        resource = Resource(NEWOBJ(EventBufferEventResource(bevent, this_.get())));
        SET_RESOURCE(this_, resource, s_eventbufferevent);

        EventBufferEventResource *EBEResource = FETCH_RESOURCE(this_, EventBufferEventResource, s_eventbufferevent);
        readcb.get()->incRefCount();
        writecb.get()->incRefCount();
        eventcb.get()->incRefCount();

        EBEResource->setCallback(readcb.get(), writecb.get(), eventcb.get());
        EBEResource->setArg(&arg);

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
        if(EBEResource->getReadCB()!=NULL){
            EBEResource->getReadCB()->decRefCount();
        }
        if(EBEResource->getWriteCB()!=NULL){
            EBEResource->getWriteCB()->decRefCount();
        }
        if(EBEResource->getEventCB()!=NULL){
            EBEResource->getEventCB()->decRefCount();
        }
    }

    static bool HHVM_METHOD(EventBufferEvent, enable, int64_t events) {
        EventBufferEventResource *EBEResource = FETCH_RESOURCE(this_, EventBufferEventResource, s_eventbufferevent);
        return bufferevent_enable((event_buffer_event_t*) EBEResource->getInternalResource(), events) == 0 ? true:false;
    }

    static bool HHVM_METHOD(EventBufferEvent, connect, const String &addr) {
        struct sockaddr_storage  ss;
        int ss_len   = sizeof(ss);
        memset(&ss, 0, sizeof(ss));
        EventBufferEventResource *EBEResource = FETCH_RESOURCE(this_, EventBufferEventResource, s_eventbufferevent);

#ifdef HAVE_SYS_UN_H
        if(strncasecmp(addr.get()->data(), s_domain_socket_prefix.get()->data(), s_domain_socket_prefix.size())){
            struct sockaddr_un *ss_un;
            ss_un = (struct sockaddr_un *) ss_un;
            ss_un->sun_family = AF_UNIX;
            ss_len = sizeof(struct sockaddr_un);
            strncpy(ss_un->sun_path, &addr.get()->data()[s_domain_socket_prefix.size()], UNIX_PATH_MAX);
        }
        else
#endif
        if(true){
            if (evutil_parse_sockaddr_port(addr.get()->data(), (struct sockaddr *) &ss, &ss_len) == -1) {
                return false;
            }
        }

        return bufferevent_socket_connect((event_buffer_event_t *) EBEResource->getInternalResource(), (struct sockaddr *) &ss, ss_len) == 0?true:false;
    }

    void eventExtension::_initEventBufferEventClass() {
        HHVM_ME(EventBufferEvent, __construct);
        HHVM_ME(EventBufferEvent, connect);
        HHVM_ME(EventBufferEvent, free);
        HHVM_ME(EventBufferEvent, enable);
    }
}
