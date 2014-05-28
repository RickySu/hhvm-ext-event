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

    <<__Native>> function __construct(EventBase $base, mixed $fd, int $what, callable $cb, mixed $arg = NULL): void;
    <<__Native>> function add(float $timeout = -1.0): bool;
    <<__Native>> function del(): bool;
    <<__Native>> static function getSupportedMethods(): array;
    <<__Native>> function free(): void;
}
