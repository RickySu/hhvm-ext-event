<?hh
final class EventConfig
{
    const integer FEATURE_ET = 1;
    const integer FEATURE_O1 = 2;
    const integer FEATURE_FDS = 4;

    private $__internal_resource;

    <<__Native>> function __construct(): void;
    <<__Native>> function __destruct(): void;
    <<__Native>> function avoidMethod(string $method): bool;
    <<__Native>> function requireFeatures(int $feature): bool;
    <<__Native>> function setMaxDispatchInterval(float $max_interval, int $max_callbacks, int $min_priority): void;
}
