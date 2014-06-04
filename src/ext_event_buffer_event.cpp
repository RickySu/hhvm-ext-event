#include "ext.h"
#ifdef HAVE_SYS_UN_H
    #include <sys/un.h>
#endif

namespace HPHP {

    static void bevent_read_cb(event_buffer_event_t *bevent, void *data)
    {
        EventBufferEventResourceData *event_buffer_event_resource_data = (EventBufferEventResourceData *) data;
        vm_call_user_func(Object(event_buffer_event_resource_data->getReadCB()), make_packed_array(Object(event_buffer_event_resource_data->getObjectData()), event_buffer_event_resource_data->getArg()));
    }

    static void bevent_write_cb(event_buffer_event_t *bevent, void *data)
    {
        EventBufferEventResourceData *event_buffer_event_resource_data = (EventBufferEventResourceData *) data;
        vm_call_user_func(Object(event_buffer_event_resource_data->getWriteCB()), make_packed_array(Object(event_buffer_event_resource_data->getObjectData()), event_buffer_event_resource_data->getArg()));
    }

    static void bevent_event_cb(event_buffer_event_t *bevent, short events, void *data)
    {
        EventBufferEventResourceData *event_buffer_event_resource_data = (EventBufferEventResourceData *) data;
        vm_call_user_func(Object(event_buffer_event_resource_data->getEventCB()), make_packed_array(Object(event_buffer_event_resource_data->getObjectData()), events, event_buffer_event_resource_data->getArg()));
    }

    ALWAYS_INLINE void freeCB(EventBufferEventResourceData *event_buffer_event_resource_data) {
        if(event_buffer_event_resource_data->getReadCB()!=NULL){
            event_buffer_event_resource_data->getReadCB()->decRefCount();
        }
        if(event_buffer_event_resource_data->getWriteCB()!=NULL){
            event_buffer_event_resource_data->getWriteCB()->decRefCount();
        }
        if(event_buffer_event_resource_data->getEventCB()!=NULL){
            event_buffer_event_resource_data->getEventCB()->decRefCount();
        }
    }

