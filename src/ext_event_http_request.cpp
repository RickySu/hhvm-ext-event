#include "ext.h"
#ifdef HAVE_LIBEVENT_HTTP_SUPPORT
namespace HPHP {

    ALWAYS_INLINE evkeyvalq_t *get_http_req_headers(evhttp_request_t *request, int64_t type){
        if (type == EVENT_REQ_HEADER_OUTPUT){
            return evhttp_request_get_output_headers(request);
        }
        return evhttp_request_get_input_headers(request);
    }

    static void event_http_request_cb(evhttp_request_t *request, void *data) {
        EventHttpRequestResourceData *resource_data = (EventHttpRequestResourceData *) data;
        evhttp_request_own((evhttp_request_t *) resource_data->getInternalResourceData());
        vm_call_user_func(Object(resource_data->getCallback()), make_packed_array(Object(resource_data->getObjectData()), resource_data->getCallbackArg()));
    }

    static void HHVM_METHOD(EventHttpRequest, __construct, const Object &callback, const Variant &data) {
        evhttp_request_t *http_request;
        Resource resource = Resource(NEWOBJ(EventHttpRequestResourceData(this_.get())));
        SET_RESOURCE(this_, resource, s_event_http_request);
        EventHttpRequestResourceData *resource_data = FETCH_RESOURCE(this_, EventHttpRequestResourceData, s_event_http_request);
        http_request = evhttp_request_new(event_http_request_cb, (void *) resource_data);
        evhttp_request_own(http_request);
        resource_data->setInternalResourceData((void *) http_request);
        callback.get()->incRefCount();
        resource_data->setCallback(callback.get());
        resource_data->setCallbackArg(data);
    }

    static void HHVM_METHOD(EventHttpRequest, free) {
        EventHttpRequestResourceData *resource_data = FETCH_RESOURCE(this_, EventHttpRequestResourceData, s_event_http_request);

        if(resource_data->isInternal){
            return;
        }

        evhttp_request_free((evhttp_request_t *) resource_data->getInternalResourceData());
    }

    static void HHVM_METHOD(EventHttpRequest, cancel) {
        EventHttpRequestResourceData *resource_data = FETCH_RESOURCE(this_, EventHttpRequestResourceData, s_event_http_request);
        evhttp_cancel_request((evhttp_request_t *) resource_data->getInternalResourceData());
    }

    static void HHVM_METHOD(EventHttpRequest, clearHeaders) {
        evkeyvalq_t *out_headers;
        EventHttpRequestResourceData *resource_data = FETCH_RESOURCE(this_, EventHttpRequestResourceData, s_event_http_request);
        out_headers = evhttp_request_get_output_headers((evhttp_request_t *) resource_data->getInternalResourceData());
        evhttp_clear_headers(out_headers);
    }

    static void HHVM_METHOD(EventHttpRequest, closeConnection) {
        struct evhttp_connection *conn;
        EventHttpRequestResourceData *resource_data = FETCH_RESOURCE(this_, EventHttpRequestResourceData, s_event_http_request);
        conn = evhttp_request_get_connection((evhttp_request_t *) resource_data->getInternalResourceData());
        if(conn != NULL){
            evhttp_connection_free(conn);
        }
    }

    static void HHVM_METHOD(EventHttpRequest, sendReply, int64_t code, const String &reason, const Object &buf) {
        EventHttpRequestResourceData *event_http_request_resource_data = FETCH_RESOURCE(this_, EventHttpRequestResourceData, s_event_http_request);
        if(buf.isNull()){
            evhttp_send_reply((evhttp_request_t *) event_http_request_resource_data->getInternalResourceData(), code, reason.c_str(), NULL);
            return;
        }
        InternalResourceData *event_buffer_resource_data = FETCH_RESOURCE(buf, InternalResourceData, s_event_buffer);
        evhttp_send_reply((evhttp_request_t *) event_http_request_resource_data->getInternalResourceData(), code, reason.c_str(), (event_buffer_t *) event_buffer_resource_data->getInternalResourceData());
    }

    static String HHVM_METHOD(EventHttpRequest, findHeader, const String &key, int64_t type) {
        evkeyvalq_t *headers;
        EventHttpRequestResourceData *event_http_request_resource_data = FETCH_RESOURCE(this_, EventHttpRequestResourceData, s_event_http_request);

        if (type & ~(EVENT_REQ_HEADER_INPUT | EVENT_REQ_HEADER_OUTPUT)) {
            return false;
        }

        headers = get_http_req_headers((evhttp_request_t *) event_http_request_resource_data->getInternalResourceData(), type);
        const char *val = evhttp_find_header(headers, key.c_str());

        if(val == NULL){
            return String();
        }

        return StringData::Make(val, CopyString);
    }

