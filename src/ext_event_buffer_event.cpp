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

    inline void freeCB(EventBufferEventResource *EBEResource) {
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

    inline void setCB(EventBufferEventResource *EBEResource, const Object &readcb, const Object &writecb, const Object &eventcb, const Variant &arg) {
        bufferevent_data_cb  read_cb;
        bufferevent_data_cb  write_cb;
        bufferevent_event_cb event_cb;
        EBEResource->setCallback(readcb.get(), writecb.get(), eventcb.get());
        EBEResource->setArg(&arg);
        if(readcb.isNull()){
            read_cb = (bufferevent_data_cb) NULL;
        }
        else {
            readcb.get()->incRefCount();
            read_cb = bevent_read_cb;
        }
        if(writecb.isNull()){
            write_cb = (bufferevent_data_cb) NULL;
        }
        else {
            writecb.get()->incRefCount();
            write_cb = bevent_write_cb;
        }
        if(eventcb.isNull()){
            event_cb = (bufferevent_event_cb) NULL;
        }
        else {
            eventcb.get()->incRefCount();
            event_cb = bevent_event_cb;
        }
        if(read_cb || write_cb || event_cb ){
            bufferevent_setcb((event_buffer_event_t*) EBEResource->getInternalResource(), read_cb, write_cb, event_cb, (void *) EBEResource);
        }
    }

    static void HHVM_METHOD(EventBufferEvent, __construct, const Object &event_base, const Resource &socket, int64_t options, const Object &readcb, const Object &writecb, const Object &eventcb, const Variant &arg) {
        evutil_socket_t fd;
        event_buffer_event_t *bevent;
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
        setCB(EBEResource, readcb, writecb, eventcb, arg);
        Object inputBuffer = makeObject("EventBuffer");
        Resource inputBufferResource = Resource(NEWOBJ(InternalResource(evbuffer_new())));
        this_->o_set(s_eventbufferevent_input, inputBuffer, s_eventbufferevent);
        Object outputBuffer = makeObject("EventBuffer");
        Resource outputBufferResource = Resource(NEWOBJ(InternalResource(evbuffer_new())));
        this_->o_set(s_eventbufferevent_output, outputBuffer, s_eventbufferevent);
    }

    static void HHVM_METHOD(EventBufferEvent, free) {
        EventBufferEventResource *EBEResource = FETCH_RESOURCE(this_, EventBufferEventResource, s_eventbufferevent);
        event_buffer_event_t *bev = (event_buffer_event_t*) EBEResource->getInternalResource();
        if(bev == NULL){
            raise_error("Failed to free bufferevent resource");
        }
        EBEResource->setInternalResource(NULL);
        bufferevent_free(bev);
        freeCB(EBEResource);
    }

    static bool HHVM_METHOD(EventBufferEvent, enable, int64_t events) {
        EventBufferEventResource *EBEResource = FETCH_RESOURCE(this_, EventBufferEventResource, s_eventbufferevent);
        return bufferevent_enable((event_buffer_event_t*) EBEResource->getInternalResource(), events) == 0 ? true:false;
    }

    static bool HHVM_METHOD(EventBufferEvent, disable, int64_t events) {
        EventBufferEventResource *EBEResource = FETCH_RESOURCE(this_, EventBufferEventResource, s_eventbufferevent);
        return bufferevent_disable((event_buffer_event_t*) EBEResource->getInternalResource(), events) == 0 ? true:false;
    }

    static int64_t HHVM_METHOD(EventBufferEvent, getEnabled) {
        EventBufferEventResource *EBEResource = FETCH_RESOURCE(this_, EventBufferEventResource, s_eventbufferevent);
        return bufferevent_get_enabled((event_buffer_event_t*) EBEResource->getInternalResource());
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

        return bufferevent_socket_connect((event_buffer_event_t *) EBEResource->getInternalResource(), (struct sockaddr *) &ss, ss_len) == 0;
    }

    static Variant HHVM_METHOD(EventBufferEvent, read, int64_t size) {
        size_t read_size;
        char *data = new char[size];
        EventBufferEventResource *EBEResource = FETCH_RESOURCE(this_, EventBufferEventResource, s_eventbufferevent);
        read_size = bufferevent_read((event_buffer_event_t *) EBEResource->getInternalResource(), (void *)data, size);

        if(read_size == 0){
            delete data;
            return false;
        }

        String ret(data, read_size, CopyString);
        delete data;
        return ret;
    }

    static bool HHVM_METHOD(EventBufferEvent, setPriority, int64_t priority) {
        EventBufferEventResource *EBEResource = FETCH_RESOURCE(this_, EventBufferEventResource, s_eventbufferevent);
        return bufferevent_priority_set((event_buffer_event_t *) EBEResource->getInternalResource(), priority) == 0;
    }

    static void HHVM_METHOD(EventBufferEvent, setCallbacks, const Object &readcb, const Object &writecb, const Object &eventcb, const Variant &arg) {
        EventBufferEventResource *EBEResource = FETCH_RESOURCE(this_, EventBufferEventResource, s_eventbufferevent);
        freeCB(EBEResource);
        setCB(EBEResource, readcb, writecb, eventcb, arg);
    }

    static bool HHVM_METHOD(EventBufferEvent, setTimeouts, double timeout_read, double timeout_write) {
        EventBufferEventResource *EBEResource = FETCH_RESOURCE(this_, EventBufferEventResource, s_eventbufferevent);
        struct timeval tv_read;
        struct timeval tv_write;
        TIMEVAL_SET(tv_read, timeout_read);
        TIMEVAL_SET(tv_write, timeout_write);
        return bufferevent_set_timeouts((event_buffer_event_t *) EBEResource->getInternalResource(), &tv_read, &tv_write) == 0;
    }

    static void HHVM_METHOD(EventBufferEvent, setWatermark, int64_t events, int64_t lowmark, int64_t highmark) {
        EventBufferEventResource *EBEResource = FETCH_RESOURCE(this_, EventBufferEventResource, s_eventbufferevent);
        bufferevent_setwatermark((event_buffer_event_t *) EBEResource->getInternalResource(), events, lowmark, highmark);
    }

    static bool HHVM_METHOD(EventBufferEvent, write, const String &data) {
        EventBufferEventResource *EBEResource = FETCH_RESOURCE(this_, EventBufferEventResource, s_eventbufferevent);
        return bufferevent_write((event_buffer_event_t *) EBEResource->getInternalResource(), data.c_str(), data.size()) == 0;
    }

    static bool HHVM_METHOD(EventBufferEvent, writeBuffer, const Object &buf) {
        EventBufferEventResource *EBEResource = FETCH_RESOURCE(this_, EventBufferEventResource, s_eventbufferevent);
        InternalResource *resource_src = FETCH_RESOURCE(buf, InternalResource, s_eventbuffer);
        return bufferevent_write_buffer((event_buffer_event_t *) EBEResource->getInternalResource(), (event_buffer_t *) resource_src->getInternalResource()) == 0;
    }

    void eventExtension::_initEventBufferEventClass() {
        HHVM_ME(EventBufferEvent, __construct);
        HHVM_ME(EventBufferEvent, connect);
        HHVM_ME(EventBufferEvent, free);
        HHVM_ME(EventBufferEvent, enable);
        HHVM_ME(EventBufferEvent, disable);
        HHVM_ME(EventBufferEvent, getEnabled);
        HHVM_ME(EventBufferEvent, read);
        HHVM_ME(EventBufferEvent, setCallbacks);
        HHVM_ME(EventBufferEvent, setPriority);
        HHVM_ME(EventBufferEvent, setTimeouts);
        HHVM_ME(EventBufferEvent, setWatermark);
        HHVM_ME(EventBufferEvent, write);
        HHVM_ME(EventBufferEvent, writeBuffer);
    }
}