    ALWAYS_INLINE void setCB(EventBufferEventResourceData *event_buffer_event_resource_data, const Object &readcb, const Object &writecb, const Object &eventcb) {
        bufferevent_data_cb  read_cb;
        bufferevent_data_cb  write_cb;
        bufferevent_event_cb event_cb;
        event_buffer_event_resource_data->setCallback(readcb.get(), writecb.get(), eventcb.get());
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
            bufferevent_setcb((event_buffer_event_t*) event_buffer_event_resource_data->getInternalResourceData(), read_cb, write_cb, event_cb, (void *) event_buffer_event_resource_data);
        }
    }

    ALWAYS_INLINE void initEventBufferEvent(event_buffer_event_t *bevent, const Object &event_buffer_event, const Object &readcb, const Object &writecb, const Object &eventcb, const Variant &arg) {
        Resource resource;
        resource = Resource(NEWOBJ(EventBufferEventResourceData(bevent, event_buffer_event.get())));
        SET_RESOURCE(event_buffer_event, resource, s_eventbufferevent);
        EventBufferEventResourceData *event_buffer_event_resource_data = FETCH_RESOURCE(event_buffer_event, EventBufferEventResourceData, s_eventbufferevent);
        event_buffer_event_resource_data->setArg(arg);
        setCB(event_buffer_event_resource_data, readcb, writecb, eventcb);
        Object inputBuffer = makeObject("EventBuffer");
        Resource inputBufferResource = Resource(NEWOBJ(InternalResourceData(evbuffer_new())));
        event_buffer_event->o_set(s_eventbufferevent_input, inputBuffer, s_eventbufferevent);
        Object outputBuffer = makeObject("EventBuffer");
        Resource outputBufferResource = Resource(NEWOBJ(InternalResourceData(evbuffer_new())));
        event_buffer_event->o_set(s_eventbufferevent_output, outputBuffer, s_eventbufferevent);
    }

    static void HHVM_METHOD(EventBufferEvent, __construct, const Object &base, const Resource &socket, int64_t options, const Object &readcb, const Object &writecb, const Object &eventcb, const Variant &arg) {
        evutil_socket_t fd;
        event_buffer_event_t *bevent;
        InternalResourceData *event_base_resource_data = FETCH_RESOURCE(base, InternalResourceData, s_eventbase);
        event_base_t *event_base = (event_base_t *)event_base_resource_data->getInternalResourceData();
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
        bevent = bufferevent_socket_new(event_base, fd, options);
        if(bevent == NULL){
            raise_error("Failed to allocate bufferevent for socket");
        }

        initEventBufferEvent(bevent, this_, readcb, writecb, eventcb, arg);
    }

    static void HHVM_METHOD(EventBufferEvent, free) {
        EventBufferEventResourceData *event_buffer_event_resource_data = FETCH_RESOURCE(this_, EventBufferEventResourceData, s_eventbufferevent);
        event_buffer_event_t *bev = (event_buffer_event_t*) event_buffer_event_resource_data->getInternalResourceData();
        if(bev == NULL){
            raise_error("Failed to free bufferevent resource");
        }
        event_buffer_event_resource_data->setInternalResourceData(NULL);
        bufferevent_free(bev);
        freeCB(event_buffer_event_resource_data);
    }

    static bool HHVM_METHOD(EventBufferEvent, enable, int64_t events) {
        EventBufferEventResourceData *event_buffer_event_resource_data = FETCH_RESOURCE(this_, EventBufferEventResourceData, s_eventbufferevent);
        return bufferevent_enable((event_buffer_event_t*) event_buffer_event_resource_data->getInternalResourceData(), events) == 0 ? true:false;
    }

    static bool HHVM_METHOD(EventBufferEvent, disable, int64_t events) {
        EventBufferEventResourceData *event_buffer_event_resource_data = FETCH_RESOURCE(this_, EventBufferEventResourceData, s_eventbufferevent);
        return bufferevent_disable((event_buffer_event_t*) event_buffer_event_resource_data->getInternalResourceData(), events) == 0 ? true:false;
    }

    static int64_t HHVM_METHOD(EventBufferEvent, getEnabled) {
        EventBufferEventResourceData *event_buffer_event_resource_data = FETCH_RESOURCE(this_, EventBufferEventResourceData, s_eventbufferevent);
        return bufferevent_get_enabled((event_buffer_event_t*) event_buffer_event_resource_data->getInternalResourceData());
    }

    static bool HHVM_METHOD(EventBufferEvent, connect, const String &addr) {
        struct sockaddr_storage  ss;
        int ss_len   = sizeof(ss);
        memset(&ss, 0, sizeof(ss));
        EventBufferEventResourceData *event_buffer_event_resource_data = FETCH_RESOURCE(this_, EventBufferEventResourceData, s_eventbufferevent);

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

        return bufferevent_socket_connect((event_buffer_event_t *) event_buffer_event_resource_data->getInternalResourceData(), (struct sockaddr *) &ss, ss_len) == 0;
    }

    static Variant HHVM_METHOD(EventBufferEvent, read, int64_t size) {
        size_t read_size;
        char *data = new char[size];
        EventBufferEventResourceData *event_buffer_event_resource_data = FETCH_RESOURCE(this_, EventBufferEventResourceData, s_eventbufferevent);
        read_size = bufferevent_read((event_buffer_event_t *) event_buffer_event_resource_data->getInternalResourceData(), (void *)data, size);

        if(read_size == 0){
            delete data;
            return false;
        }

        String ret(data, read_size, CopyString);
        delete data;
        return ret;
    }

    static bool HHVM_METHOD(EventBufferEvent, setPriority, int64_t priority) {
        EventBufferEventResourceData *event_buffer_event_resource_data = FETCH_RESOURCE(this_, EventBufferEventResourceData, s_eventbufferevent);
        return bufferevent_priority_set((event_buffer_event_t *) event_buffer_event_resource_data->getInternalResourceData(), priority) == 0;
    }

    static void HHVM_METHOD(EventBufferEvent, setCallbacks, const Object &readcb, const Object &writecb, const Object &eventcb, const Variant &arg) {
        EventBufferEventResourceData *event_buffer_event_resource_data = FETCH_RESOURCE(this_, EventBufferEventResourceData, s_eventbufferevent);
        freeCB(event_buffer_event_resource_data);
        event_buffer_event_resource_data->setArg(arg);
        setCB(event_buffer_event_resource_data, readcb, writecb, eventcb);
    }

    static bool HHVM_METHOD(EventBufferEvent, setTimeouts, double timeout_read, double timeout_write) {
        EventBufferEventResourceData *event_buffer_event_resource_data = FETCH_RESOURCE(this_, EventBufferEventResourceData, s_eventbufferevent);
        struct timeval tv_read;
        struct timeval tv_write;
        TIMEVAL_SET(tv_read, timeout_read);
        TIMEVAL_SET(tv_write, timeout_write);
        return bufferevent_set_timeouts((event_buffer_event_t *) event_buffer_event_resource_data->getInternalResourceData(), &tv_read, &tv_write) == 0;
    }

    static void HHVM_METHOD(EventBufferEvent, setWatermark, int64_t events, int64_t lowmark, int64_t highmark) {
        EventBufferEventResourceData *event_buffer_event_resource_data = FETCH_RESOURCE(this_, EventBufferEventResourceData, s_eventbufferevent);
        bufferevent_setwatermark((event_buffer_event_t *) event_buffer_event_resource_data->getInternalResourceData(), events, lowmark, highmark);
    }

    static bool HHVM_METHOD(EventBufferEvent, write, const String &data) {
        EventBufferEventResourceData *event_buffer_event_resource_data = FETCH_RESOURCE(this_, EventBufferEventResourceData, s_eventbufferevent);
        return bufferevent_write((event_buffer_event_t *) event_buffer_event_resource_data->getInternalResourceData(), data.c_str(), data.size()) == 0;
    }

    static bool HHVM_METHOD(EventBufferEvent, writeBuffer, const Object &buf) {
        EventBufferEventResourceData *event_buffer_event_resource_data = FETCH_RESOURCE(this_, EventBufferEventResourceData, s_eventbufferevent);
        InternalResourceData *resource_src = FETCH_RESOURCE(buf, InternalResourceData, s_eventbuffer);
        return bufferevent_write_buffer((event_buffer_event_t *) event_buffer_event_resource_data->getInternalResourceData(), (event_buffer_t *) resource_src->getInternalResourceData()) == 0;
    }

    static bool HHVM_METHOD(EventBufferEvent, readBuffer, const Object &buf) {
        EventBufferEventResourceData *event_buffer_event_resource_data = FETCH_RESOURCE(this_, EventBufferEventResourceData, s_eventbufferevent);
        InternalResourceData *resource_src = FETCH_RESOURCE(buf, InternalResourceData, s_eventbuffer);
        return bufferevent_read_buffer((event_buffer_event_t *) event_buffer_event_resource_data->getInternalResourceData(), (event_buffer_t *) resource_src->getInternalResourceData()) == 0;
    }

    static Variant HHVM_STATIC_METHOD(EventBufferEvent, createPair, const Object &base, int64_t options) {
        event_buffer_event_t *bevent_pair[2];
        InternalResourceData *event_base_resource_data = FETCH_RESOURCE(base, InternalResourceData, s_eventbase);
        event_base_t *event_base = (event_base_t *)event_base_resource_data->getInternalResourceData();

        if(bufferevent_pair_new(event_base, options, bevent_pair)){
            return false;
        }

        return make_packed_array(makeObject("EventBufferEvent", make_packed_array(base)), makeObject("EventBufferEvent", make_packed_array(base)));
    }

