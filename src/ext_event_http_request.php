<?hh
<*HAVE_LIBEVENT_HTTP_SUPPORT*>class  EventHttpRequest {
<*HAVE_LIBEVENT_HTTP_SUPPORT*>    const int CMD_GET = 1 ;
<*HAVE_LIBEVENT_HTTP_SUPPORT*>    const int CMD_POST = 2 ;
<*HAVE_LIBEVENT_HTTP_SUPPORT*>    const int CMD_HEAD = 4 ;
<*HAVE_LIBEVENT_HTTP_SUPPORT*>    const int CMD_PUT = 8 ;
<*HAVE_LIBEVENT_HTTP_SUPPORT*>    const int CMD_DELETE = 16 ;
<*HAVE_LIBEVENT_HTTP_SUPPORT*>    const int CMD_OPTIONS = 32 ;
<*HAVE_LIBEVENT_HTTP_SUPPORT*>    const int CMD_TRACE = 64 ;
<*HAVE_LIBEVENT_HTTP_SUPPORT*>    const int CMD_CONNECT = 128 ;
<*HAVE_LIBEVENT_HTTP_SUPPORT*>    const int CMD_PATCH = 256 ;
<*HAVE_LIBEVENT_HTTP_SUPPORT*>    const int INPUT_HEADER = 1 ;
<*HAVE_LIBEVENT_HTTP_SUPPORT*>    const int OUTPUT_HEADER = 2 ;
<*HAVE_LIBEVENT_HTTP_SUPPORT*>    private ?resource $__internal_resource = null;
<*HAVE_LIBEVENT_HTTP_SUPPORT*>    <<__Native>> function __construct(?callable $callback, ?mixed $data = null): void;
<*HAVE_LIBEVENT_HTTP_SUPPORT*>    <<__Native>> function free(): void;
<*HAVE_LIBEVENT_HTTP_SUPPORT*>    <<__Native>> function cancel(): void;
<*HAVE_LIBEVENT_HTTP_SUPPORT*>    <<__Native>> function clearHeaders(): void;
<*HAVE_LIBEVENT_HTTP_SUPPORT*>    <<__Native>> function closeConnection(): void;
<*HAVE_LIBEVENT_HTTP_SUPPORT*>    <<__Native>> function sendReply(int $code, string $reason, ?EventBuffer $buf = null): void;
<*HAVE_LIBEVENT_HTTP_SUPPORT*>    <<__Native>> function findHeader(string $key, int $type): string;
<*HAVE_LIBEVENT_HTTP_SUPPORT*>    <<__Native>> function getEventBufferEvent(): ?EventBufferEvent;
<*HAVE_LIBEVENT_HTTP_SUPPORT*>    <<__Native>> function getCommand(): int;
<*HAVE_LIBEVENT_HTTP_SUPPORT*>}
