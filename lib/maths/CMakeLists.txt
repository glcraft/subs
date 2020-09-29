
project(SubsMaths)

set(SEXPRTK_INCLUDE_DIR)
set(SFPARSER_INCLUDE_DIR)
set(SFPARSER_LIBRARIES)
if (USE_SUBS_EXPRTK)
    message("subs : Using ExprTk evaluation")
    find_package(ExprTk REQUIRED)
    list(APPEND SubsMaths_SRC "src/subs_exprtk.cpp")
    set(SEXPRTK_INCLUDE_DIR ${EXPRTK_INCLUDE_DIR})
    if (MSVC)
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj")
    endif()
    
endif()
if (USE_SUBS_FPARSER)
    message("subs : Using FParser evaluation")
    find_package(FParser REQUIRED)
    list(APPEND SubsMaths_SRC "src/subs_fparser.cpp")
    set(SFPARSER_LIBRARIES ${FPARSER_LIBRARIES})
    set(SFPARSER_INCLUDE_DIR ${FPARSER_INCLUDE_DIR})
    
endif()

add_library(SubsMaths ${SubsMaths_SRC})
if (USE_SUBS_FPARSER)
    target_include_directories(SubsMaths 
        PUBLIC "${SFPARSER_INCLUDE_DIR}"
    )
endif()
if (USE_SUBS_EXPRTK)
    target_include_directories(SubsMaths 
        PUBLIC "${SEXPRTK_INCLUDE_DIR}"
    )
endif()
target_include_directories(SubsMaths 
    PRIVATE "${SUBS_INCLUDE_DIR}"
)
target_link_libraries(SubsMaths ${SFPARSER_LIBRARIES})
set_target_properties(SubsMaths PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON
)