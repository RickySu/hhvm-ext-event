#ifndef EXT_H_
#define EXT_H_
#include "../config.h"
#include "hphp/runtime/base/base-includes.h"
#include "hphp/runtime/base/socket.h"
#include "hphp/runtime/base/thread-info.h"
#include "hphp/runtime/vm/jit/translator-inline.h"
#include "resource/EventBufferEventResource.h"
#include "util.h"
#include "common.h"
#include <event2/thread.h>


namespace HPHP
{
    class eventExtension: public Extension
    {
        public:
            eventExtension(): Extension("event"){}
            virtual void moduleInit()
            {
                evthread_use_pthreads();
                _initEventBaseClass();
                _initEventConfigClass();
                _initEventBufferEventClass();
                _initEventClass();
                loadSystemlib();
            }
        private:
            void _initEventBaseClass();
            void _initEventConfigClass();
            void _initEventBufferEventClass();
            void _initEventClass();
    };
}
#endif