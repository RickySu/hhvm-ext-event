# - Find LibEvent (a cross event library)
# This module defines
# LIBEVENT_INCLUDE_DIR, where to find LibEvent headers
# LIBEVENT_LIB, LibEvent libraries
# LibEvent_FOUND, If false, do not try to use libevent

set(LibEvent_EXTRA_PREFIXES /usr/local /opt/local "$ENV{HOME}")
foreach(prefix ${LibEvent_EXTRA_PREFIXES})
	list(APPEND LibEvent_INCLUDE_PATHS "${prefix}/include")
	list(APPEND LibEvent_LIB_PATHS "${prefix}/lib")
endforeach()

find_path(LIBEVENT_INCLUDE_DIR event.h PATHS ${LibEvent_INCLUDE_PATHS})
find_library(LIBEVENT_LIB NAMES event PATHS ${LibEvent_LIB_PATHS})

if (LIBEVENT_LIB AND LIBEVENT_INCLUDE_DIR)
  set(LibEvent_FOUND TRUE)
  set(LIBEVENT_LIB ${LIBEVENT_LIB})
else ()
  set(LibEvent_FOUND FALSE)
endif ()

if (LibEvent_FOUND)
  if (NOT LibEvent_FIND_QUIETLY)
    message(STATUS "Found libevent: ${LIBEVENT_LIB}")
  endif ()
else ()
    if (LibEvent_FIND_REQUIRED)
        message(FATAL_ERROR "Could NOT find libevent.")
    endif ()
    message(STATUS "libevent NOT found.")
endif ()

mark_as_advanced(
    LIBEVENT_LIB
    LIBEVENT_INCLUDE_DIR
)

CHECK_FUNCTION_EXISTS(event_config_set_max_dispatch_interval HAVE_DISPATCH_INTERVAL_FUNCTION)
CHECK_INCLUDE_FILES(event2/http.h HAVE_LIBEVENT_HTTP_SUPPORT)
CHECK_INCLUDE_FILES(event2/dns.h HAVE_LIBEVENT_DNS_SUPPORT)
CHECK_INCLUDE_FILES(event2/bufferevent_ssl.h HAVE_LIBEVENT_SSL_SUPPORT)
