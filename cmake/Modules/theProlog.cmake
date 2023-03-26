#[[ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    Copyright Enrico Sorichetti 2018 - 2019
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file BOOST_LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
#]]

#[[ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#]]
include_guard( GLOBAL )

#[[ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    deprecate 'In source' builds ( source directory/subdirectories )
    if You accidentally try an in-source build
    You will still have to manually clean up a few leftovers :
    file 'CMakeCache.txt' and directory 'CMakeFiles'
#]]
set( CMAKE_DISABLE_IN_SOURCE_BUILD ON )
set( CMAKE_DISABLE_SOURCE_CHANGES  ON )

get_filename_component( _s "${CMAKE_SOURCE_DIR}" REALPATH)
string( TOLOWER "${_s}" _s )

get_filename_component( _b "${CMAKE_BINARY_DIR}" REALPATH)
string( TOLOWER "${_b}" _b )

if( ( "${_b}" STREQUAL "${_s}" ) OR
    ( "${_b}" MATCHES  "${_s}/" ) )
    message( FATAL_ERROR "
 binary directory '${CMAKE_BINARY_DIR}'
 source directory '${CMAKE_SOURCE_DIR}'
 builds that modify the source directory/subdirectories are not allowed,
 remove the files 'CMakeCache.txt' and the directory 'CMakeFiles' " )
endif()

#[[ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    dummy project to schedule as soon as possible the basic cmake check's
    and to enable the default languages (``C`` and ``CXX`` )
#]]
project( dummy )

#[[ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    helpers
#]]
include( vDump )
include( vSnap )
include( vDrop )
include( vScan )

#[[ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    cleanup the constructs of the dummy project
#]]
vdrop( "CMAKE_PROJECT_*" "dummy_*" "PROJECT_*" "Project_*" "quiet" )

#[[ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#]]
include( RCSinfo )
set( BUILD      "${RCS_WC_COMMITS}" )
set( REVISION   "${RCS_WC_REVISION}" )

#[[ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    architecture
#]]
if( CMAKE_SIZEOF_VOID_P MATCHES 8 )
    set( HOST_ARCHITECTURE "64" )
elseif( CMAKE_SIZEOF_VOID_P MATCHES 4 )
    set( HOST_ARCHITECTURE "32")
else()
    message( FATAL_ERROR "
 unable to determine the 'word size'/'system architecture' " )
endif()

#[[ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    endian-ness
#]]
include( TestBigEndian )
test_big_endian( HOST_BIGENDIAN )
vdrop( "HAVE_HOST_BIGENDIAN" "quiet" )

#[[ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#]]
if( WIN32 )
    include( MSVC_defaultCompilerFlags OPTIONAL )
else()
    include( UNIX_defaultCompilerFlags )
endif()

#[[ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    ccache
#]]
include( ccache )

#[[ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#]]

