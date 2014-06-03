#include "ext.h"

#ifdef HAVE_LIBEVENT_HTTP_SUPPORT
namespace HPHP {

    void event_http_cb(evhttp_request_t *request, void *data) {
        EventResource *EResource = (EventResource *) data;
        vm_call_user_func(Object(EResource->getCallback()), make_packed_array(EResource->getArg()));
    }

    void event_http_default_cb(evhttp_request_t *request, void *data) {
        EventResource *EResource = (EventResource *) data;
        vm_call_user_func(Object(EResource->getCallback()), make_packed_array(EResource->getArg()));
    }

    static void HHVM_METHOD(EventHttp, __construct, const Object &base) {
        evhttp_t *http;
        InternalResource *EBResource = FETCH_RESOURCE(base, InternalResource, s_eventbase);
        event_base_t *event_base = (event_base_t *)EBResource->getInternalResource();
        http = evhttp_new(event_base);
        if(!http){
            raise_error("Failed to allocate space for new HTTP server(evhttp_new)");
        }

        Resource resource = Resource(NEWOBJ(EventHttpResource(http)));
        SET_RESOURCE(this_, resource, s_eventhttp);
    }

    static bool HHVM_METHOD(EventHttp, accept, const Resource &socket) {
        evutil_socket_t fd;

        EventHttpResource *resource = FETCH_RESOURCE(this_, EventHttpResource, s_eventhttp);
        Socket *sock = socket.getTyped<Socket>();
        fd = sock->fd();

        if(fd < 0){
            raise_error("valid PHP socket resource expected");
            return false;
        }

        evutil_make_socket_nonblocking(fd);
        return evhttp_accept_socket((evhttp_t *)resource->getInternalResource(), fd) == 0;
    }

    static bool HHVM_METHOD(EventHttp, addServerAlias, const String &alias) {
        EventHttpResource *resource = FETCH_RESOURCE(this_, EventHttpResource, s_eventhttp);
        return evhttp_add_server_alias((evhttp_t *)resource->getInternalResource(), alias.c_str()) == 0;
    }

    static bool HHVM_METHOD(EventHttp, bind, const String &address, int64_t port) {
        EventHttpResource *resource = FETCH_RESOURCE(this_, EventHttpResource, s_eventhttp);
        return evhttp_bind_socket((evhttp_t *)resource->getInternalResource(), address.c_str(), port) == 0;
    }

    static bool HHVM_METHOD(EventHttp, removeServerAlias, const String &alias) {
        EventHttpResource *resource = FETCH_RESOURCE(this_, EventHttpResource, s_eventhttp);
        return evhttp_remove_server_alias((evhttp_t *)resource->getInternalResource(), alias.c_str()) == 0;
    }

    static void HHVM_METHOD(EventHttp, setAllowedMethods, int64_t methods) {
        EventHttpResource *resource = FETCH_RESOURCE(this_, EventHttpResource, s_eventhttp);
        evhttp_set_allowed_methods((evhttp_t *)resource->getInternalResource(), methods);
    }

    static void HHVM_METHOD(EventHttp, setCallback, const String &path, const Object &cb, const Variant &arg) {
        EventHttpResource *resource = FETCH_RESOURCE(this_, EventHttpResource, s_eventhttp);
        resource->setCallback(cb.get());
        cb.get()->incRefCount();
        resource->setCallback(cb.get());
        resource->setCallbackArg(arg);
        evhttp_set_cb((evhttp_t *)resource->getInternalResource(), path.c_str(), event_http_cb, (void *) resource);
    }

    static void HHVM_METHOD(EventHttp, setDefaultCallback, const Object &cb, const Variant &arg) {
        EventHttpResource *resource = FETCH_RESOURCE(this_, EventHttpResource, s_eventhttp);
        resource->setCallback(cb.get());
        cb.get()->incRefCount();
        resource->setCallback(cb.get());
        resource->setCallbackArg(arg);
        evhttp_set_gencb((evhttp_t *)resource->getInternalResource(), event_http_default_cb, (void *) resource);
    }

    static void HHVM_METHOD(EventHttp, setMaxBodySize, int64_t value) {
        EventHttpResource *resource = FETCH_RESOURCE(this_, EventHttpResource, s_eventhttp);
        evhttp_set_max_body_size((evhttp_t *)resource->getInternalResource(), value);
    }

    static void HHVM_METHOD(EventHttp, setMaxHeadersSize, int64_t value) {
        EventHttpResource *resource = FETCH_RESOURCE(this_, EventHttpResource, s_eventhttp);
        evhttp_set_max_headers_size((evhttp_t *)resource->getInternalResource(), value);
    }

    static void HHVM_METHOD(EventHttp, setTimeout, int64_t value) {
        EventHttpResource *resource = FETCH_RESOURCE(this_, EventHttpResource, s_eventhttp);
        evhttp_set_timeout((evhttp_t *)resource->getInternalResource(), value);
    }

    void eventExtension::_initEventHttpClass() {
        HHVM_ME(EventHttp, __construct);
        HHVM_ME(EventHttp, accept);
        HHVM_ME(EventHttp, addServerAlias);
        HHVM_ME(EventHttp, bind);
        HHVM_ME(EventHttp, removeServerAlias);
        HHVM_ME(EventHttp, setAllowedMethods);
        HHVM_ME(EventHttp, setCallback);
        HHVM_ME(EventHttp, setDefaultCallback);
        HHVM_ME(EventHttp, setMaxBodySize);
        HHVM_ME(EventHttp, setMaxHeadersSize);
        HHVM_ME(EventHttp, setTimeout);
    }
}
#endif