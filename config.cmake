find_library(libevent REQUIRED)
set(LIBEVENT_LIBRARIES event.so)

HHVM_EXTENSION(event
    src/resource/InternalResource.cpp
    src/resource/EventBaseResource.cpp
    src/ext_event.cpp
    src/ext_event_base.cpp
)
HHVM_SYSTEMLIB(event ext_event.php)

target_link_libraries(event ${LIBEVENT_LIBRARIES})
