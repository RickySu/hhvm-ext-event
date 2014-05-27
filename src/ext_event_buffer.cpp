#include "ext.h"

namespace HPHP
{

    static void HHVM_METHOD(EventBuffer, __construct)
    {
        Resource resource = Resource(NEWOBJ(InternalResource(evbuffer_new())));
        SET_RESOURCE(this_, resource, s_eventbuffer);
    }

    static bool HHVM_METHOD(EventBuffer, add, const String &data) {
        InternalResource *resource = FETCH_RESOURCE(this_, InternalResource, s_eventbuffer);
        return evbuffer_add((event_buffer_t *) resource->getInternalResource(), data.c_str(), data.size()) == 0?true:false;
    }

    static bool HHVM_METHOD(EventBuffer, addBuffer, const Object &buf) {
        InternalResource *resource_dst = FETCH_RESOURCE(this_, InternalResource, s_eventbuffer);
        InternalResource *resource_src = FETCH_RESOURCE(buf, InternalResource, s_eventbuffer);
        return evbuffer_add_buffer((event_buffer_t *) resource_dst->getInternalResource(), (event_buffer_t *) resource_src->getInternalResource()) == 0?true:false;
    }

    static int64_t HHVM_METHOD(EventBuffer, appendFrom, const Object &buf, int64_t len) {
        InternalResource *resource_dst = FETCH_RESOURCE(this_, InternalResource, s_eventbuffer);
        InternalResource *resource_src = FETCH_RESOURCE(buf, InternalResource, s_eventbuffer);
        return evbuffer_remove_buffer((event_buffer_t *) resource_dst->getInternalResource(), (event_buffer_t *) resource_src->getInternalResource(), len);
    }

    static int64_t HHVM_METHOD(EventBuffer, copyout, VRefParam data, int64_t max_bytes) {
        char *tmp_data = new char[max_bytes+1];
        ev_ssize_t read_size;
        InternalResource *resource = FETCH_RESOURCE(this_, InternalResource, s_eventbuffer);
        read_size = evbuffer_copyout((event_buffer_t *) resource->getInternalResource(), tmp_data, max_bytes);
        if(read_size > 0){
            data = StringData::Make(tmp_data, read_size, CopyString);
        }
        delete tmp_data;
        return read_size;
    }

    static bool HHVM_METHOD(EventBuffer, drain, int64_t len) {
        InternalResource *resource = FETCH_RESOURCE(this_, InternalResource, s_eventbuffer);
        return evbuffer_drain((event_buffer_t *) resource->getInternalResource(), len) == 0?true:false;
    }

    static void HHVM_METHOD(EventBuffer, enableLocking) {
        InternalResource *resource = FETCH_RESOURCE(this_, InternalResource, s_eventbuffer);
        evbuffer_enable_locking((event_buffer_t *) resource->getInternalResource(), NULL);
    }

    static bool HHVM_METHOD(EventBuffer, expand, int64_t len) {
        InternalResource *resource = FETCH_RESOURCE(this_, InternalResource, s_eventbuffer);
        return evbuffer_expand((event_buffer_t *) resource->getInternalResource(), len) == 0?true:false;
    }

    static bool HHVM_METHOD(EventBuffer, freeze, bool at_front) {
        InternalResource *resource = FETCH_RESOURCE(this_, InternalResource, s_eventbuffer);
        return evbuffer_freeze((event_buffer_t *) resource->getInternalResource(), (int)at_front) == 0?true:false;
    }

    static void HHVM_METHOD(EventBuffer, lock) {
        InternalResource *resource = FETCH_RESOURCE(this_, InternalResource, s_eventbuffer);
        evbuffer_lock((event_buffer_t *) resource->getInternalResource());
    }

    static bool HHVM_METHOD(EventBuffer, prepend, const String &data) {
        InternalResource *resource = FETCH_RESOURCE(this_, InternalResource, s_eventbuffer);
        return evbuffer_prepend((event_buffer_t *) resource->getInternalResource(), data.c_str(), data.size()) == 0?true:false;
    }

    static bool HHVM_METHOD(EventBuffer, prependBuffer, const Object &buf) {
        InternalResource *resource_dst = FETCH_RESOURCE(this_, InternalResource, s_eventbuffer);
        InternalResource *resource_src = FETCH_RESOURCE(buf, InternalResource, s_eventbuffer);
        return evbuffer_prepend_buffer((event_buffer_t *) resource_dst->getInternalResource(), (event_buffer_t *) resource_src->getInternalResource()) == 0?true:false;
    }

    static String HHVM_METHOD(EventBuffer, pullup, int64_t size) {
        unsigned char *data;
        InternalResource *resource = FETCH_RESOURCE(this_, InternalResource, s_eventbuffer);
        if(size < 0){
            return String();
        }
        data = evbuffer_pullup((event_buffer_t *) resource->getInternalResource(), size);
        if(data == NULL){
            return String();
        }
        return StringData::Make((const char *)data, size, CopyString);
    }

    static String HHVM_METHOD(EventBuffer, read, int64_t max_bytes) {
        char *tmp_data = new char[max_bytes+1];
        int read_size;
        StringData *data;
        InternalResource *resource = FETCH_RESOURCE(this_, InternalResource, s_eventbuffer);
        read_size = evbuffer_remove((event_buffer_t *) resource->getInternalResource(), (void *)tmp_data, max_bytes);
        if(read_size <= 0){
            delete tmp_data;
            return String();
        }
        data = StringData::Make(tmp_data, read_size, CopyString);
        delete tmp_data;
        return data;
    }

    int resource_to_fd(const Resource &fd){
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

    static Variant HHVM_METHOD(EventBuffer, readFrom, const Resource &fd, int64_t howmuch) {
        int fd_r;
        int64_t res;
        InternalResource *resource = FETCH_RESOURCE(this_, InternalResource, s_eventbuffer);
        fd_r = resource_to_fd(fd);

        if(fd_r < 0){
            return false;
        }

        res = evbuffer_read((event_buffer_t *) resource->getInternalResource(), fd_r, howmuch);

        if(res < 0){
            return false;
        }

        return res;
    }

    static String HHVM_METHOD(EventBuffer, readLine, int64_t eol_style) {
        size_t len;
        char *res;

        InternalResource *resource = FETCH_RESOURCE(this_, InternalResource, s_eventbuffer);
        res = evbuffer_readln((event_buffer_t *) resource->getInternalResource(), &len, (evbuffer_eol_style)eol_style);

        if(!res){
            return String();
        }

        return StringData::Make(res, len, CopyString);
    }

    void eventExtension::_initEventBufferClass()
    {
        HHVM_ME(EventBuffer, __construct);
        HHVM_ME(EventBuffer, add);
        HHVM_ME(EventBuffer, addBuffer);
        HHVM_ME(EventBuffer, appendFrom);
        HHVM_ME(EventBuffer, copyout);
        HHVM_ME(EventBuffer, drain);
        HHVM_ME(EventBuffer, enableLocking);
        HHVM_ME(EventBuffer, expand);
        HHVM_ME(EventBuffer, freeze);
        HHVM_ME(EventBuffer, lock);
        HHVM_ME(EventBuffer, prepend);
        HHVM_ME(EventBuffer, prependBuffer);
        HHVM_ME(EventBuffer, pullup);
        HHVM_ME(EventBuffer, read);
        HHVM_ME(EventBuffer, readFrom);
        HHVM_ME(EventBuffer, readLine);
    }

}
