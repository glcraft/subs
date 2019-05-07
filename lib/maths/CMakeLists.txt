project(SubsMaths)

set(SEXPRTK_INCLUDE_DIR)
set(SFPARSER_INCLUDE_DIR)
set(SFPARSER_LIBRARIES)
if (USE_SUBS_EXPRTK)
    find_package(ExprTk REQUIRED)
    file(GLOB Filemaths FILES
    "src/subs_exprtk.cpp"
    )
    list(APPEND SubsMaths_SRC ${Filemaths})
    set(SFPARSER_INCLUDE_DIR ${FPARSER_INCLUDE_DIR})
    if (MSVC)
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj")
    endif()
endif()
if (USE_SUBS_FPARSER)
    find_package(FParser REQUIRED)
    file(GLOB Filemaths FILES
        "src/subs_fparser.cpp"
    )
    list(APPEND SubsMaths_SRC ${Filemaths})
    set(SFPARSER_LIBRARIES ${FPARSER_LIBRARIES})
    set(SFPARSER_INCLUDE_DIR ${FPARSER_INCLUDE_DIR})
endif()

add_library(SubsMaths ${SubsMaths_SRC})
target_include_directories(SubsMaths 
    PUBLIC "${SEXPRTK_INCLUDE_DIR}"
    PUBLIC "${SFPARSER_INCLUDE_DIR}"
    PRIVATE "${SUBS_INCLUDE_DIR}"
)
target_link_libraries(SubsMaths ${SFPARSER_LIBRARIES})
set_target_properties(SubsMaths PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON
)