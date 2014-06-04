#include "ext.h"

#ifdef HAVE_LIBEVENT_HTTP_SUPPORT
namespace HPHP {

    static void event_http_request_cb(evhttp_request_t *request, void *data) {
        //EventResourceData *EResource = (EventResourceData *) data;
        //vm_call_user_func(Object(EResource->getCallback()), make_packed_array(EResource->getArg()));
    }

    static void HHVM_METHOD(EventHttpRequest, __construct, const Object &callback, const Variant &data) {
        evhttp_request_t *http_request;

        Resource resource = Resource(NEWOBJ(EventHttpRequestResourceData()));
        SET_RESOURCE(this_, resource, s_eventhttprequest);
        EventHttpRequestResourceData *resource_data = FETCH_RESOURCE(this_, EventHttpRequestResourceData, s_eventhttprequest);
        //http_request = evhttp_request_new(event_http_request_cb, );

    }

    void eventExtension::_initEventHttpRequestClass() {
        HHVM_ME(EventHttpRequest, __construct);
    }
}
#endif