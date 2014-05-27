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

    <<__Native>> function __construct(callable $eventcb): void;
    <<__Native>> function __destruct(): void;
}
