<?hh
final class EventBuffer
{
    /* Constants */
    const int EOL_ANY = 0 ;
    const int EOL_CRLF = 1 ;
    const int EOL_CRLF_STRICT = 2 ;
    const int EOL_LF = 3 ;
    const int PTR_SET = 0 ;
    const int PTR_ADD = 1 ;

    /* Properties */
    public int $length ;
    public int $contiguous_space ;

    private ?resource $__internal_resource = null;

    <<__Native>> function __construct(): void;
    <<__Native>> function add(string $data): bool;
    <<__Native>> function addBuffer(Eventbuffer $buf): bool;
    <<__Native>> function appendFrom(Eventbuffer $buf, int $len): int;
    <<__Native>> function copyout(mixed &$data, int $max_bytes): int;
    <<__Native>> function drain(int $len): bool;
    <<__Native>> function enableLocking(): void;
    <<__Native>> function expand(int $len): bool;
    <<__Native>> function freeze(bool $at_front): bool;
    <<__Native>> function lock(): void;
    <<__Native>> function prepend(string $data): bool;
    <<__Native>> function prependBuffer(Eventbuffer $buf): bool;
    <<__Native>> function pullup(int $size): string;
    <<__Native>> function read(int $max_bytes): string;
    <<__Native>> function readFrom(resource $fd, int $howmuch): mixed;
    <<__Native>> function readLine(int $eol_style): string;
    <<__Native>> function search(string $what, int $start = -1, int $end = -1): mixed;
    <<__Native>> function searchEol(int $start = -1, int $eol_style = EventBuffer::EOL_ANY): mixed;
    <<__Native>> function substr(int $start, int $length = -1): mixed;
    <<__Native>> function unfreeze(bool $at_front): bool;
    <<__Native>> function unlock(): void;
    <<__Native>> function write(resource $fd, int $howmuch = -1): mixed;
}
