#include "ext_event.h"

namespace HPHP
{
    static void HHVM_METHOD(EventConfig, __construct)
    {
    }
    
    void eventExtension::_initEventConfigClass()
    {
        HHVM_ME(EventConfig, __construct);
    }
}
