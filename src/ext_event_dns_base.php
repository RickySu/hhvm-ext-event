<?hh
<*HAVE_LIBEVENT_DNS_SUPPORT*>final class EventDNSBase
<*HAVE_LIBEVENT_DNS_SUPPORT*>{
<*HAVE_LIBEVENT_DNS_SUPPORT*>    const int OPTION_SEARCH = 1 ;
<*HAVE_LIBEVENT_DNS_SUPPORT*>    const int OPTION_NAMESERVERS = 2 ;
<*HAVE_LIBEVENT_DNS_SUPPORT*>    const int OPTION_MISC = 4 ;
<*HAVE_LIBEVENT_DNS_SUPPORT*>    const int OPTION_HOSTSFILE = 8 ;
<*HAVE_LIBEVENT_DNS_SUPPORT*>    const int OPTIONS_ALL = 15 ;
<*HAVE_LIBEVENT_DNS_SUPPORT*>    private ?resource $__internal_resource = null;
<*HAVE_LIBEVENT_DNS_SUPPORT*>    <<__Native>> function __construct(EventBase $base, bool $initialize): void;
<*HAVE_LIBEVENT_DNS_SUPPORT*>}
