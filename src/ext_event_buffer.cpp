#include "ext.h"

namespace HPHP
{

    static void HHVM_METHOD(EventBuffer, __construct)
    {
        Resource resource = Resource(NEWOBJ(InternalResourceData(evbuffer_new())));
        SET_RESOURCE(this_, resource, s_event_buffer);
    }

    static bool HHVM_METHOD(EventBuffer, add, const String &data) {
        InternalResourceData *resource_data = FETCH_RESOURCE(this_, InternalResourceData, s_event_buffer);
        return evbuffer_add((event_buffer_t *) resource_data->getInternalResourceData(), data.c_str(), data.size()) == 0;
    }

    static bool HHVM_METHOD(EventBuffer, addBuffer, const Object &buf) {
        InternalResourceData *resource_data_dst = FETCH_RESOURCE(this_, InternalResourceData, s_event_buffer);
        InternalResourceData *resource_data_src = FETCH_RESOURCE(buf, InternalResourceData, s_event_buffer);
        return evbuffer_add_buffer((event_buffer_t *) resource_data_dst->getInternalResourceData(), (event_buffer_t *) resource_data_src->getInternalResourceData()) == 0;
    }

    static int64_t HHVM_METHOD(EventBuffer, appendFrom, const Object &buf, int64_t len) {
        InternalResourceData *resource_data_dst = FETCH_RESOURCE(this_, InternalResourceData, s_event_buffer);
        InternalResourceData *resource_data_src = FETCH_RESOURCE(buf, InternalResourceData, s_event_buffer);
        return evbuffer_remove_buffer((event_buffer_t *) resource_data_dst->getInternalResourceData(), (event_buffer_t *) resource_data_src->getInternalResourceData(), len);
    }

    static int64_t HHVM_METHOD(EventBuffer, copyout, VRefParam data, int64_t max_bytes) {
        char *tmp_data = new char[max_bytes+1];
        ev_ssize_t read_size;
        InternalResourceData *resource_data = FETCH_RESOURCE(this_, InternalResourceData, s_event_buffer);
        read_size = evbuffer_copyout((event_buffer_t *) resource_data->getInternalResourceData(), tmp_data, max_bytes);
        if(read_size > 0){
            data = StringData::Make(tmp_data, read_size, CopyString);
        }
        delete tmp_data;
        return read_size;
    }

    static bool HHVM_METHOD(EventBuffer, drain, int64_t len) {
        InternalResourceData *resource_data = FETCH_RESOURCE(this_, InternalResourceData, s_event_buffer);
        return evbuffer_drain((event_buffer_t *) resource_data->getInternalResourceData(), len) == 0;
    }

    static void HHVM_METHOD(EventBuffer, enableLocking) {
        InternalResourceData *resource_data = FETCH_RESOURCE(this_, InternalResourceData, s_event_buffer);
        evbuffer_enable_locking((event_buffer_t *) resource_data->getInternalResourceData(), NULL);
    }

    static bool HHVM_METHOD(EventBuffer, expand, int64_t len) {
        InternalResourceData *resource_data = FETCH_RESOURCE(this_, InternalResourceData, s_event_buffer);
        return evbuffer_expand((event_buffer_t *) resource_data->getInternalResourceData(), len) == 0;
    }

    static bool HHVM_METHOD(EventBuffer, freeze, bool at_front) {
        InternalResourceData *resource_data = FETCH_RESOURCE(this_, InternalResourceData, s_event_buffer);
        return evbuffer_freeze((event_buffer_t *) resource_data->getInternalResourceData(), (int)at_front) == 0;
    }

    static void HHVM_METHOD(EventBuffer, lock) {
        InternalResourceData *resource_data = FETCH_RESOURCE(this_, InternalResourceData, s_event_buffer);
        evbuffer_lock((event_buffer_t *) resource_data->getInternalResourceData());
    }

    static bool HHVM_METHOD(EventBuffer, prepend, const String &data) {
        InternalResourceData *resource_data = FETCH_RESOURCE(this_, InternalResourceData, s_event_buffer);
        return evbuffer_prepend((event_buffer_t *) resource_data->getInternalResourceData(), data.c_str(), data.size()) == 0;
    }

