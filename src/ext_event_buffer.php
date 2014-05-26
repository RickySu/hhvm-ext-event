<?hh
final class EventBuffer
{
    /* Constants */
    const integer EOL_ANY = 0 ;
    const integer EOL_CRLF = 1 ;
    const integer EOL_CRLF_STRICT = 2 ;
    const integer EOL_LF = 3 ;
    const integer PTR_SET = 0 ;
    const integer PTR_ADD = 1 ;

    /* Properties */
    public int $length ;
    public int $contiguous_space ;

    private $__internal_resource;

    <<__Native>> function __construct(): void;
    <<__Native>> function add(string $data): bool;
    <<__Native>> function addBuffer(Eventbuffer $buf): bool;
    <<__Native>> function appendFrom(Eventbuffer $buf, int $len): int;
    <<__Native>> function copyout(mixed &$data, int $max_bytes): int;
}
