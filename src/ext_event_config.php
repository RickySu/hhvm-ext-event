<?hh
final class EventConfig
{
    const int FEATURE_ET = 1;
    const int FEATURE_O1 = 2;
    const int FEATURE_FDS = 4;

    private ?resource $__internal_resource = null;

    <<__Native>> function __construct(): void;
    <<__Native>> function __destruct(): void;
    <<__Native>> function avoidMethod(string $method): bool;
    <<__Native>> function requireFeatures(int $feature): bool;
    <<__Native>> function setMaxDispatchInterval(float $max_interval, int $max_callbacks, int $min_priority): void;
}
