cmake_minimum_required(VERSION 3.19.0)

project(qtz)

set(QTZ_LOCAL_BUILD ON)
set(QTZ_LOCAL_INCLUDES_DIR "${CMAKE_CURRENT_BINARY_DIR}/include")

add_subdirectory(core)
add_subdirectory(security)
add_subdirectory(data)
add_subdirectory(widgets)
#add_subdirectory(plugin)
