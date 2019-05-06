project(SubsCore)

file(GLOB SubsCore_SRC FILES
    "include/*.h"
    "include/*.hpp"
    "src/*.cpp"
)
list(FILTER SubsCore_SRC EXCLUDE REGEX ".*json.*")
add_library(SubsCore ${SubsCore_SRC})
target_include_directories(SubsCore PRIVATE ${SUBS_INCLUDE_DIR})
set_target_properties(SubsCore PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON
)