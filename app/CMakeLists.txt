project (SubsApp)

file(GLOB SubsApp_SRC FILES
    "main.cpp"
    "common.cpp"
    "common.h"
    "regex_part.cpp"
    "rename_part.cpp"
    "nothing_part.cpp"
)
if (${USE_SUBS_JSON})
    LIST(APPEND SubsApp_SRC "json_part.cpp")
endif()
add_executable(SubsApp ${SubsApp_SRC})
target_include_directories(SubsApp PRIVATE ${SUBS_INCLUDE_DIR})
target_link_libraries(SubsApp SubsCore)
if (${USE_SUBS_EXPRTK} OR ${USE_SUBS_FPARSER})
    target_link_libraries(SubsApp SubsMaths)
endif()
if (${USE_SUBS_JSON})
    target_link_libraries(SubsApp SubsJson)
endif()
set_target_properties(SubsApp PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)