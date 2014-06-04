#include "ext.h"
#ifdef HAVE_LIBEVENT_HTTP_SUPPORT
namespace HPHP {

    static void event_http_request_cb(evhttp_request_t *request, void *data) {
        EventHttpRequestResourceData *resource_data = (EventHttpRequestResourceData *) data;
        evhttp_request_own((evhttp_request_t *) resource_data->getInternalResourceData());
        vm_call_user_func(Object(resource_data->getCallback()), make_packed_array(Object(resource_data->getObjectData()), resource_data->getCallbackArg()));
    }

    static void HHVM_METHOD(EventHttpRequest, __construct, const Object &callback, const Variant &data) {
        evhttp_request_t *http_request;
        Resource resource = Resource(NEWOBJ(EventHttpRequestResourceData(this_.get())));
        SET_RESOURCE(this_, resource, s_eventhttprequest);
        EventHttpRequestResourceData *resource_data = FETCH_RESOURCE(this_, EventHttpRequestResourceData, s_eventhttprequest);
        http_request = evhttp_request_new(event_http_request_cb, (void *) resource_data);
        evhttp_request_own(http_request);
        resource_data->setInternalResourceData((void *) http_request);
        callback.get()->incRefCount();
        resource_data->setCallback(callback.get());
        resource_data->setCallbackArg(data);
    }

    static void HHVM_METHOD(EventHttpRequest, free) {
        EventHttpRequestResourceData *resource_data = FETCH_RESOURCE(this_, EventHttpRequestResourceData, s_eventhttprequest);
        evhttp_request_free((evhttp_request_t *) resource_data->getInternalResourceData());
    }

    static void HHVM_METHOD(EventHttpRequest, cancel) {
        EventHttpRequestResourceData *resource_data = FETCH_RESOURCE(this_, EventHttpRequestResourceData, s_eventhttprequest);
        evhttp_cancel_request((evhttp_request_t *) resource_data->getInternalResourceData());
    }

    static void HHVM_METHOD(EventHttpRequest, clearHeaders) {
        struct evkeyvalq *out_headers;
        EventHttpRequestResourceData *resource_data = FETCH_RESOURCE(this_, EventHttpRequestResourceData, s_eventhttprequest);
        out_headers = evhttp_request_get_output_headers((evhttp_request_t *) resource_data->getInternalResourceData());
        evhttp_clear_headers(out_headers);
    }

    static void HHVM_METHOD(EventHttpRequest, closeConnection) {
        struct evhttp_connection *conn;
        EventHttpRequestResourceData *resource_data = FETCH_RESOURCE(this_, EventHttpRequestResourceData, s_eventhttprequest);
        conn = evhttp_request_get_connection((evhttp_request_t *) resource_data->getInternalResourceData());
        if(conn != NULL){
            evhttp_connection_free(conn);
        }
    }

    static void HHVM_METHOD(EventHttpRequest, sendReply, int64_t code, const String &reason, const Object &buf) {
        EventHttpRequestResourceData *event_http_request_resource_data = FETCH_RESOURCE(this_, EventHttpRequestResourceData, s_eventhttprequest);
        if(buf.isNull()){
            evhttp_send_reply((evhttp_request_t *) event_http_request_resource_data->getInternalResourceData(), code, reason.c_str(), NULL);
            return;
        }
        InternalResourceData *event_buffer_resource_data = FETCH_RESOURCE(buf, InternalResourceData, s_eventbuffer);
        evhttp_send_reply((evhttp_request_t *) event_http_request_resource_data->getInternalResourceData(), code, reason.c_str(), ((event_buffer_t *) event_buffer_resource_data->getInternalResourceData()));
    }

    void eventExtension::_initEventHttpRequestClass() {
        HHVM_ME(EventHttpRequest, __construct);
        HHVM_ME(EventHttpRequest, free);
        HHVM_ME(EventHttpRequest, cancel);
        HHVM_ME(EventHttpRequest, clearHeaders);
        HHVM_ME(EventHttpRequest, closeConnection);
        HHVM_ME(EventHttpRequest, sendReply);
    }
}
#endif