    static Variant HHVM_METHOD(EventHttpRequest, getEventBufferEvent) {
        evhttp_connection_t *conn;
        event_buffer_event_t *bevent;
        EventHttpRequestResourceData *event_http_request_resource_data = FETCH_RESOURCE(this_, EventHttpRequestResourceData, s_event_http_request);

        if((conn = evhttp_request_get_connection((evhttp_request_t *) event_http_request_resource_data->getInternalResourceData())) == NULL){
            return Variant();
        }

        if((bevent = evhttp_connection_get_bufferevent(conn)) == NULL){
            return Variant();
        }

        String ClassName("EventBufferEvent");
        Object event_buffer_event = ObjectData::newInstance(Unit::lookupClass(ClassName.get()));
        Resource resource;
        resource = Resource(NEWOBJ(EventBufferEventResourceData(bevent, event_buffer_event.get())));
        SET_RESOURCE(event_buffer_event, resource, s_event_bufferevent);
        EventBufferEventResourceData *event_buffer_event_resource_data = FETCH_RESOURCE(event_buffer_event, EventBufferEventResourceData, s_event_bufferevent);
        event_buffer_event_resource_data->isInternal = true;
        return event_buffer_event;
    }

    static int64_t HHVM_METHOD(EventHttpRequest, getCommand) {
        EventHttpRequestResourceData *event_http_request_resource_data = FETCH_RESOURCE(this_, EventHttpRequestResourceData, s_event_http_request);
        return evhttp_request_get_command((evhttp_request_t *) event_http_request_resource_data->getInternalResourceData());
    }

    static String HHVM_METHOD(EventHttpRequest, getHost) {
        EventHttpRequestResourceData *event_http_request_resource_data = FETCH_RESOURCE(this_, EventHttpRequestResourceData, s_event_http_request);
        return StringData::Make(evhttp_request_get_host((evhttp_request_t *) event_http_request_resource_data->getInternalResourceData()), CopyString);
    }

    static Variant HHVM_METHOD(EventHttpRequest, getInputBuffer) {
        event_buffer_t *buf;
        EventHttpRequestResourceData *event_http_request_resource_data = FETCH_RESOURCE(this_, EventHttpRequestResourceData, s_event_http_request);

        if((buf = evhttp_request_get_input_buffer((evhttp_request_t *) event_http_request_resource_data->getInternalResourceData())) == NULL){
            return Variant();
        }

        Object event_buffer = ObjectData::newInstance(Unit::lookupClass(String("EventBuffer").get()));
        Resource resource = Resource(NEWOBJ(InternalResourceData(buf)));
        SET_RESOURCE(event_buffer, resource, s_event_buffer);
        InternalResourceData *resource_data = FETCH_RESOURCE(event_buffer, InternalResourceData, s_event_buffer);
        resource_data->isInternal = true;
        return event_buffer;
    }

    static Array HHVM_METHOD(EventHttpRequest, getInputHeaders) {
        EventHttpRequestResourceData *event_http_request_resource_data = FETCH_RESOURCE(this_, EventHttpRequestResourceData, s_event_http_request);
        evkeyvalq_t *headers;
        evkeyval_t  *header;

        Array ret = Array::Create();

        if((headers = evhttp_request_get_input_headers((evhttp_request_t *) event_http_request_resource_data->getInternalResourceData())) == NULL){
            return ret;
        }

        header = headers->tqh_first;

        while(header){
            ret.set(String(header->key, CopyString), String(header->value, CopyString));
            header = header->next.tqe_next;
        }
        return ret;
    }

    static Variant HHVM_METHOD(EventHttpRequest, getOutputBuffer) {
        event_buffer_t *buf;
        EventHttpRequestResourceData *event_http_request_resource_data = FETCH_RESOURCE(this_, EventHttpRequestResourceData, s_event_http_request);

        if((buf = evhttp_request_get_output_buffer((evhttp_request_t *) event_http_request_resource_data->getInternalResourceData())) == NULL){
            return Variant();
        }

        Object event_buffer = ObjectData::newInstance(Unit::lookupClass(String("EventBuffer").get()));
        Resource resource = Resource(NEWOBJ(InternalResourceData(buf)));
        SET_RESOURCE(event_buffer, resource, s_event_buffer);
        InternalResourceData *resource_data = FETCH_RESOURCE(event_buffer, InternalResourceData, s_event_buffer);
        resource_data->isInternal = true;
        return event_buffer;
    }

    static Array HHVM_METHOD(EventHttpRequest, getOutputHeaders) {
        EventHttpRequestResourceData *event_http_request_resource_data = FETCH_RESOURCE(this_, EventHttpRequestResourceData, s_event_http_request);
        evkeyvalq_t *headers;
        evkeyval_t  *header;

        Array ret = Array::Create();

        if((headers = evhttp_request_get_output_headers((evhttp_request_t *) event_http_request_resource_data->getInternalResourceData())) == NULL){
            return ret;
        }

        header = headers->tqh_first;

        while(header){
            ret.set(String(header->key, CopyString), String(header->value, CopyString));
            header = header->next.tqe_next;
        }
        return ret;
    }

