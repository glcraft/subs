
add_subdirectory(core)
if (${USE_SUBS_EXPRTK} OR ${USE_SUBS_FPARSER})
    add_subdirectory(maths)
endif()
if (${USE_SUBS_JSON})
    add_subdirectory(json)
endif()
# add_subdirectory(lps)
