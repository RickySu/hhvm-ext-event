#include "ext.h"
#ifdef HAVE_SYS_UN_H
    #include <sys/un.h>
#endif

namespace HPHP
{

    static int64_t HHVM_STATIC_METHOD(EventUtil, getLastSocketErrno, const Resource &socket){
        if(socket.isNull()){
            return EVUTIL_SOCKET_ERROR();
        }
        return evutil_socket_geterror(resource_to_fd(socket.asCResRef()));
    }

    static String HHVM_STATIC_METHOD(EventUtil, getLastSocketError, const Resource &socket){
        if(socket.isNull()){
            return StringData::Make(evutil_socket_error_to_string(EVUTIL_SOCKET_ERROR()));
        }
        return StringData::Make(evutil_socket_geterror(resource_to_fd(socket.asCResRef())));
    }

    static int64_t HHVM_STATIC_METHOD(EventUtil, getSocketFd, const Resource &socket){
        return resource_to_fd(socket);
    }

    static bool HHVM_STATIC_METHOD(EventUtil, getSocketName, const Resource &socket, VRefParam address, VRefParam port){
        evutil_socket_t fd = resource_to_fd(socket);
        struct sockaddr sa;
        socklen_t sa_len = sizeof(struct sockaddr);
        long l_port = 0;

        if (getsockname(fd, &sa, &sa_len)) {
            raise_warning("Unable to retreive socket name.");
            return false;
        }

        switch (sa.sa_family) {
            case AF_INET:
            {
                struct sockaddr_in *sa_in = (struct sockaddr_in *) &sa;
                char c_addr[INET_ADDRSTRLEN + 1];
                if (evutil_inet_ntop(sa.sa_family, &sa_in->sin_addr, c_addr, sizeof(c_addr))) {
                    address = StringData::Make(c_addr, CopyString);
                    l_port = ntohs(sa_in->sin_port);
                }
            }
            break;
#ifdef AF_INET6
            case AF_INET6:
            {
                struct sockaddr_in6 *sa_in6 = (struct sockaddr_in6 *) &sa;
                char c_addr6[INET6_ADDRSTRLEN + 1];
                if (evutil_inet_ntop(sa.sa_family, &sa_in6->sin6_addr, c_addr6, sizeof(c_addr6))) {
                    address = StringData::Make(c_addr6, CopyString);
                    l_port = ntohs(sa_in6->sin6_port);
                }
            }
            break;
#endif
#ifdef HAVE_SYS_UN_H
            case AF_UNIX:
            {
                struct sockaddr_un *ua = (struct sockaddr_un *) &sa;
                address = StringData::Make(ua->sun_path, CopyString);
            }
            break;
#endif
            default:
                raise_warning("Unsupported address family.");
                return false;
        }
        if(port.isReferenced()){
            port = l_port;
        }
        return true;
    }

    void eventExtension::_initEventUtilClass()
    {
        HHVM_STATIC_ME(EventUtil, getLastSocketErrno);
        HHVM_STATIC_ME(EventUtil, getLastSocketError);
        HHVM_STATIC_ME(EventUtil, getSocketFd);
        HHVM_STATIC_ME(EventUtil, getSocketName);
    }

}
