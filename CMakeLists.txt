cmake_minimum_required(VERSION 3.1.0)

project(qtz)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

set(QTZ_LOCAL_BUILD ON)

add_subdirectory(core)
add_subdirectory(security)
add_subdirectory(data)
add_subdirectory(widgets)
add_subdirectory(plugin)