    static bool HHVM_METHOD(EventBuffer, prependBuffer, const Object &buf) {
        InternalResourceData *resource_data_dst = FETCH_RESOURCE(this_, InternalResourceData, s_event_buffer);
        InternalResourceData *resource_data_src = FETCH_RESOURCE(buf, InternalResourceData, s_event_buffer);
        return evbuffer_prepend_buffer((event_buffer_t *) resource_data_dst->getInternalResourceData(), (event_buffer_t *) resource_data_src->getInternalResourceData()) == 0;
    }

    static String HHVM_METHOD(EventBuffer, pullup, int64_t size) {
        unsigned char *data;
        InternalResourceData *resource_data = FETCH_RESOURCE(this_, InternalResourceData, s_event_buffer);
        if(size < 0){
            return String();
        }
        data = evbuffer_pullup((event_buffer_t *) resource_data->getInternalResourceData(), size);
        if(data == NULL){
            return String();
        }
        return StringData::Make((const char *)data, size, CopyString);
    }

    static String HHVM_METHOD(EventBuffer, read, int64_t max_bytes) {
        char *tmp_data = new char[max_bytes+1];
        int read_size;
        StringData *data;
        InternalResourceData *resource_data = FETCH_RESOURCE(this_, InternalResourceData, s_event_buffer);
        read_size = evbuffer_remove((event_buffer_t *) resource_data->getInternalResourceData(), (void *)tmp_data, max_bytes);
        if(read_size <= 0){
            delete tmp_data;
            return String();
        }
        data = StringData::Make(tmp_data, read_size, CopyString);
        delete tmp_data;
        return data;
    }

    static Variant HHVM_METHOD(EventBuffer, readFrom, const Resource &fd, int64_t howmuch) {
        int fd_r;
        int64_t res;
        InternalResourceData *resource_data = FETCH_RESOURCE(this_, InternalResourceData, s_event_buffer);
        fd_r = resource_to_fd(fd);

        if(fd_r < 0){
            return false;
        }

        res = evbuffer_read((event_buffer_t *) resource_data->getInternalResourceData(), fd_r, howmuch);

        if(res < 0){
            return false;
        }

        return res;
    }

    static String HHVM_METHOD(EventBuffer, readLine, int64_t eol_style) {
        size_t len;
        char *res;

        InternalResourceData *resource_data = FETCH_RESOURCE(this_, InternalResourceData, s_event_buffer);
        res = evbuffer_readln((event_buffer_t *) resource_data->getInternalResourceData(), &len, (evbuffer_eol_style) eol_style);

        if(!res){
            return String();
        }

        return StringData::Make(res, len, CopyString);
    }

    static bool get_pos(struct evbuffer_ptr *out_ptr, const long pos, event_buffer_t *buf) {

        if (pos < 0) {
                return false;
        }

        if (evbuffer_ptr_set(buf, out_ptr, pos, EVBUFFER_PTR_SET) == -1) {
                raise_warning("Failed to set position to %ld", pos);
                return false;
        }

        return true;
    }

    static Variant HHVM_METHOD(EventBuffer, search, const String &what, int64_t start, int64_t end) {
        struct evbuffer_ptr ptr_start, ptr_end, ptr_res;
        InternalResourceData *resource_data = FETCH_RESOURCE(this_, InternalResourceData, s_event_buffer);
        event_buffer_t *buf = (event_buffer_t *) resource_data->getInternalResourceData();

        if (start != -1 && get_pos(&ptr_start, start, buf) == false) {
            start = -1;
        }

        if (end != -1 && (end > evbuffer_get_length(buf) || get_pos(&ptr_end, end, buf) == false)) {
            end = -1;
        }

        if (end != -1) {
                ptr_res = evbuffer_search_range(buf, what.c_str(), what.size(), (start != -1 ? &ptr_start : NULL), &ptr_end);
        } else {
                ptr_res = evbuffer_search(buf, what.c_str(), what.size(), (start != -1 ? &ptr_start : NULL));
        }

        if (ptr_res.pos == -1){
            return false;
        }

        return ptr_res.pos;
    }

