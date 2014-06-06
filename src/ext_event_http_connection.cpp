#include "ext.h"

#ifdef HAVE_LIBEVENT_HTTP_SUPPORT
namespace HPHP {

    static void HHVM_METHOD(HttpConnection, __construct) {
    }

    void eventExtension::_initEventHttpConnectionClass() {
        HHVM_ME(HttpConnection, __construct);
    }
}
#endif