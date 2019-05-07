
add_subdirectory(core)
if (${USE_SUBS_EXPRTK} OR ${USE_SUBS_FPARSER})
    add_subdirectory(maths)
endif()
# add_subdirectory(lps)
