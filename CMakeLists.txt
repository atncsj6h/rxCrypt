#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
# Copyright (c) 2019, 2023 Enrico Sorichetti
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file BOOST_LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
message( STATUS "CMake version  '${CMAKE_VERSION}'" )
message( STATUS "Processing     '${CMAKE_CURRENT_LIST_FILE}'" )

#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
if( CMAKE_VERSION VERSION_LESS  3.25 )
  message( FATAL_ERROR "
 CMake '3.25' or higher is required,
 You are running '${CMAKE_VERSION}'" )
endif()
cmake_minimum_required( VERSION ${CMAKE_VERSION} )

#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
project( rxcrypt
  LANGUAGES C CXX )

#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
find_program( oorexx_EXECUTABLE rexx )

if( NOT oorexx_EXECUTABLE )
  message( FATAL_ERROR "
 unable to continue - ooRexx not available " )
endif()

get_filename_component(oorexx_PREFIX  "${oorexx_EXECUTABLE}" DIRECTORY )
get_filename_component(oorexx_PREFIX  "${oorexx_PREFIX}" DIRECTORY )
set( oorexx_INCLUDEDIR "${oorexx_PREFIX}/include" )
set( oorexx_LIBDIR     "${oorexx_PREFIX}/lib" )
set( ooRexx_LIBRARIES  "rexx;rexxapi" )

message( "++ oorexx_EXECUTABLE  '${oorexx_EXECUTABLE}' " )
message( "++ oorexx_PREFIX      '${oorexx_PREFIX}' " )
message( "++ oorexx_INCLUDEDIR  '${oorexx_INCLUDEDIR}' " )
message( "++ oorexx_LIBDIR      '${oorexx_INCLUDEDIR}' " )
message( "++ ooRexx_LIBRARIES   '${ooRexx_LIBRARIES}' " )

#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
set( CMAKE_C_STANDARD 11 )
set( CMAKE_C_STANDARD_REQUIRED YES )
set( CMAKE_CXX_STANDARD 11 )
set( CMAKE_CXX_STANDARD_REQUIRED YES )

#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
option( BUILD_UNIVERSAL_BINARIES "" ON )
if( APPLE AND BUILD_UNIVERSAL_BINARIES )
  set( CMAKE_OSX_ARCHITECTURES arm64 x86_64 )
endif()

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
  ${CMAKE_SOURCE_DIR}/lib/include
  ${oorexx_INCLUDEDIR}
)

link_directories(
  ${oorexx_LIBDIR}
)

#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
add_library( rxcrypt SHARED
  rxcrypt.cpp
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
