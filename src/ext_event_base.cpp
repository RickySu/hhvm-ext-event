#include "ext_event.h"

namespace HPHP
{
    static void HHVM_METHOD(EventBase, __construct)
    {
    }
    
    void eventExtension::_initEventBaseClass()
    {
        HHVM_ME(EventBase, __construct);
    }
}