#ifdef HAVE_LIBEVENT_SSL_SUPPORT

    ALWAYS_INLINE bool is_valid_ssl_state(int64_t state)
    {
            return  (state == BUFFEREVENT_SSL_OPEN
                    || state == BUFFEREVENT_SSL_CONNECTING
                    || state == BUFFEREVENT_SSL_ACCEPTING);
    }

    static String HHVM_METHOD(EventBufferEvent, sslError) {
        unsigned long e;
        char buf[512];
        EventBufferEventResourceData *event_buffer_event_resource_data = FETCH_RESOURCE(this_, EventBufferEventResourceData, s_eventbufferevent);
        e = bufferevent_get_openssl_error((event_buffer_event_t *) event_buffer_event_resource_data->getInternalResourceData());
        if(e){
            return StringData::Make(ERR_error_string(e, buf));
        }
        return false;
    }

    static void HHVM_METHOD(EventBufferEvent, sslRenegotiate) {
        EventBufferEventResourceData *event_buffer_event_resource_data = FETCH_RESOURCE(this_, EventBufferEventResourceData, s_eventbufferevent);
        bufferevent_ssl_renegotiate((event_buffer_event_t *) event_buffer_event_resource_data->getInternalResourceData());
    }

    static Object HHVM_STATIC_METHOD(EventBufferEvent, sslFilter, const Object &base, const Object &underlying, const Object ctx, int64_t state, int64_t options) {
        SSL *ssl;
        event_buffer_event_t *bevent;
        InternalResourceData *event_base_resource_data = FETCH_RESOURCE(base, InternalResourceData, s_eventbase);
        EventBufferEventResourceData *event_buffer_event_resource_data = FETCH_RESOURCE(underlying, EventBufferEventResourceData, s_eventbufferevent);
        EventSSLContextResourceData *event_ssl_context_resource_data = FETCH_RESOURCE(underlying, EventSSLContextResourceData, s_event_ssl_context);

        if (!is_valid_ssl_state(state)) {
            raise_warning("Invalid state specified");
            return NULL;
        }

        if(!(ssl = SSL_new((SSL_CTX *) event_ssl_context_resource_data->getInternalResourceData()))){
            raise_warning("Event: Failed creating SSL handle");
            return NULL;
        }

        if((bevent = bufferevent_openssl_filter_new((event_base_t *) event_base_resource_data->getInternalResourceData(), (event_buffer_event_t *) event_buffer_event_resource_data->getInternalResourceData(), ssl, (bufferevent_ssl_state) state, options)) == NULL){
            raise_warning("Failed to allocate bufferevent filter");
            return NULL;
        }

        String ClassName("EventBufferEvent");
        Object event_buffer_event = ObjectData::newInstance(Unit::lookupClass(ClassName.get()));
        initEventBufferEvent(bevent, event_buffer_event, Object(), Object(), Object(), Variant());
        return event_buffer_event;
    }

    static Object HHVM_STATIC_METHOD(EventBufferEvent, sslSocket, const Object &base, const Resource &socket, const Object ctx, int64_t state, int64_t options) {
        evutil_socket_t fd;
        SSL *ssl;
        event_buffer_event_t *bevent;
        InternalResourceData *event_base_resource_data = FETCH_RESOURCE(base, InternalResourceData, s_eventbase);
        EventSSLContextResourceData *event_ssl_context_resource_data = FETCH_RESOURCE(ctx, EventSSLContextResourceData, s_event_ssl_context);

        if (!is_valid_ssl_state(state)) {
            raise_warning("Invalid state specified");
            return NULL;
        }

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

        if(!(ssl = SSL_new((SSL_CTX *) event_ssl_context_resource_data->getInternalResourceData()))){
            raise_warning("Event: Failed creating SSL handle");
            return NULL;
        }

        SSL_set_ex_data(ssl, event_ssl_data_index, event_ssl_context_resource_data);

        if((bevent = bufferevent_openssl_socket_new((event_base_t *) event_base_resource_data->getInternalResourceData(), fd, ssl, (bufferevent_ssl_state) state, options))){
            raise_warning("Failed to allocate bufferevent ssl socket");
            return NULL;
        }

        String ClassName("EventBufferEvent");
        Object event_buffer_event = ObjectData::newInstance(Unit::lookupClass(ClassName.get()));
        initEventBufferEvent(bevent, event_buffer_event, Object(), Object(), Object(), Variant());
        return event_buffer_event;

    }

