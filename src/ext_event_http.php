<?hh
<*HAVE_LIBEVENT_HTTP_SUPPORT*>final class  EventHttp {
<*HAVE_LIBEVENT_HTTP_SUPPORT*>    private ?resource $__internal_resource = null;
<*HAVE_LIBEVENT_HTTP_SUPPORT*>    <<__Native>> function __construct(EventBase $base): void;
<*HAVE_LIBEVENT_HTTP_SUPPORT*>    <<__Native>> function accept(resource $socket): bool;
<*HAVE_LIBEVENT_HTTP_SUPPORT*>    <<__Native>> function addServerAlias(string $alias): bool;
<*HAVE_LIBEVENT_HTTP_SUPPORT*>    <<__Native>> function bind(string $address, int $port): void;
<*HAVE_LIBEVENT_HTTP_SUPPORT*>    <<__Native>> function removeServerAlias(string $alias): bool;
<*HAVE_LIBEVENT_HTTP_SUPPORT*>    <<__Native>> function setAllowedMethods(int $methods): void;
<*HAVE_LIBEVENT_HTTP_SUPPORT*>    <<__Native>> function setCallback(string $path, callable $cb, ?mixed $arg = null): void;
<*HAVE_LIBEVENT_HTTP_SUPPORT*>    <<__Native>> function setDefaultCallback(callable $cb, ?mixed $arg = null): void;
<*HAVE_LIBEVENT_HTTP_SUPPORT*>    <<__Native>> function setMaxBodySize(int $value): void;
<*HAVE_LIBEVENT_HTTP_SUPPORT*>    <<__Native>> function setMaxHeadersSize(int $value): void;
<*HAVE_LIBEVENT_HTTP_SUPPORT*>    <<__Native>> function setTimeout(int $value): void;
<*HAVE_LIBEVENT_HTTP_SUPPORT*>}
