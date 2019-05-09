#
# Find FParser
#
# Try to find FParser
# This module defines 
# - FPARSER_INCLUDE_DIRS
# - FPARSER_FOUND
#
# The following variables can be set as arguments for the module.
# - FPARSER_ROOT_DIR : Root library directory of FParser 
#
# Additional modules
include(FindPackageHandleStandardArgs)

# Invoke pkgconfig for hints
find_package(PkgConfig QUIET)
set(FPARSER_INCLUDE_HINTS)
set(FPARSER_LIB_HINTS)
if(PKG_CONFIG_FOUND)
	pkg_search_module(FPARSERPC QUIET FPARSER)
	if(FPARSERPC_INCLUDE_DIRS)
		set(FPARSER_INCLUDE_HINTS ${FPARSERPC_INCLUDE_DIRS})
	endif()
	if(FPARSERPC_LIBRARY_DIRS)
		set(FPARSER_LIB_HINTS ${FPARSERPC_LIBRARY_DIRS})
	endif()
endif()
if (WIN32)
	if (${CMAKE_SIZEOF_VOID_P} EQUAL 8) # 64-bit
		set(SUFFIX_LIB lib/Windows/x64)
	else()
		set(SUFFIX_LIB lib/Windows/x86)
	endif()
else()
	set(SUFFIX_LIB "")
endif()


find_library(FPARSER_LIBRARY_DEBUG
	NAMES
    fparserd
    HINTS
    ${FPARSER_LIB_HINTS}
	PATHS
    ${FPARSER_ROOT_DIR}
    ${DEPS_ROOT}
	$ENV{VISUALEXTEND}
	ENV FPARSERDIR
    PATH_SUFFIXES lib ${SUFFIX_LIB} FPARSER ${FPARSER_LIB_PATH_SUFFIX})
find_library(FPARSER_LIBRARY_RELEASE
    NAMES 
    fparser
    HINTS ${FPARSER_LIB_HINTS}
	PATHS
	${FPARSER_ROOT_DIR}
    ${DEPS_ROOT}
	$ENV{VISUALEXTEND}
	ENV FPARSERDIR
    PATH_SUFFIXES lib ${SUFFIX_LIB} FPARSER ${FPARSER_LIB_PATH_SUFFIX})
    
if (WIN32)
	# Find include files
	find_path(
		FPARSER_INCLUDE_DIR
		NAMES json/json.hpp
		PATHS
        ${DEPS_ROOT}
		$ENV{PROGRAMFILES}
		$ENV{VISUALEXTEND}
		${FPARSER_ROOT_DIR}
		PATH_SUFFIXES
		include
		DOC "The directory where jsoncpp/jsoncpp.hpp resides")
else()
	# Find include files
	find_path(
		FPARSER_INCLUDE_DIR
		NAMES json/json.hpp
		PATHS
        ${DEPS_ROOT}
		/usr/include
		/usr/local/include
		/sw/include
		/opt/local/include
		${FPARSER_ROOT_DIR}/include
		DOC "The directory where jsoncpp/jsoncpp.hpp resides")
endif()

set(FPARSER_LIBRARIES debug ${FPARSER_LIBRARY_DEBUG} optimized ${FPARSER_LIBRARY_RELEASE})

# Handle REQUIRD argument, define *_FOUND variable
find_package_handle_standard_args(FPARSER DEFAULT_MSG FPARSER_INCLUDE_DIR)

# Hide some variables
mark_as_advanced(FPARSER_INCLUDE_DIR FPARSER_LIBRARY_DEBUG FPARSER_LIBRARY_RELEASE)