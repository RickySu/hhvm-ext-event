#ifndef EXT_EVENT_H_
#define EXT_EVENT_H_

#include <event2/event.h>
#include <event2/event-config.h>
#include "hphp/runtime/base/base-includes.h"
#include "resource/InternalResource.h"
#include "resource/EventBaseResource.h"
#include "resource/EventConfigResource.h"
#include "util.h"
#include "common.h"

namespace HPHP
{
    class eventExtension: public Extension
    {
        public:
            eventExtension(): Extension("event"){}
            virtual void moduleInit()
            {
                _initEventBaseClass();
                _initEventConfigClass();
                loadSystemlib();
            }
        private:
            void _initEventBaseClass();
            void _initEventConfigClass();
    };
}
#endif