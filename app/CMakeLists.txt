project (SubsApp)

file(GLOB SubsApp_SRC FILES
    "*.cpp"
)

add_executable(SubsApp ${SubsApp_SRC})
target_include_directories(SubsApp PRIVATE ${SUBS_INCLUDE_DIR})
target_link_libraries(SubsApp SubsCore)
if (${USE_SUBSMATHS})
    target_link_libraries(SubsApp SubsMaths)
endif()
set_target_properties(SubsApp PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)