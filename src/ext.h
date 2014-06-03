#ifndef EXT_H_
#define EXT_H_
#include "../config.h"
#include "hphp/runtime/base/base-includes.h"
#include "hphp/runtime/base/socket.h"
#include "hphp/runtime/base/thread-info.h"
#include "hphp/runtime/vm/jit/translator-inline.h"
#include "resource/EventBufferEventResource.h"
#include "resource/EventResource.h"
#include "resource/EventSSLContextResource.h"
#include "h/util.h"
#include "h/common.h"
#include "h/use_event.h"
#include "h/use_openssl.h"

#ifdef HAVE_LIBEVENT_SSL_SUPPORT
    extern int event_ssl_data_index;
#endif

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
                _initEventUtilClass();
#ifdef HAVE_LIBEVENT_SSL_SUPPORT
                _initEventSslContextClass();
#endif
#ifdef HAVE_EVENT_HTTP_SUPPORT
                _initEventHttpClass();
#endif
                loadSystemlib();
            }
        private:
            void _initEventBaseClass();
            void _initEventConfigClass();
            void _initEventBufferEventClass();
            void _initEventBufferClass();
            void _initEventClass();
            void _initEventUtilClass();
#ifdef HAVE_LIBEVENT_SSL_SUPPORT
            void _initEventSslContextClass();
#endif
#ifdef HAVE_EVENT_HTTP_SUPPORT
            void _initEventHttpClass();
#endif
    };

    ALWAYS_INLINE Object makeObject(const String &ClassName, const Array arg){
        Class* cls = Unit::lookupClass(ClassName.get());
        Object ret = ObjectData::newInstance(Unit::lookupClass(ClassName.get()));
        TypedValue dummy;
        g_context->invokeFunc(&dummy, cls->getCtor(), arg, ret.get());
        return ret;
    }

    ALWAYS_INLINE Object makeObject(const String &ClassName){
        return makeObject(ClassName, Array::Create());
    }

    ALWAYS_INLINE Object makeObject(const char *ClassName, const Array arg){
        return makeObject(String(ClassName), arg);
    }

    ALWAYS_INLINE Object makeObject(const char *ClassName){
        return makeObject(String(ClassName), Array::Create());
    }

    ALWAYS_INLINE int resource_to_fd(const Resource &fd){
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