#include "ext.h"

namespace HPHP
{

    static void HHVM_METHOD(EventBuffer, __construct)
    {
        Resource resource = Resource(NEWOBJ(InternalResource(evbuffer_new())));
        SET_RESOURCE(this_, resource, s_eventbuffer);
    }

    void eventExtension::_initEventBufferClass()
    {
        HHVM_ME(EventBuffer, __construct);
    }

}
