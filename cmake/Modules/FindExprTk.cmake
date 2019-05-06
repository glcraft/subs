#
# Find LibMC
#
# Try to find LibMC : OpenGL Mathematics.
# This module defines 
# - EXPRTK_INCLUDE_DIRS
# - EXPRTK_FOUND
#
# The following variables can be set as arguments for the module.
# - EXPRTK_ROOT_DIR : Root library directory of LibMC 
# Additional modules
include(FindPackageHandleStandardArgs)
    
if (WIN32)
	# Find include files
	find_path(
		EXPRTK_INCLUDE_DIR
		NAMES exprtk.hpp
		PATHS
		$ENV{PROGRAMFILES}
		$ENV{VISUALEXTEND}
		${EXPRTK_ROOT_DIR}
		PATH_SUFFIXES
		include
		DOC "The directory where exprtk.h resides")
else()
	# Find include files
	find_path(
		EXPRTK_INCLUDE_DIR
		NAMES exprtk.hpp
		PATHS
		/usr/include
		/usr/local/include
		/sw/include
		/opt/local/include
		${EXPRTK_ROOT_DIR}/include
		DOC "The directory where exprtk.h resides")
endif()

# Handle REQUIRD argument, define *_FOUND variable
find_package_handle_standard_args(EXPRTK DEFAULT_MSG EXPRTK_INCLUDE_DIR)

# Hide some variables
mark_as_advanced(EXPRTK_INCLUDE_DIR)