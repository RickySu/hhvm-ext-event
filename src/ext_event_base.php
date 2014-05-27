<?hh
final class EventBase
{
    const int LOOP_ONCE = 1 ;
    const int LOOP_NONBLOCK = 2 ;
    const int NOLOCK = 1 ;
    const int STARTUP_IOCP = 4 ;
    const int NO_CACHE_TIME = 8 ;
    const int EPOLL_USE_CHANGELIST = 16 ;

    private ?resource $__internal_resource = null;

    <<__Native>> function __construct(?EventConfig $config = NULL): void;
    <<__Native>> function __destruct(): void;
    <<__Native>> function dispatch(): void;
    <<__Native>> function loopexit(float $timeout = 0.0): bool;
    <<__Native>> function getFeatures(): int;
    <<__Native>> function getMethod(?EventConfig $cb = NULL): string;
    <<__Native>> function getTimeOfDayCached(): float;
    <<__Native>> function gotExit(): bool;
    <<__Native>> function gotStop(): bool;
    <<__Native>> function loop(int $flags = 0): bool;
    <<__Native>> function priorityInit(int $n_priorities): bool;
    <<__Native>> function reInit(): bool;
    <<__Native>> function stop(): bool;
}
