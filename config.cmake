set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake" ${CMAKE_MODULE_PATH})
MESSAGE("${CMAKE_MODULE_PATH}")
find_package(libevent2 REQUIRED)
set(LIBEVENT_LIBRARIES event.so)

HHVM_EXTENSION(event
    src/resource/InternalResource.cpp
    src/resource/EventBaseResource.cpp
    src/resource/EventConfigResource.cpp
    src/ext_event.cpp
    src/ext_event_base.cpp
    src/ext_event_config.cpp
)
HHVM_SYSTEMLIB(event ext_event.php)

target_link_libraries(event ${LIBEVENT_LIBRARIES})
