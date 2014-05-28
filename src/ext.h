#ifndef EXT_H_
#define EXT_H_
#include "../config.h"
#include "hphp/runtime/base/base-includes.h"
#include "hphp/runtime/base/socket.h"
#include "hphp/runtime/base/thread-info.h"
#include "hphp/runtime/vm/jit/translator-inline.h"
#include "resource/EventBufferEventResource.h"
#include "resource/EventResource.h"
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

    inline Object makeObject(const String &ClassName, const Array arg){
        Class* cls = Unit::lookupClass(ClassName.get());
        Object ret = ObjectData::newInstance(Unit::lookupClass(ClassName.get()));
        TypedValue dummy;
        g_context->invokeFunc(&dummy, cls->getCtor(), arg, ret.get());
        return ret;
    }

    inline Object makeObject(const String &ClassName){
        return makeObject(ClassName, Array::Create());
    }

    inline Object makeObject(const char *ClassName, const Array arg){
        return makeObject(String(ClassName), arg);
    }

    inline Object makeObject(const char *ClassName){
        return makeObject(String(ClassName), Array::Create());
    }

    inline int resource_to_fd(const Resource &fd){
        File *file = fd.getTyped<File>();
        if(file->valid()){
            return file->fd();
        }
        Socket *sock = fd.getTyped<Socket>();
        if(sock->valid()){
            return sock->fd();
        }
        return -1;
    }

}
#endif