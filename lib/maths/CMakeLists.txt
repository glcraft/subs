project(SubsMaths)

find_package(ExprTk REQUIRED)

set(SUBSMATHS_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

file(GLOB SubsMaths_SRC FILES
    "include/*.h"
    "include/*.hpp"
    "src/*.cpp"
)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj")
add_library(SubsMaths ${SubsMaths_SRC})
target_include_directories(SubsMaths 
    PUBLIC "${EXPRTK_INCLUDE_DIR}"
    PUBLIC ${SUBSCORE_INCLUDE_DIR}
    PRIVATE ${SUBS_INCLUDE_DIR}
)
set_target_properties(SubsMaths PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON
)