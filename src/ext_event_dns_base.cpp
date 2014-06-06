#include "ext.h"

#ifdef HAVE_LIBEVENT_DNS_SUPPORT
namespace HPHP {

    static void HHVM_METHOD(EventDNSBase, __construct, const Object &base, bool initialize) {
        InternalResourceData *event_base_resource_data = FETCH_RESOURCE(base, InternalResourceData, s_event_base);
        Resource resource = Resource(NEWOBJ(InternalResourceData(evdns_base_new((event_base_t *) event_base_resource_data->getInternalResourceData(), initialize))));
        SET_RESOURCE(this_, resource, s_event_dns_base);
    }

    static bool HHVM_METHOD(EventDNSBase, addNameserverIp, const String &ip) {
        InternalResourceData *event_dns_base_resource_data = FETCH_RESOURCE(this_, InternalResourceData, s_event_dns_base);
        return evdns_base_nameserver_ip_add((evdns_base_t *) event_dns_base_resource_data->getInternalResourceData(), ip.c_str()) == 0;
    }

    static void HHVM_METHOD(EventDNSBase, addSearch, const String &domain) {
        InternalResourceData *event_dns_base_resource_data = FETCH_RESOURCE(this_, InternalResourceData, s_event_dns_base);
        evdns_base_search_add((evdns_base_t *) event_dns_base_resource_data->getInternalResourceData(), domain.c_str());
    }

    static void HHVM_METHOD(EventDNSBase, clearSearch) {
        InternalResourceData *event_dns_base_resource_data = FETCH_RESOURCE(this_, InternalResourceData, s_event_dns_base);
        evdns_base_search_clear((evdns_base_t *) event_dns_base_resource_data->getInternalResourceData());
    }

    static int64_t HHVM_METHOD(EventDNSBase, countNameservers) {
        InternalResourceData *event_dns_base_resource_data = FETCH_RESOURCE(this_, InternalResourceData, s_event_dns_base);
        return evdns_base_count_nameservers((evdns_base_t *) event_dns_base_resource_data->getInternalResourceData());
    }

    static bool HHVM_METHOD(EventDNSBase, loadHosts, const String &hosts) {
        InternalResourceData *event_dns_base_resource_data = FETCH_RESOURCE(this_, InternalResourceData, s_event_dns_base);
        return evdns_base_load_hosts((evdns_base_t *) event_dns_base_resource_data->getInternalResourceData(), hosts.c_str()) == 0;
    }

    static bool HHVM_METHOD(EventDNSBase, parseResolvConf, int64_t flags, const String &filename) {
        int ret;
        InternalResourceData *event_dns_base_resource_data = FETCH_RESOURCE(this_, InternalResourceData, s_event_dns_base);
        if(flags & ~(DNS_OPTION_NAMESERVERS | DNS_OPTION_SEARCH | DNS_OPTION_MISC  | DNS_OPTIONS_ALL)) {
            raise_warning("Invalid flags");
            return false;
        }
        ret = evdns_base_resolv_conf_parse((evdns_base_t *) event_dns_base_resource_data->getInternalResourceData(), flags, filename.c_str());
        if(ret){
            switch(ret){
                case 1:
                     raise_warning("Failed to open file");
                     break;
                case 2:
                     raise_warning("Failed to stat file");
                     break;
                case 3:
                     raise_warning("File too large");
                     break;
                case 4:
                     raise_warning("Out of memory");
                     break;
                case 5:
                     raise_warning("Short read from file");
                     break;
                case 6:
                     raise_warning("No nameservers listed in the file");
                     break;
            }
        }
        return true;
    }

    static bool HHVM_METHOD(EventDNSBase, setOption, const String &option, const String &value) {
        InternalResourceData *event_dns_base_resource_data = FETCH_RESOURCE(this_, InternalResourceData, s_event_dns_base);
        return evdns_base_set_option((evdns_base_t *) event_dns_base_resource_data->getInternalResourceData(), option.c_str(), value.c_str()) == 0;
    }

    void eventExtension::_initEventDNSBaseClass() {
        HHVM_ME(EventDNSBase, __construct);
        HHVM_ME(EventDNSBase, addNameserverIp);
        HHVM_ME(EventDNSBase, addSearch);
        HHVM_ME(EventDNSBase, clearSearch);
        HHVM_ME(EventDNSBase, countNameservers);
        HHVM_ME(EventDNSBase, loadHosts);
        HHVM_ME(EventDNSBase, parseResolvConf);
        HHVM_ME(EventDNSBase, setOption);
    }
}
#endif