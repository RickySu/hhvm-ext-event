<?hh
final class EventSslContext
{
    /* Constants */
    const int SSLv2_CLIENT_METHOD = 1 ;
    const int SSLv3_CLIENT_METHOD = 2 ;
    const int SSLv23_CLIENT_METHOD = 3 ;
    const int TLS_CLIENT_METHOD = 4 ;
    const int SSLv2_SERVER_METHOD = 5 ;
    const int SSLv3_SERVER_METHOD = 6 ;
    const int SSLv23_SERVER_METHOD = 7 ;
    const int TLS_SERVER_METHOD = 8 ;
    const int OPT_LOCAL_CERT = 1 ;
    const int OPT_LOCAL_PK = 2 ;
    const int OPT_PASSPHRASE = 3 ;
    const int OPT_CA_FILE = 4 ;
    const int OPT_CA_PATH = 5 ;
    const int OPT_ALLOW_SELF_SIGNED = 6 ;
    const int OPT_VERIFY_PEER = 7 ;
    const int OPT_VERIFY_DEPTH = 8 ;
    const int OPT_CIPHERS = 9 ;

    /* Properties */
    public ?string $local_cert = NULL;
    public ?string $local_pk = NULL;

    /* Methods */
    <*HAVE_LIBEVENT_SSL_SUPPORT*><<__Native>> public function __construct(int $method, array $options): void;
}
