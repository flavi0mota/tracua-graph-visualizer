cmake_minimum_required(VERSION 3.10)
project(GraphVisualizer)

set(CMAKE_CXX_STANDARD 17)

find_package(raylib REQUIRED)

include_directories(include)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(GraphViz ${SOURCES})

if (UNIX AND NOT APPLE)
    target_link_libraries(GraphViz raylib GL m pthread dl rt X11)
else()
    target_link_libraries(GraphViz raylib)
endif()