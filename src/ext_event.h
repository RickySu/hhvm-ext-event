#ifndef EXT_EVENT_H_
#define EXT_EVENT_H_

#include <event2/event.h>
#include "hphp/runtime/base/base-includes.h"
#include "resource/InternalResource.h"
#include "resource/EventBaseResource.h"

namespace HPHP
{
    class eventExtension: public Extension
    {
        public:
            eventExtension(): Extension("event"){}
            virtual void moduleInit()
            {
                _initEventBaseClass();
                loadSystemlib();
            }
        private:
            void _initEventBaseClass();
    };
}
#endif