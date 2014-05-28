<?hh
final class Event
{

    const int ET = 32 ;
    const int PERSIST = 16 ;
    const int READ = 2 ;
    const int WRITE = 4 ;
    const int SIGNAL = 8 ;
    const int TIMEOUT = 1 ;

    private ?resource $__internal_resource = null;

    <<__Native>> function add(float $timeout = -1.0): bool;
    function addSignal(float $timeout = -1.0): bool{
        return $this->add($timeout);
    }
    function addTimer(float $timeout = -1.0): bool{
        return $this->add($timeout);
    }
    <<__Native>> function __construct(EventBase $base, mixed $fd, int $what, callable $cb, ?mixed $arg = NULL): void;
    <<__Native>> function del(): bool;
    function delSignal(): bool{
        return $this->del($timeout);
    }
    function delTimer(): bool{
        return $this->del($timeout);
    }
    <<__Native>> function free(): void;
    <<__Native>> static function getSupportedMethods(): array;
    <<__Native>> function pending(int $flags): bool;
    <<__Native>> function set(EventBase $base, mixed $fd, int $what = -1, ?callable $cb = NULL, mixed $arg = NULL): bool;
    <<__Native>> function setPriority(int $priority): bool;
    function setTimer(EventBase $base, callable $cb, mixed $arg = NULL): bool {
        return $this->set($base, -1, -1, $cb, $arg);
    }
    static function signal(EventBase $base, int $signum, callable $callback, ?mixed $arg = NULL): Event {
        return new self($base, $signum, Event::SIGNAL|Event::PERSIST, $callback, $arg);
    }
    static function timer(EventBase $base, callable $callback, ?mixed $arg = NULL): Event {
        return new self($base, -1, Event::TIMEOUT|Event::PERSIST, $callback, $arg);
    }
}
