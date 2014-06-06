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
<*HAVE_LIBEVENT_DNS_SUPPORT*>    <<__Native>> function addNameserverIp(string $ip): bool;
<*HAVE_LIBEVENT_DNS_SUPPORT*>    <<__Native>> function addSearch(string $domain): void;
<*HAVE_LIBEVENT_DNS_SUPPORT*>    <<__Native>> function clearSearch(): void;
<*HAVE_LIBEVENT_DNS_SUPPORT*>    <<__Native>> function countNameservers(): int;
<*HAVE_LIBEVENT_DNS_SUPPORT*>    <<__Native>> function loadHosts(string $hosts): bool;
<*HAVE_LIBEVENT_DNS_SUPPORT*>    <<__Native>> function parseResolvConf(int $flags, string $filename): bool;
<*HAVE_LIBEVENT_DNS_SUPPORT*>    <<__Native>> function setOption(string $option, string $value): bool;
<*HAVE_LIBEVENT_DNS_SUPPORT*>    <<__Native>> function setSearchNdots(int $ndots): void;
<*HAVE_LIBEVENT_DNS_SUPPORT*>}
