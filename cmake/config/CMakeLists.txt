## ---------------------------------------------------------------------
##
## This file is copyrighted by the deal2lkit authors and by the deal.II 
## authors (see below for the original copyright in the deal.II library.)
## 
## The structure of the cmake files are the same of those of the 
## deal.II library and they have been modified in order to make them
## compatible with the deal2lkit library.
##
## This file is part of the deal2lkit library.
##
## The deal2lkit library is free software; you can use it, redistribute
## it, and/or modify it under the terms of the GNU Lesser General
## Public License as published by the Free Software Foundation; either
## version 2.1 of the License, or (at your option) any later version.
## The full text of the license can be found in the file LICENSE at
## the top level of the deal2lkit distribution.
##
## ---------------------------------------------------------------------

## ---------------------------------------------------------------------
##
## Copyright (C) 2012 - 2015 by the deal.II authors
##
## This file is part of the deal.II library.
##
## The deal.II library is free software; you can use it, redistribute
## it, and/or modify it under the terms of the GNU Lesser General
## Public License as published by the Free Software Foundation; either
## version 2.1 of the License, or (at your option) any later version.
## The full text of the license can be found in the file LICENSE at
## the top level of the deal.II distribution.
##
## ---------------------------------------------------------------------


#
# This file sets up the project configuration consisting of
#
#   deal2lkitConfig.cmake
#   deal2lkitVersionConfig.cmake
#   Make.global_options
#
# We support two configurations out of which deal2lkit can be used - directly
# from the build directory or after installation. So we have to prepare
# two distinct setups.
#

MESSAGE(STATUS "Setting up project configuration")


########################################################################
#                                                                      #
#                   Setup and install cmake macros:                    #
#                                                                      #
########################################################################

SET(_macros
  ${CMAKE_SOURCE_DIR}/cmake/macros/macro_deal2lkit_initialize_cached_variables.cmake
  ${CMAKE_SOURCE_DIR}/cmake/macros/macro_deal2lkit_invoke_autopilot.cmake
  ${CMAKE_SOURCE_DIR}/cmake/macros/macro_deal2lkit_setup_target.cmake
  ${CMAKE_SOURCE_DIR}/cmake/macros/macro_deal2lkit_query_git_information.cmake
  #  ${CMAKE_SOURCE_DIR}/cmake/macros/macro_deal_ii_add_test.cmake
  #${CMAKE_SOURCE_DIR}/cmake/macros/macro_deal_ii_pickup_tests.cmake
  )
FILE(COPY ${_macros}
  DESTINATION ${CMAKE_BINARY_DIR}/${D2K_SHARE_RELDIR}/macros
  )
INSTALL(FILES ${_macros}
  DESTINATION ${D2K_SHARE_RELDIR}/macros
  COMPONENT library
  )


########################################################################
#                                                                      #
#        Configure and install the cmake project configuration:        #
#                                                                      #
########################################################################

#
# Do not force --as-needed for executables on user side:
#

STRIP_FLAG(D2K_LINKER_FLAGS "-Wl,--as-needed")

#
# Strip -Wno-deprecated-declarations from D2K_CXX_FLAGS so that
# deprecation warnings are actually shown for user code:
#
STRIP_FLAG(D2K_CXX_FLAGS "-Wno-deprecated-declarations")

#
# Populate a bunch of CONFIG_* variables with useful information:
#

FOREACH(_build ${D2K_BUILD_TYPES})

  IF(NOT "${CMAKE_BUILD_TYPE}" STREQUAL "DebugRelease")
    SET(_keyword "general")
  ELSE()
    IF(_build MATCHES DEBUG)
      SET(_keyword "debug")
    ELSE()
      SET(_keyword "optimized")
    ENDIF()
  ENDIF()

  #
  # Build up library name depending on link type and platform:
  #

  IF(BUILD_SHARED_LIBS)
    SET(_type "SHARED")
  ELSE()
    SET(_type "STATIC")
  ENDIF()

  SET(_name "${CMAKE_${_type}_LIBRARY_PREFIX}${D2K_BASE_NAME}${D2K_${_build}_SUFFIX}${CMAKE_${_type}_LIBRARY_SUFFIX}")
  SET(CONFIG_LIBRARIES_${_build}
    "\${D2K_PATH}/${D2K_LIBRARY_RELDIR}/${_name}"
    ${D2K_LIBRARIES_${_build}}
    ${D2K_LIBRARIES}
    )
  TO_STRING(MAKEFILE_LIBRARIES_${_build} ${CONFIG_LIBRARIES_${_build}})
  TO_STRING(MAKEFILE_USER_DEFINITIONS_${_build} ${D2K_USER_DEFINITIONS_${_build}})
  LIST(APPEND CONFIG_LIBRARIES ${_keyword} \${D2K_LIBRARIES_${_build}})

  SET(CONFIG_TARGET_${_build} ${D2K_BASE_NAME}${D2K_${_build}_SUFFIX})
  LIST(APPEND CONFIG_TARGET ${_keyword} \${D2K_TARGET_${_build}})
ENDFOREACH()

