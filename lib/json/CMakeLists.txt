project(SubsJson)
find_package(nlohmann_json 3.2.0 REQUIRED)
file(GLOB SubsJson_SRC FILES
    "src/json_compil_subs.cpp"
)
add_library(SubsJson ${SubsJson_SRC})
target_link_libraries(SubsJson PRIVATE nlohmann_json::nlohmann_json)
target_include_directories(SubsJson 
    PRIVATE "${SUBS_INCLUDE_DIR}"
)