#endif

    void eventExtension::_initEventBufferEventClass() {
        HHVM_ME(EventBufferEvent, __construct);
        HHVM_ME(EventBufferEvent, connect);
        HHVM_ME(EventBufferEvent, free);
        HHVM_ME(EventBufferEvent, enable);
        HHVM_ME(EventBufferEvent, disable);
        HHVM_ME(EventBufferEvent, getEnabled);
        HHVM_ME(EventBufferEvent, read);
        HHVM_ME(EventBufferEvent, readBuffer);
        HHVM_ME(EventBufferEvent, setCallbacks);
        HHVM_ME(EventBufferEvent, setPriority);
        HHVM_ME(EventBufferEvent, setTimeouts);
        HHVM_ME(EventBufferEvent, setWatermark);
#ifdef HAVE_LIBEVENT_SSL_SUPPORT
        HHVM_ME(EventBufferEvent, sslError);
        HHVM_ME(EventBufferEvent, sslRenegotiate);
        HHVM_STATIC_ME(EventBufferEvent, sslFilter);
        HHVM_STATIC_ME(EventBufferEvent, sslSocket);
#endif
        HHVM_ME(EventBufferEvent, write);
        HHVM_ME(EventBufferEvent, writeBuffer);
        HHVM_STATIC_ME(EventBufferEvent, createPair);
    }
}
