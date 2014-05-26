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
#include <event2/event.h>
#include <event2/util.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
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
                _initEventBufferClass();
                _initEventClass();
                loadSystemlib();
            }
        private:
            void _initEventBaseClass();
            void _initEventConfigClass();
            void _initEventBufferEventClass();
            void _initEventBufferClass();
            void _initEventClass();
    };

    inline Object makeObject(String &ClassName){
        Object ret = ObjectData::newInstance(Unit::lookupClass(ClassName.get()));
        return ret;
    }

    inline Object makeObject(const char *ClassName){
        Object ret = ObjectData::newInstance(Unit::lookupClass(StringData::Make(ClassName)));
        return ret;
    }

}
#endif