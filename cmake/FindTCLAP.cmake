find_package(PkgConfig)
pkg_check_modules(PC_TCLAP QUIET TCLAP)

mark_as_advanced(TCLAP_FOUND TCLAP_INCLUDE_DIR TCLAP_VERSION)

find_path(TCLAP_INCLUDE_DIR
  NAMES tclap/MultiArg.h
  PATHS ${TCLAP_PKGCONF_INCLUDE_DIR}
)

set(TCLAP_VERSION ${PC_TCLAP_VERSION})

mark_as_advanced(TCLAP_FOUND TCLAP_INCLUDE_DIR TCLAP_VERSION)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(TCLAP
	REQUIRED_VARS TCLAP_INCLUDE_DIR
	VERSION_VAR TCLAP_VERSION
)

if(TCLAP_FOUND)
	#Set include dirs to parent, to enable includes like #include <rapidjson/document.h>
	get_filename_component(TCLAP_INCLUDE_DIRS ${TCLAP_INCLUDE_DIR} DIRECTORY)
endif()

if(TCLAP_FOUND AND NOT TARGET TCLAP::TCLAP)
	add_library(TCLAP::TCLAP INTERFACE IMPORTED)
	set_target_properties(TCLAP::TCLAP PROPERTIES
		INTERFACE_INCLUDE_DIRECTORIES ${TCLAP_INCLUDE_DIRS}
	)
endif()
