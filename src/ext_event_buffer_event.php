<?hh
final class EventBufferEvent
{
    const integer READING = 1 ;
    const integer WRITING = 2 ;
    const integer EOF = 16 ;
    const integer ERROR = 32 ;
    const integer TIMEOUT = 64 ;
    const integer CONNECTED = 128 ;
    const integer OPT_CLOSE_ON_FREE = 1 ;
    const integer OPT_THREADSAFE = 2 ;
    const integer OPT_DEFER_CALLBACKS = 4 ;
    const integer OPT_UNLOCK_CALLBACKS = 8 ;
    const integer SSL_OPEN = 0 ;
    const integer SSL_CONNECTING = 1 ;
    const integer SSL_ACCEPTING = 2 ;

    private $__internal_resource;

    <<__Native>> function __construct(EventBase $base, ?resource $socket = NULL, int $options = 0, callable $readcb = NULL, callable $writecb = NULL, callable $eventcb = NULL, mixed $arg = NULL): void;
    <<__Native>> function connect(string $addr): bool;
    <<__Native>> function enable(int $events): bool;
    <<__Native>> function disable(int $events): bool;
    <<__Native>> function getEnabled(): int;
    <<__Native>> function free(): void;
    <<__Native>> function read(int $size): mixed;
    <<__Native>> function setCallbacks(callable $readcb, callable $writecb, callable $eventcb, mixed $arg = NULL): void;
    <<__Native>> function setPriority(int $priority):bool;
    <<__Native>> function setTimeouts(float $timeout_read, float $timeout_write):bool;
}