    static int64_t HHVM_METHOD(EventHttpRequest, getResponseCode) {
        EventHttpRequestResourceData *event_http_request_resource_data = FETCH_RESOURCE(this_, EventHttpRequestResourceData, s_event_http_request);
        return evhttp_request_get_response_code((evhttp_request_t *) event_http_request_resource_data->getInternalResourceData());
    }

    static String HHVM_METHOD(EventHttpRequest, getUri) {
        EventHttpRequestResourceData *event_http_request_resource_data = FETCH_RESOURCE(this_, EventHttpRequestResourceData, s_event_http_request);
        return StringData::Make(evhttp_request_get_uri((evhttp_request_t *) event_http_request_resource_data->getInternalResourceData()), CopyString);
    }

    static bool HHVM_METHOD(EventHttpRequest, removeHeader, const String &key, int64_t type) {
        evkeyvalq_t *headers;
        EventHttpRequestResourceData *event_http_request_resource_data = FETCH_RESOURCE(this_, EventHttpRequestResourceData, s_event_http_request);
        headers = get_http_req_headers((evhttp_request_t *) event_http_request_resource_data->getInternalResourceData(), type);
        return evhttp_remove_header(headers, key.c_str()) == 0;
    }

    static void HHVM_METHOD(EventHttpRequest, sendError, int64_t error, const String &reason) {
        EventHttpRequestResourceData *event_http_request_resource_data = FETCH_RESOURCE(this_, EventHttpRequestResourceData, s_event_http_request);
        evhttp_send_error((evhttp_request_t *) event_http_request_resource_data->getInternalResourceData(), error, reason.isNull()?NULL:reason.c_str());
    }

    static void HHVM_METHOD(EventHttpRequest, sendReplyStart, int64_t code, const String &reason) {
        EventHttpRequestResourceData *event_http_request_resource_data = FETCH_RESOURCE(this_, EventHttpRequestResourceData, s_event_http_request);
        evhttp_send_reply_start((evhttp_request_t *) event_http_request_resource_data->getInternalResourceData(), code, reason.c_str());
    }

    static void HHVM_METHOD(EventHttpRequest, sendReplyChunk, const Object &buf) {
        EventHttpRequestResourceData *event_http_request_resource_data = FETCH_RESOURCE(this_, EventHttpRequestResourceData, s_event_http_request);
        InternalResourceData *event_buffer_resource_data = FETCH_RESOURCE(buf, InternalResourceData, s_event_buffer);
        evhttp_send_reply_chunk((evhttp_request_t *) event_http_request_resource_data->getInternalResourceData(), (event_buffer_t *) event_buffer_resource_data->getInternalResourceData());
    }

    static void HHVM_METHOD(EventHttpRequest, sendReplyEnd) {
        EventHttpRequestResourceData *event_http_request_resource_data = FETCH_RESOURCE(this_, EventHttpRequestResourceData, s_event_http_request);
        evhttp_send_reply_end((evhttp_request_t *) event_http_request_resource_data->getInternalResourceData());
    }

    static String HHVM_METHOD(EventHttpRequest, addHeader, const String &key, const String &value, int64_t type) {
        evkeyvalq_t *headers;
        EventHttpRequestResourceData *event_http_request_resource_data = FETCH_RESOURCE(this_, EventHttpRequestResourceData, s_event_http_request);
        headers = get_http_req_headers((evhttp_request_t *) event_http_request_resource_data->getInternalResourceData(), type);
        return evhttp_add_header(headers, key.c_str(), value.c_str());
    }

    void eventExtension::_initEventHttpRequestClass() {
        HHVM_ME(EventHttpRequest, __construct);
        HHVM_ME(EventHttpRequest, free);
        HHVM_ME(EventHttpRequest, cancel);
        HHVM_ME(EventHttpRequest, clearHeaders);
        HHVM_ME(EventHttpRequest, closeConnection);
        HHVM_ME(EventHttpRequest, sendReply);
        HHVM_ME(EventHttpRequest, findHeader);
        HHVM_ME(EventHttpRequest, getEventBufferEvent);
        HHVM_ME(EventHttpRequest, getCommand);
        HHVM_ME(EventHttpRequest, getHost);
        HHVM_ME(EventHttpRequest, getInputBuffer);
        HHVM_ME(EventHttpRequest, getInputHeaders);
        HHVM_ME(EventHttpRequest, getOutputBuffer);
        HHVM_ME(EventHttpRequest, getOutputHeaders);
        HHVM_ME(EventHttpRequest, getResponseCode);
        HHVM_ME(EventHttpRequest, getUri);
        HHVM_ME(EventHttpRequest, removeHeader);
        HHVM_ME(EventHttpRequest, sendError);
        HHVM_ME(EventHttpRequest, sendReplyStart);
        HHVM_ME(EventHttpRequest, sendReplyChunk);
        HHVM_ME(EventHttpRequest, sendReplyEnd);
        HHVM_ME(EventHttpRequest, addHeader);
    }
}
#endif