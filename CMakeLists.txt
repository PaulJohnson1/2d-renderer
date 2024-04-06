cmake_minimum_required(VERSION 3.5)
project(2d_renderer)

set(SOURCES
    src/common.c)

if(WASM_BUILD)
    set(SOURCES ${SOURCES} src/web.c)
else()
    set(SOURCES ${SOURCES} src/native.cc)
    include_directories(/usr/include/skia)
endif()

include_directories(include)

add_library(2d_renderer ${SOURCES})

target_link_libraries(2d_renderer PUBLIC skia glfw)

install(TARGETS 2d_renderer
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)
