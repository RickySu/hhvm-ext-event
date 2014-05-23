<?hh
final class Event
{

    const integer ET = 32 ;
    const integer PERSIST = 16 ;
    const integer READ = 2 ;
    const integer WRITE = 4 ;
    const integer SIGNAL = 8 ;
    const integer TIMEOUT = 1 ;

    private $__internal_resource;

    <<__Native>> function __construct(object $eventcb): void;
    <<__Native>> function __destruct(): void;
}