TO_STRING(MAKEFILE_BUILD_TYPES ${D2K_BUILD_TYPES})
TO_STRING(MAKEFILE_USER_DEFINITIONS ${D2K_USER_DEFINITIONS})

IF(D2K_STATIC_EXECUTABLE)
  SET(MAKEFILE_STATIC_EXECUTABLE "true")
ELSE()
  SET(MAKEFILE_STATIC_EXECUTABLE "false")
ENDIF()

#
# For binary dir:
#

SET(CONFIG_BUILD_DIR TRUE)
SET(MAKEFILE_BUILD_DIR "true")
SET(CONFIG_INCLUDE_DIRS
  \${D2K_PATH}/include
  ${CMAKE_SOURCE_DIR}/include/
  ${D2K_BUNDLED_INCLUDE_DIRS}
  ${D2K_USER_INCLUDE_DIRS}
  )
CONFIGURE_FILE(
  ${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
  ${CMAKE_BINARY_DIR}/${D2K_PROJECT_CONFIG_RELDIR}/${D2K_PROJECT_CONFIG_NAME}Config.cmake
  @ONLY
  )
CONFIGURE_FILE(
  ${CMAKE_CURRENT_SOURCE_DIR}/ConfigVersion.cmake.in
  ${CMAKE_BINARY_DIR}/${D2K_PROJECT_CONFIG_RELDIR}/${D2K_PROJECT_CONFIG_NAME}ConfigVersion.cmake
  @ONLY
  )
TO_STRING(MAKEFILE_INCLUDE_DIRS ${CONFIG_INCLUDE_DIRS})
CONFIGURE_FILE(
  ${CMAKE_CURRENT_SOURCE_DIR}/Make.global_options.in
  ${CMAKE_BINARY_DIR}/${D2K_SHARE_RELDIR}/Make.global_options
  @ONLY
  )

#
# For installation:
#

SET(CONFIG_BUILD_DIR FALSE)
SET(MAKEFILE_BUILD_DIR "false")
SET(CONFIG_INCLUDE_DIRS
  \${D2K_PATH}/\${D2K_INCLUDE_RELDIR}
  ${D2K_USER_INCLUDE_DIRS}
  )
CONFIGURE_FILE(
  ${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/${D2K_PROJECT_CONFIG_NAME}Config.cmake
  @ONLY
  )
CONFIGURE_FILE(
  ${CMAKE_CURRENT_SOURCE_DIR}/ConfigVersion.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/${D2K_PROJECT_CONFIG_NAME}ConfigVersion.cmake
  @ONLY
  )
TO_STRING(MAKEFILE_INCLUDE_DIRS ${CONFIG_INCLUDE_DIRS})
CONFIGURE_FILE(
  ${CMAKE_CURRENT_SOURCE_DIR}/Make.global_options.in
  ${CMAKE_CURRENT_BINARY_DIR}/Make.global_options
  @ONLY
  )
INSTALL(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/${D2K_PROJECT_CONFIG_NAME}Config.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/${D2K_PROJECT_CONFIG_NAME}ConfigVersion.cmake
  DESTINATION ${D2K_PROJECT_CONFIG_RELDIR}
  COMPONENT library
  )
INSTALL(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/Make.global_options
  DESTINATION ${D2K_SHARE_RELDIR}
  COMPONENT library
  )

#
# Append feature configuration to all configuration files:
#

SET(_files
  ${CMAKE_BINARY_DIR}/${D2K_PROJECT_CONFIG_RELDIR}/${D2K_PROJECT_CONFIG_NAME}Config.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/${D2K_PROJECT_CONFIG_NAME}Config.cmake
  )
SET(_makefiles
  ${CMAKE_BINARY_DIR}/${D2K_SHARE_RELDIR}/Make.global_options
  ${CMAKE_CURRENT_BINARY_DIR}/Make.global_options
  )
FOREACH(_file ${_files} ${_makefiles})
  FILE(APPEND ${_file} "\n\n#\n# Feature configuration:\n#\n\n")
ENDFOREACH()

GET_CMAKE_PROPERTY(res VARIABLES)
FOREACH(var ${res})
  IF(var MATCHES "D2K_WITH")
    STRING(REPLACE "D2K_WITH_" "" _name ${var})

    FOREACH(_file ${_files})
      FILE(APPEND ${_file} "SET(${var} ${${var}})\n")
      IF(${var} AND NOT "${${_name}_VERSION}" STREQUAL "")
        FILE(APPEND ${_file}
          "SET(D2K_${_name}_VERSION \"${${_name}_VERSION}\")\n"
          )
      ENDIF()
    ENDFOREACH()

    FOREACH(_file ${_makefiles})
      IF(${var})
        FILE(APPEND ${_file} "${var} = true\n")
        IF(NOT "${${_name}_VERSION}" STREQUAL "")
          FILE(APPEND ${_file}
            "D2K_${_name}_VERSION = ${${_name}_VERSION}\n"
            )
        ENDIF()
      ELSE()
        FILE(APPEND ${_file} "${var} = false\n")
      ENDIF()
    ENDFOREACH()
  ENDIF()
ENDFOREACH()

MESSAGE(STATUS "Setting up project configuration - Done")
