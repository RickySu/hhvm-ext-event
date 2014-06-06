#include "ext.h"
#include "hphp/runtime/base/thread-init-fini.h"
namespace HPHP
{

    void event_cb(evutil_socket_t fd_r, short events, void * data) {
        EventResourceData *event_resource_data = (EventResourceData *) data;
        vm_call_user_func(Object(event_resource_data->getCallback()), make_packed_array(event_resource_data->getFd(), events, event_resource_data->getArg()));
    }

    void timer_cb(evutil_socket_t fd_r, short events, void * data) {
        EventResourceData *event_resource_data = (EventResourceData *) data;
        vm_call_user_func(Object(event_resource_data->getCallback()), make_packed_array(event_resource_data->getArg()));
    }

    void signal_cb(evutil_socket_t signum, short events, void * data) {
        EventResourceData *event_resource_data = (EventResourceData *) data;
        vm_call_user_func(Object(event_resource_data->getCallback()), make_packed_array(signum, event_resource_data->getArg()));
    }

    ALWAYS_INLINE void setCB(EventResourceData *event_resource_data, const Object &callback) {
        event_resource_data->setCallback(callback.get());
        callback.get()->incRefCount();
    }

    ALWAYS_INLINE void freeCB(EventResourceData *event_resource_data) {
        if(event_resource_data->getCallback() != NULL){
            event_resource_data->getCallback()->decRefCount();
        }
    }

    static void HHVM_METHOD(Event, __construct, const Object &base, const Variant &fd, int64_t what, const Object &cb, const Variant & arg) {
        event_callback_fn event_cb_fn;
        evutil_socket_t fd_r;
        event_t *event;
        Resource resource;
        InternalResourceData *event_base_resource_data = FETCH_RESOURCE(base, InternalResourceData, s_event_base);
        event_base_t *event_base = (event_base_t *)event_base_resource_data->getInternalResourceData();

        resource = Resource(NEWOBJ(EventResourceData(NULL, this_.get())));
        SET_RESOURCE(this_, resource, s_event);
        EventResourceData *event_resource_data = FETCH_RESOURCE(this_, EventResourceData, s_event);

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
            event_resource_data->setFd(fd.asCResRef());
        }

        event_resource_data->setArg(arg);
        setCB(event_resource_data, cb);
        event = event_new(event_base, fd_r, what, event_cb_fn, (void *)event_resource_data);
        event_resource_data->setInternalResourceData((void*)event);
    }

    static bool HHVM_METHOD(Event, add, double timeout)
    {
        EventResourceData *event_resource_data = FETCH_RESOURCE(this_, EventResourceData, s_event);
        int res;
        struct timeval tv;
        if(timeout < 0){
            res = event_add((event_t *) event_resource_data->getInternalResourceData(), NULL);
        }
        else {
            TIMEVAL_SET(tv, timeout);
            res = event_add((event_t *) event_resource_data->getInternalResourceData(), &tv);
        }
        return res == 0;
    }

    static bool HHVM_METHOD(Event, del)
    {
        EventResourceData *event_resource_data = FETCH_RESOURCE(this_, EventResourceData, s_event);
        return event_del((event_t *) event_resource_data->getInternalResourceData()) == 0;
    }

    static void HHVM_METHOD(Event, free)
    {
        EventResourceData *event_resource_data = FETCH_RESOURCE(this_, EventResourceData, s_event);
        event_t *event = (event_t *) event_resource_data->getInternalResourceData();
        if(event == NULL){
            raise_error("Failed to free event resource");
        }
        event_resource_data->setInternalResourceData(NULL);
        event_free(event);
        freeCB(event_resource_data);
    }

    static bool HHVM_METHOD(Event, pending, int64_t flags)
    {
        EventResourceData *event_resource_data = FETCH_RESOURCE(this_, EventResourceData, s_event);
        return event_pending((event_t *) event_resource_data->getInternalResourceData(), flags, NULL) == 0;
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
        EventResourceData *event_resource_data = FETCH_RESOURCE(this_, EventResourceData, s_event);
        event_t *event = (event_t *) event_resource_data->getInternalResourceData();
        evutil_socket_t fd_r;
        event_callback_fn event_cb_fn;
        InternalResourceData *event_base_resource_data = FETCH_RESOURCE(base, InternalResourceData, s_event_base);
        event_base_t *event_base = (event_base_t *)event_base_resource_data->getInternalResourceData();

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
            event_resource_data->setFd(fd.asCResRef());
        }

        event_resource_data->setArg(arg);

        freeCB(event_resource_data);
        setCB(event_resource_data, cb);

        return event_assign(event, event_base, fd_r, s_what, event_cb_fn, (void *)event_resource_data) == 0;
    }

    static bool HHVM_METHOD(Event, setPriority, int64_t priority){
        EventResourceData *event_resource_data = FETCH_RESOURCE(this_, EventResourceData, s_event);
        return event_priority_set((event_t *) event_resource_data->getInternalResourceData(), priority) == 0;
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
