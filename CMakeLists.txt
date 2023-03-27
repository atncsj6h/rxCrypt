#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
# Copyright Enrico Sorichetti 2021 - 2023
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file BOOST_LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
message( STATUS "CMake version  '${CMAKE_VERSION}'" )
message( STATUS "Processing     '${CMAKE_CURRENT_LIST_FILE}'" )

#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
if( CMAKE_VERSION VERSION_LESS  3.20 )
  message( FATAL_ERROR "
 CMake '3.20' or higher is required,
 You are running '${CMAKE_VERSION}'" )
endif()
cmake_minimum_required( VERSION ${CMAKE_VERSION} )

#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
# must come before the project command
# lowest macOS supported 10.13.6 High Sierra
if( APPLE AND BUILD_UNIVERSAL_BINARIES )
  set( CMAKE_OSX_DEPLOYMENT_TARGET 10.13.6 CACHE STRING  "" FORCE)
  add_compile_options( -Wunguarded-availability )
endif()

#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#
set( VERSION 1.0.0 )

project( rxcrypt
  VERSION ${VERSION}
  LANGUAGES C CXX )
set( VERSION_STRING "${VERSION}" )
set( VERSION_STRING "${VERSION_STRING}-${CMAKE_SYSTEM_PROCESSOR}" )

#[[ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#]]
find_program( REXX_EXECUTABLE rexx )
if( REXX_EXECUTABLE )
    message( "++ REXX_EXECUTABLE ${REXX_EXECUTABLE} " )
    get_filename_component(OOREXX_INSTALL_PREFIX "${REXX_EXECUTABLE}" DIRECTORY )
    get_filename_component(OOREXX_INSTALL_PREFIX "${OOREXX_INSTALL_PREFIX}" DIRECTORY )
    message( "++ OOREXX_INSTALL_PREFIX ${OOREXX_INSTALL_PREFIX} " )
endif()

if( NOT REXX_EXECUTABLE )
  message( FATAL_ERROR "
 unable to continue - ooRexx not available " )
endif()

message( "++ REXX_EXECUTABLE ${REXX_EXECUTABLE} " )
get_filename_component(oorexx_PREFIX "${REXX_EXECUTABLE}" DIRECTORY )
get_filename_component(oorexx_PREFIX  "${oorexx_PREFIX}" DIRECTORY )
message( "++ oorexx_PREFIX ${oorexx_PREFIX} " )

set( oorexx_INCLUDEDIR "${oorexx_PREFIX}/include" )
set( oorexx_LIBDIR     "${oorexx_PREFIX}/lib" )
set( ooRexx_LIBRARIES  "rexx;rexxapi" )

#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
set( CMAKE_C_STANDARD 11 )
set( CMAKE_C_STANDARD_REQUIRED YES )
set( CMAKE_CXX_STANDARD 11 )
set( CMAKE_CXX_STANDARD_REQUIRED YES )

#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
option( BUILD_UNIVERSAL_BINARIES "" ON )

if( APPLE AND BUILD_UNIVERSAL_BINARIES )
  set( CMAKE_OSX_ARCHITECTURES arm64 x86_64 )
  set( VERSION_STRING "${VERSION_STRING}-universal" )
endif()

string( TIMESTAMP  BUILD_TIMESTAMP %Y/%m/%d-%H:%M:%S )
set( VERSION_STRING "${VERSION_STRING} ${BUILD_TIMESTAMP}" )

message( STATUS "Building ${PROJECT} ${VERSION_STRING}" )

add_compile_definitions(
  VERSION_STRING="${VERSION_STRING}"
  ENABLE_SHA224 ENABLE_SHA384
)

#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
option( WITH_IPO "" ON )
if( WITH_IPO )
  include( CheckIPOSupported )
  check_ipo_supported( RESULT HAVE_IPO_SUPPORT )
  if ( HAVE_IPO_SUPPORT )
    message( STATUS "IPO is Supported and Enabled" )
  endif()
endif()

#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
include_directories(
  ${CMAKE_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/src/include
  ${CMAKE_SOURCE_DIR}/lib/include
  ${oorexx_INCLUDEDIR}
)

link_directories(
  ${oorexx_LIBDIR}
)

#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
add_library( rxcrypt SHARED
  src/rxcrypt.cpp
  lib/aes.c
  lib/cbc.c
  lib/ctr.c
  lib/ecb.c
  lib/md5.c
  lib/ofb.c
  lib/sha1.c
  lib/sha2.c
)

#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
if( HAVE_IPO_SUPPORT )
  set_property( TARGET rxcrypt
    PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE
  )
endif()

target_link_libraries( rxcrypt
  ${oorexx_LIBRARIES}
)
