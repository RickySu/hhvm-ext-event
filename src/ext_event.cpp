#include "ext.h"
#include "hphp/runtime/base/thread-init-fini.h"
namespace HPHP
{

    void event_cb(evutil_socket_t fd_r, short events, void * data) {
        EventResource *EResource = (EventResource *) data;
        vm_call_user_func(Object(EResource->getCallback()), make_packed_array(EResource->getFd(), events, EResource->getArg()));
    }

    void timer_cb(evutil_socket_t fd_r, short events, void * data) {
        EventResource *EResource = (EventResource *) data;
        vm_call_user_func(Object(EResource->getCallback()), make_packed_array(EResource->getArg()));
    }

    void signal_cb(evutil_socket_t signum, short events, void * data) {
        EventResource *EResource = (EventResource *) data;
        vm_call_user_func(Object(EResource->getCallback()), make_packed_array(signum, EResource->getArg()));
    }

    ALWAYS_INLINE void setCB(EventResource *EResource, const Object &callback) {
        EResource->setCallback(callback.get());
        callback.get()->incRefCount();
    }

    ALWAYS_INLINE void freeCB(EventResource *EResource) {
        if(EResource->getCallback() != NULL){
            EResource->getCallback()->decRefCount();
        }
    }

    static void HHVM_METHOD(Event, __construct, const Object &base, const Variant &fd, int64_t what, const Object &cb, const Variant & arg) {
        event_callback_fn event_cb_fn;
        evutil_socket_t fd_r;
        event_t *event;
        Resource resource;
        InternalResource *EBResource = FETCH_RESOURCE(base, InternalResource, s_eventbase);
        event_base_t *event_base = (event_base_t *)EBResource->getInternalResource();

        resource = Resource(NEWOBJ(EventResource(NULL, this_.get())));
        SET_RESOURCE(this_, resource, s_event);
        EventResource *EResource = FETCH_RESOURCE(this_, EventResource, s_event);

        if(what & ~(EV_TIMEOUT | EV_READ | EV_WRITE | EV_SIGNAL | EV_PERSIST | EV_ET)) {
            raise_error("Invalid mask");
        }

        if (what & EV_SIGNAL) {
            fd_r = fd.asInt64Val();
            event_cb_fn = signal_cb;
        }
        else if (what & EV_TIMEOUT) {
            fd_r = -1;
            event_cb_fn = timer_cb;
        }
        else{
            event_cb_fn = event_cb;
            fd_r = resource_to_fd(fd.asCResRef());
            if(fd_r < 0){
                raise_error("valid PHP socket resource expected");
            }
            EResource->setFd(fd.asCResRef());
        }

        EResource->setArg(arg);
        setCB(EResource, cb);
        event = event_new(event_base, fd_r, what, event_cb_fn, (void *)EResource);
        EResource->setInternalResource((void*)event);
    }

    static bool HHVM_METHOD(Event, add, double timeout)
    {
        EventResource *EResource = FETCH_RESOURCE(this_, EventResource, s_event);
        int res;
        struct timeval tv;
        if(timeout < 0){
            res = event_add((event_t *) EResource->getInternalResource(), NULL);
        }
        else {
            TIMEVAL_SET(tv, timeout);
            res = event_add((event_t *) EResource->getInternalResource(), &tv);
        }
        return res == 0;
    }

    static bool HHVM_METHOD(Event, del)
    {
        EventResource *EResource = FETCH_RESOURCE(this_, EventResource, s_event);
        return event_del((event_t *) EResource->getInternalResource()) == 0;
    }

    static void HHVM_METHOD(Event, free)
    {
        EventResource *EResource = FETCH_RESOURCE(this_, EventResource, s_event);
        event_t *event = (event_t *) EResource->getInternalResource();
        if(event == NULL){
            raise_error("Failed to free event resource");
        }
        EResource->setInternalResource(NULL);
        event_free(event);
        freeCB(EResource);
    }

    static bool HHVM_METHOD(Event, pending, int64_t flags)
    {
        EventResource *EResource = FETCH_RESOURCE(this_, EventResource, s_event);
        return event_pending((event_t *) EResource->getInternalResource(), flags, NULL) == 0;
    }

    static Array HHVM_STATIC_METHOD(Event, getSupportedMethods)
    {
        int i;
        const char **methods;
        Array ret;

        methods = event_get_supported_methods();

        if(methods != NULL){
            for (i = 0; methods[i] != NULL; ++i) {
                ret.add(i, String(methods[i]));
            }
        }
        return ret;
    }

    static bool HHVM_METHOD(Event, set, const Object &base, const Variant &fd, int64_t what, const Object &cb, const Variant & arg) {
        short s_what = what;
        EventResource *EResource = FETCH_RESOURCE(this_, EventResource, s_event);
        event_t *event = (event_t *) EResource->getInternalResource();
        evutil_socket_t fd_r;
        event_callback_fn event_cb_fn;
        InternalResource *EBResource = FETCH_RESOURCE(base, InternalResource, s_eventbase);
        event_base_t *event_base = (event_base_t *)EBResource->getInternalResource();

        if(event_is_pending(event)){
            raise_warning("Can't modify pending event");
            return false;
        }

        if(s_what != -1){
            if(s_what & ~(EV_TIMEOUT | EV_READ | EV_WRITE | EV_SIGNAL | EV_PERSIST | EV_ET)) {
                raise_warning("Invalid mask");
                return false;
            }
        }

        event_get_assignment(event, &event_base,
                        (fd_r ? NULL : &fd_r),
                        (short *) (s_what == -1 ? &s_what : NULL),
                        NULL /* ignore old callback */ ,
                        NULL /* ignore old callback argument */);

        if (s_what & EV_SIGNAL) {
            fd_r = fd.asInt64Val();
            event_cb_fn = signal_cb;
        }
        else if (s_what & EV_TIMEOUT) {
            fd_r = -1;
            event_cb_fn = timer_cb;
        }
        else{
            event_cb_fn = event_cb;
            fd_r = resource_to_fd(fd.asCResRef());
            if(fd_r < 0){
                raise_warning("valid PHP socket resource expected");
                return false;
            }
            EResource->setFd(fd.asCResRef());
        }

        if(!arg.isNull()){
            EResource->setArg(arg);
        }

        freeCB(EResource);
        setCB(EResource, cb);

        return event_assign(event, event_base, fd_r, s_what, event_cb_fn, (void *)EResource) == 0;
    }

    static bool HHVM_METHOD(Event, setPriority, int64_t priority){
        EventResource *EResource = FETCH_RESOURCE(this_, EventResource, s_event);
        return event_priority_set((event_t *) EResource->getInternalResource(), priority) == 0;
    }

    void eventExtension::_initEventClass()
    {
        HHVM_ME(Event, __construct);
        HHVM_ME(Event, add);
        HHVM_ME(Event, del);
        HHVM_ME(Event, free);
        HHVM_ME(Event, pending);
        HHVM_ME(Event, set);
        HHVM_ME(Event, setPriority);
        HHVM_STATIC_ME(Event, getSupportedMethods);
    }
}
