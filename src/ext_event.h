#ifndef EXT_EVENT_H_
#define EXT_EVENT_H_
#include "../config.h"
#include "hphp/runtime/base/base-includes.h"
#include "resource/EventBaseResource.h"
#include "resource/EventConfigResource.h"
#include "resource/EventBufferEventResource.h"
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
                _initEventBufferEventClass();
                loadSystemlib();
            }
        private:
            void _initEventBaseClass();
            void _initEventConfigClass();
            void _initEventBufferEventClass();
    };
}
#endif