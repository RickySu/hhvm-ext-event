set(HNI_PHP_FILE ext_event.php)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake" ${CMAKE_MODULE_PATH})

include(CheckFunctionExists)
include(CheckIncludeFiles)
include(FindOpenSSL)

find_package(libevent2 REQUIRED)
set(LIBEVENT_LIBRARIES event.so event_pthreads.so)

CHECK_INCLUDE_FILES (sys/un.h HAVE_SYS_UN_H)

CONFIGURE_FILE(${CMAKE_CURRENT_BINARY_DIR}/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)

include(ConfigureHNIInclude)

HHVM_EXTENSION(event
    src/resource/InternalResource.cpp
    src/resource/EventBufferEventResource.cpp
    src/resource/EventResource.cpp
    src/resource/EventSSLContextResource.cpp
    src/resource/EventHttpResource.cpp
    src/ext.cpp
    src/ext_event_base.cpp
    src/ext_event_config.cpp
    src/ext_event_buffer_event.cpp
    src/ext_event_buffer.cpp
    src/ext_event.cpp
    src/ext_event_util.cpp
    src/ext_event_http.cpp
    src/ext_event_ssl_context
)
HHVM_SYSTEMLIB(event ext_event.php)

target_link_libraries(event ${LIBEVENT_LIBRARIES} ${OPENSSL_LIBRARIES})
