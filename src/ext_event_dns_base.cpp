#include "ext.h"

#ifdef HAVE_LIBEVENT_DNS_SUPPORT
namespace HPHP {
    static void HHVM_METHOD(EventDNSBase, __construct, const Object &base, bool initialize) {
        InternalResourceData *event_base_resource_data = FETCH_RESOURCE(base, InternalResourceData, s_event_base);
        Resource resource = Resource(NEWOBJ(InternalResourceData(evdns_base_new((event_base_t *) event_base_resource_data->getInternalResourceData(), initialize))));
        SET_RESOURCE(this_, resource, s_event_dns_base);
    }

    void eventExtension::_initEventDNSBaseClass() {
        HHVM_ME(EventDNSBase, __construct);
    }
}
#endif