    static Variant HHVM_METHOD(EventBuffer, searchEol, int64_t start, int64_t eol_style) {
        struct evbuffer_ptr ptr_start, ptr_res;
        InternalResourceData *resource_data = FETCH_RESOURCE(this_, InternalResourceData, s_event_buffer);
        event_buffer_t *buf = (event_buffer_t *) resource_data->getInternalResourceData();
        if (start != -1 && get_pos(&ptr_start, start, buf) == false) {
            start = -1;
        }

        ptr_res = evbuffer_search_eol(buf, (start != -1 ? &ptr_start : NULL), NULL, (evbuffer_eol_style) eol_style);

        if (ptr_res.pos == -1) {
            return false;
        }

        return ptr_res.pos;
    }

    static Variant HHVM_METHOD(EventBuffer, substr, int64_t start, int64_t length) {
        StringData *data;
        struct evbuffer_ptr    ptr;
        typedef struct evbuffer_iovec evbuffer_iovec_t;
        evbuffer_iovec_t *pv;
        int64_t n_chunks, n_read = 0, i;

        InternalResourceData *resource_data = FETCH_RESOURCE(this_, InternalResourceData, s_event_buffer);
        event_buffer_t *buf = (event_buffer_t *) resource_data->getInternalResourceData();

        if (get_pos(&ptr, start, buf) == false) {
            return false;
        }
        /* Determine how many chunks we need */
        n_chunks = evbuffer_peek(buf, length, &ptr, NULL, 0);
        /* Allocate space for the chunks. */
        pv = new evbuffer_iovec_t[n_chunks];
        /* Fill up pv */
        n_chunks = evbuffer_peek(buf, length, &ptr, pv, n_chunks);

        /* Determine the size of the result string */
        for (i = 0; i < n_chunks; ++i) {
                size_t len = pv[i].iov_len;

                if (n_read + len > length) {
                        len = length - n_read;
                }

                n_read += len;
        }

/* Build result string */
        data = StringData::Make(n_read);
        data->setSize(n_read);
        for (n_read = 0, i = 0; i < n_chunks; ++i) {
                size_t len = pv[i].iov_len;
                if (n_read + len > length) {
                        len = length - n_read;
                }
                memcpy((void*) &data->data()[n_read], (void*) pv[i].iov_base, len);
                n_read += len;
        }
        delete pv;
        return data;
    }

    static bool HHVM_METHOD(EventBuffer, unfreeze, bool at_front) {
        InternalResourceData *resource_data = FETCH_RESOURCE(this_, InternalResourceData, s_event_buffer);
        return evbuffer_unfreeze((event_buffer_t *) resource_data->getInternalResourceData(), (int)at_front) == 0;
    }

    static void HHVM_METHOD(EventBuffer, unlock) {
        InternalResourceData *resource_data = FETCH_RESOURCE(this_, InternalResourceData, s_event_buffer);
        evbuffer_unlock((event_buffer_t *) resource_data->getInternalResourceData());
    }

    static Variant HHVM_METHOD(EventBuffer, write, const Resource &fd, int64_t howmuch) {
        int fd_r;
        int64_t res;
        InternalResourceData *resource_data = FETCH_RESOURCE(this_, InternalResourceData, s_event_buffer);
        event_buffer_t *buf = (event_buffer_t *) resource_data->getInternalResourceData();
        fd_r = resource_to_fd(fd);

        if(fd_r < 0){
            return false;
        }

        if (howmuch < 0) {
            res = evbuffer_write(buf, fd_r);
        }
        else{
            res = evbuffer_write_atmost(buf, fd_r, howmuch);
        }

        if(res < 0){
            return false;
        }

        return res;
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
        HHVM_ME(EventBuffer, search);
        HHVM_ME(EventBuffer, searchEol);
        HHVM_ME(EventBuffer, substr);
        HHVM_ME(EventBuffer, unfreeze);
        HHVM_ME(EventBuffer, unlock);
        HHVM_ME(EventBuffer, write);
    }

}
