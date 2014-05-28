<?hh
final class  EventUtil
{
    /* Constants */
    const int AF_INET = 2 ;
    const int AF_INET6 = 10 ;
    const int AF_UNSPEC = 0 ;
    const int LIBEVENT_VERSION_NUMBER = 33559808 ;
    const int SO_DEBUG = 1 ;
    const int SO_REUSEADDR = 2 ;
    const int SO_KEEPALIVE = 9 ;
    const int SO_DONTROUTE = 5 ;
    const int SO_LINGER = 13 ;
    const int SO_BROADCAST = 6 ;
    const int SO_OOBINLINE = 10 ;
    const int SO_SNDBUF = 7 ;
    const int SO_RCVBUF = 8 ;
    const int SO_SNDLOWAT = 19 ;
    const int SO_RCVLOWAT = 18 ;
    const int SO_SNDTIMEO = 21 ;
    const int SO_RCVTIMEO = 20 ;
    const int SO_TYPE = 3 ;
    const int SO_ERROR = 4 ;
    const int SOL_SOCKET = 1 ;
    const int SOL_TCP = 6 ;
    const int SOL_UDP = 17 ;
    const int IPPROTO_IP = 0 ;
    const int IPPROTO_IPV6 = 41 ;

    private function __construct(): void {}
    <<__Native>> static function getLastSocketErrno(?resource $socket = NULL): int;
    <<__Native>> static function getLastSocketError(?resource $socket = NULL): string;
    <<__Native>> static function getSocketFd(resource $socket): int;
    <<__Native>> static function getSocketName(resource $socket, string &$address, ?mixed &$port = NULL): bool;
}
