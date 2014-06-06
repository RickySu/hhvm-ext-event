#include "ext.h"

#ifdef HAVE_LIBEVENT_HTTP_SUPPORT
namespace HPHP {

    ALWAYS_INLINE Object makeEventHttpRequestObject(evhttp_request_t *request) {
        Object http_request = ObjectData::newInstance(Unit::lookupClass(String("EventHttpRequest").get()));
        Resource resource = Resource(NEWOBJ(EventHttpRequestResourceData(http_request.get())));
        SET_RESOURCE(http_request, resource, s_event_http_request);
        EventHttpRequestResourceData *resource_data = FETCH_RESOURCE(http_request, EventHttpRequestResourceData, s_event_http_request);
        evhttp_request_own(request);
        resource_data->setInternalResourceData((void *) request);
        resource_data->isInternal = true;
        return http_request;
    }

    static void event_http_cb(evhttp_request_t *request, void *data) {
        EventHttpResourceData *resource_data = (EventHttpResourceData *) data;
        vm_call_user_func(Object(resource_data->getCallback()), make_packed_array(makeEventHttpRequestObject(request), resource_data->getCallbackArg()));
    }

    static void event_http_default_cb(evhttp_request_t *request, void *data) {
        EventHttpResourceData *resource_data = (EventHttpResourceData *) data;
        vm_call_user_func(Object(resource_data->getDefaultCallback()), make_packed_array(makeEventHttpRequestObject(request), resource_data->getDefaultCallbackArg()));
    }

    static void HHVM_METHOD(EventHttp, __construct, const Object &base) {
        evhttp_t *http;
        InternalResourceData *event_base_resource_data = FETCH_RESOURCE(base, InternalResourceData, s_event_base);
        event_base_t *event_base = (event_base_t *)event_base_resource_data->getInternalResourceData();
        http = evhttp_new(event_base);

        if(!http){
            raise_error("Failed to allocate space for new HTTP server(evhttp_new)");
        }

        Resource resource = Resource(NEWOBJ(EventHttpResourceData(http)));
        SET_RESOURCE(this_, resource, s_event_http);
    }

    static bool HHVM_METHOD(EventHttp, accept, const Resource &socket) {
        evutil_socket_t fd;
        EventHttpResourceData *resource_data = FETCH_RESOURCE(this_, EventHttpResourceData, s_event_http);
        Socket *sock = socket.getTyped<Socket>();
        fd = sock->fd();

        if(fd < 0){
            raise_error("valid PHP socket resource expected");
            return false;
        }

        evutil_make_socket_nonblocking(fd);
        return evhttp_accept_socket((evhttp_t *)resource_data->getInternalResourceData(), fd) == 0;
    }

    static bool HHVM_METHOD(EventHttp, addServerAlias, const String &alias) {
        EventHttpResourceData *resource_data = FETCH_RESOURCE(this_, EventHttpResourceData, s_event_http);
        return evhttp_add_server_alias((evhttp_t *)resource_data->getInternalResourceData(), alias.c_str()) == 0;
    }

    static bool HHVM_METHOD(EventHttp, bind, const String &address, int64_t port) {
        EventHttpResourceData *resource_data = FETCH_RESOURCE(this_, EventHttpResourceData, s_event_http);
        return evhttp_bind_socket((evhttp_t *)resource_data->getInternalResourceData(), address.c_str(), port) == 0;
    }

    static bool HHVM_METHOD(EventHttp, removeServerAlias, const String &alias) {
        EventHttpResourceData *resource_data = FETCH_RESOURCE(this_, EventHttpResourceData, s_event_http);
        return evhttp_remove_server_alias((evhttp_t *)resource_data->getInternalResourceData(), alias.c_str()) == 0;
    }

    static void HHVM_METHOD(EventHttp, setAllowedMethods, int64_t methods) {
        EventHttpResourceData *resource_data = FETCH_RESOURCE(this_, EventHttpResourceData, s_event_http);
        evhttp_set_allowed_methods((evhttp_t *)resource_data->getInternalResourceData(), methods);
    }

    static void HHVM_METHOD(EventHttp, setCallback, const String &path, const Object &cb, const Variant &arg) {
        EventHttpResourceData *resource_data = FETCH_RESOURCE(this_, EventHttpResourceData, s_event_http);        ;
        cb.get()->incRefCount();
        resource_data->setCallback(cb.get());
        resource_data->setCallbackArg(arg);
        evhttp_set_cb((evhttp_t *)resource_data->getInternalResourceData(), path.c_str(), event_http_cb, (void *) resource_data);
    }

    static void HHVM_METHOD(EventHttp, setDefaultCallback, const Object &cb, const Variant &arg) {
        EventHttpResourceData *resource_data = FETCH_RESOURCE(this_, EventHttpResourceData, s_event_http);
        cb.get()->incRefCount();
        resource_data->setDefaultCallback(cb.get());
        resource_data->setDefaultCallbackArg(arg);
        evhttp_set_gencb((evhttp_t *)resource_data->getInternalResourceData(), event_http_default_cb, (void *) resource_data);
    }

    static void HHVM_METHOD(EventHttp, setMaxBodySize, int64_t value) {
        EventHttpResourceData *resource_data = FETCH_RESOURCE(this_, EventHttpResourceData, s_event_http);
        evhttp_set_max_body_size((evhttp_t *)resource_data->getInternalResourceData(), value);
    }

    static void HHVM_METHOD(EventHttp, setMaxHeadersSize, int64_t value) {
        EventHttpResourceData *resource_data = FETCH_RESOURCE(this_, EventHttpResourceData, s_event_http);
        evhttp_set_max_headers_size((evhttp_t *)resource_data->getInternalResourceData(), value);
    }

    static void HHVM_METHOD(EventHttp, setTimeout, int64_t value) {
        EventHttpResourceData *resource_data = FETCH_RESOURCE(this_, EventHttpResourceData, s_event_http);
        evhttp_set_timeout((evhttp_t *)resource_data->getInternalResourceData(), value);
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