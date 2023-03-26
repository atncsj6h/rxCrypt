#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#   Copyright Enrico Sorichetti 2020 - 2021
#   Distributed under the Boost Software License, Version 1.0.
#   (See accompanying file BOOST_LICENSE_1_0.txt or copy at
#   http://www.boost.org/LICENSE_1_0.txt)

#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#
message( STATUS "CMake version  '${CMAKE_VERSION}'" )
message( STATUS "Processing     '${CMAKE_CURRENT_LIST_FILE}'" )

#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#
if( CMAKE_VERSION VERSION_LESS  3.25 )
  message( FATAL_ERROR "
 CMake '3.25' or higher is required,
 You are running '${CMAKE_VERSION}'" )
endif()
cmake_minimum_required( VERSION ${CMAKE_VERSION} )

#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#
if( NOT DEFINED ENV{CMTK} )
  message( FATAL_ERROR "
 unable to locate the cmToolKit directory" )
endif()

set( CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR} )
list( APPEND CMAKE_MODULE_PATH "$ENV{CMTK}/cmake/Modules" )
set( TEMPL_SOURCE_DIR "$ENV{CMTK}/cmake/Templates" )

include( vDump )
include( vDrop )
include( vSnap )

#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
# deprecate 'in-source' builds ( source directory/subdirectories )
# if You accidentally try an 'in-source' build
# You will have to manually clean up the leftovers :
# file 'CMakeCache.txt' and directory 'CMakeFiles'
set( CMAKE_DISABLE_IN_SOURCE_BUILD ON )
set( CMAKE_DISABLE_SOURCE_CHANGES  ON )
get_filename_component( _s "${CMAKE_SOURCE_DIR}" REALPATH )
string( TOLOWER "${_s}" _s )
get_filename_component( _b "${CMAKE_BINARY_DIR}" REALPATH )
string( TOLOWER "${_b}" _b )
if( ( "${_b}" STREQUAL "${_s}" ) OR
    ( "${_b}" MATCHES  "${_s}/" ) )
  message( FATAL_ERROR "
 binary directory '${CMAKE_BINARY_DIR}'
 source directory '${CMAKE_SOURCE_DIR}'
 builds that modify the source directory/subdirectories are not allowed,
 remove file 'CMakeCache.txt' and directory 'CMakeFiles' " )
endif()

#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
# must come before the project command
# lowest macOS supported 10.13.6 High Sierra
if( APPLE AND BUILD_OSX_UNIVERSAL_BINARIES )
  set( CMAKE_OSX_DEPLOYMENT_TARGET 10.13.6 CACHE STRING  "" FORCE)
  set( CMAKE_C_FLAGS   "-Wunguarded-availability" )
  set( CMAKE_CXX_FLAGS "-Wunguarded-availability" )
endif()

#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#
set( PROJECT        "rxcrypt" )
set( VERSION        1.0.0 )
project( ${PROJECT}
  VERSION ${VERSION}
  LANGUAGES C CXX )

set( VERSION_STRING "${VERSION}" )
set( VERSION_STRING "${VERSION_STRING}-${CMAKE_SYSTEM_PROCESSOR}" )

#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
# check that the 'oorexx' libraries are available
# and terminate if not
include( FindPkgConfig )
pkg_check_modules( oorexx oorexx )
if( NOT oorexx_FOUND )
    message( FATAL_ERROR "
 unable to locate the ooRexx libraries " )
endif()

#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#
set( CMAKE_C_STANDARD 11 )
set( CMAKE_C_STANDARD_REQUIRED YES )
set( CMAKE_CXX_STANDARD 11 )
set( CMAKE_CXX_STANDARD_REQUIRED YES )

#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#
set( SYSTEM_NAME "${CMAKE_HOST_SYSTEM_NAME}" )
string( TOUPPER "${SYSTEM_NAME}" SYSTEM_NAME_UC  )
set( "${SYSTEM_NAME_UC}" 1 )
add_compile_definitions(
  SYSTEM_NAME="${SYSTEM_NAME}"
  ${SYSTEM_NAME_UC}
)

#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#
if( APPLE AND BUILD_OSX_UNIVERSAL_BINARIES )
  set( CMAKE_OSX_ARCHITECTURES arm64 x86_64 )
  set( VERSION_STRING "${VERSION_STRING}-universal" )
endif()

string( TIMESTAMP  BUILD_TIMESTAMP %Y/%m/%d-%H:%M:%S )
set( VERSION_STRING "${VERSION_STRING} ${BUILD_TIMESTAMP}" )

message( STATUS "Building ${PROJECT} ${VERSION_STRING}" )
add_compile_definitions(
  VERSION_STRING="${VERSION_STRING}" )

#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
# arch/bits
math( EXPR HOST_ARCH "8 * ${CMAKE_SIZEOF_VOID_P}" OUTPUT_FORMAT DECIMAL)
if( NOT ( HOST_ARCH MATCHES 64 ) )
  message( SEND_ERROR "
 unsupported Architecture '${HOST_ARCH}-bits'" )
endif()
# message( STATUS "Building ${PROJECT} ${VERSION_STRING} '${HOST_ARCH}-bits'" )

#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
# endian-ness
if( NOT ( CMAKE_C_BYTE_ORDER STREQUAL "LITTLE_ENDIAN" ) )
  message( SEND_ERROR "
 unsupported Endian-ness '${CMAKE_C_BYTE_ORDER}'" )
endif()

#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#   setup the proper install paths according to the GNU standards,
include( installDirectories )

# set( CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${INST_BIN_DIR}" )
# set( CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${INST_LIB_DIR}" )
# set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${INST_LIB_DIR}" )

#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#
macro( copy_file name )
    configure_file( ${CMAKE_SOURCE_DIR}/${name} ${CMAKE_BINARY_DIR}/${name}
        COPYONLY )
endmacro( copy_file)

copy_file( rxAes.rex )
copy_file( rxEcb.rex )
copy_file( rxCbc.rex )
copy_file( rxOfb.rex )
copy_file( rxHash.rex )

#[[ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#]]
add_definitions( -DENABLE_CSRAND )

add_definitions( -DENABLE_SHA224 )
add_definitions( -DENABLE_SHA384 )

#[[ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#]]
add_compile_options( -fPIC )

#[[ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#]]
include_directories( ${CMAKE_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/lib/include
  ${CMAKE_SOURCE_DIR}/src/include
  ${oorexx_INCLUDE_DIRS}
)

link_directories( ${oorexx_LIBRARY_DIRS} )

#[[ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#]]
add_library( crypt
  STATIC
  ${CMAKE_SOURCE_DIR}/lib/aes.c
  ${CMAKE_SOURCE_DIR}/lib/ecb.c
  ${CMAKE_SOURCE_DIR}/lib/cbc.c
  ${CMAKE_SOURCE_DIR}/lib/ctr.c
  ${CMAKE_SOURCE_DIR}/lib/ofb.c
  ${CMAKE_SOURCE_DIR}/lib/md5.c
  ${CMAKE_SOURCE_DIR}/lib/sha1.c
  ${CMAKE_SOURCE_DIR}/lib/sha2.c
)
if( HAVE_IPO_SUPPORT )
  set_property( TARGET crypt
    PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE
  )
endif()

add_library( rxcrypt
  SHARED
  ${CMAKE_SOURCE_DIR}/src/rxcrypt.cpp
  ${CMAKE_SOURCE_DIR}/src/shvaccess.c
)
if( HAVE_IPO_SUPPORT )
  set_property( TARGET rxcrypt
    PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE
  )
endif()
target_link_libraries( rxcrypt
    crypt
    ${oorexx_LIBRARIES} )

#[[ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#]]
vdump( "rxcrypt" "exit")
