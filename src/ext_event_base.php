<?hh
class EventBase
{
    private $__event_base;
    <<__Native>> function __construct(?EventConfig $cb = NULL): void;
    <<__Native>> function __destruct(): void;
    <<__Native>> function dispatch(): void;

}
