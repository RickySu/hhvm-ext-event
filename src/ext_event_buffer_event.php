<?hh
final class EventBufferEvent
{
    const int READING = 1 ;
    const int WRITING = 2 ;
    const int EOF = 16 ;
    const int ERROR = 32 ;
    const int TIMEOUT = 64 ;
    const int CONNECTED = 128 ;
    const int OPT_CLOSE_ON_FREE = 1 ;
    const int OPT_THREADSAFE = 2 ;
    const int OPT_DEFER_CALLBACKS = 4 ;
    const int OPT_UNLOCK_CALLBACKS = 8 ;
    const int SSL_OPEN = 0 ;
    const int SSL_CONNECTING = 1 ;
    const int SSL_ACCEPTING = 2 ;

    private ?resource $__internal_resource = null;

    private ?EventBuffer $input = null;
    private ?EventBuffer $output = null;

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
    <<__Native>> function setWatermark(int $events, int $lowmark, int $highmark):void;
    <<__Native>> function write(string $data):bool;
}
