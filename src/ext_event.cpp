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

    inline void setCB(EventResource *EResource, const Object &callback) {
        EResource->setCallback(callback.get());
        callback.get()->incRefCount();
    }

    inline void freeCB(EventResource *EResource) {
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

        if(what & ~(EV_TIMEOUT | EV_READ | EV_WRITE | EV_SIGNAL | EV_PERSIST | EV_ET)) {
            raise_error("Invalid mask");
        }

        resource = Resource(NEWOBJ(EventResource(NULL, this_.get())));
        SET_RESOURCE(this_, resource, s_event);
        EventResource *EResource = FETCH_RESOURCE(this_, EventResource, s_event);

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

    void eventExtension::_initEventClass()
    {
        HHVM_ME(Event, __construct);
        HHVM_ME(Event, free);
    }
}
