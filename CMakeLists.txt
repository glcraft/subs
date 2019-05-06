cmake_minimum_required(VERSION 3.10)

project(Subs)

set(USE_SUBSMATHS ON CACHE BOOL "Compile Subs Maths then use it in the application")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

if (NOT EXISTS "${CMAKE_SOURCE_DIR}/lib/maths")
    set(USE_SUBSMATHS OFF)
endif()

set(SUBS_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")

add_subdirectory(lib)
add_subdirectory(app)

add_dependencies(SubsApp SubsCore)
if (USE_SUBSMATHS)
    add_dependencies(SubsApp SubsMaths)
endif()
