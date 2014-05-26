include(CheckFunctionExists)
include(CheckIncludeFiles)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake" ${CMAKE_MODULE_PATH})
find_package(libevent2 REQUIRED)

CHECK_FUNCTION_EXISTS(event_config_set_max_dispatch_interval HAVE_DISPATCH_INTERVAL_FUNCTION)
CONFIGURE_FILE(${CMAKE_CURRENT_BINARY_DIR}/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)


CHECK_INCLUDE_FILES (sys/un.h HAVE_SYS_UN_H)
CONFIGURE_FILE(${CMAKE_CURRENT_BINARY_DIR}/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)

set(LIBEVENT_LIBRARIES event.so event_pthreads.so)

HHVM_EXTENSION(event
    src/resource/InternalResource.cpp
    src/resource/EventBufferEventResource.cpp
    src/ext.cpp
    src/ext_event_base.cpp
    src/ext_event_config.cpp
    src/ext_event_buffer_event.cpp
    src/ext_event.cpp
)
HHVM_SYSTEMLIB(event ext_event.php)

target_link_libraries(event ${LIBEVENT_LIBRARIES})
