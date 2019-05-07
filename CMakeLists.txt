cmake_minimum_required(VERSION 3.10)

project(Subs VERSION 1.1.0)

set(USE_SUBS_EXPRTK OFF CACHE BOOL "Compile Subs ExprTk then use it in the application")
set(USE_SUBS_FPARSER ON CACHE BOOL "Compile Subs FParser then use it in the application")
set(USE_SUBS_JSON ON CACHE BOOL "Add Subs json container")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")


if (NOT EXISTS "${CMAKE_SOURCE_DIR}/lib/maths")
    set(USE_SUBS_EXPRTK OFF)
    set(USE_SUBS_FPARSER OFF)
endif()

set(SUBS_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")

configure_file("${SUBS_INCLUDE_DIR}/subs/subs_config.h.in" "${SUBS_INCLUDE_DIR}/subs/subs_config.h")
add_subdirectory(lib)
add_subdirectory(app)

add_dependencies(SubsApp SubsCore)
if (USE_SUBS_EXPRTK)
    add_dependencies(SubsApp SubsMaths)
endif()
