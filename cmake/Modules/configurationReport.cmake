#[[ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    Copyright Enrico Sorichetti 2018 - 2019
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file BOOST_LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
#]]
include_guard( GLOBAL )

#[[ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#]]
include( getProgramVersion )

#[[ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#]]
if( CMAKE_WARNINGS )
    message( "" )
    message( STATUS "" )
    message( STATUS "NOTES / WARNINGS : " )
    foreach( _warning ${CMAKE_WARNINGS} )
        message( STATUS "  ${_warning}" )
    endforeach()
endif()

#[[ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#]]
message( "" )
message( STATUS "" )
message( STATUS "configuration summary for project '${PROJECT}' " )
message( STATUS "" )
message( STATUS "  version string .................. : ${VERSION_STRING}" )
message( STATUS "  build ........................... : ${BUILD}" )
message( STATUS "  revision ........................ : ${REVISION}" )
message( STATUS "  RCS ............................. : ${RCS_NAME} - ${RCS_VERSION}" )
message( STATUS "" )

#[[ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#]]
if( REQUIRES_LIBRARIES )
    message( STATUS "  requires libraries .............. : ${REQUIRES_LIBRARIES}" )
    set( _desc      "    found ......................... :" )
    foreach( _item ${REQUIRES_LIBRARIES} )
        message( STATUS "${_desc} ${_item} - ${${_item}_VERSION}" )
        set( _desc  "                                    :" )
    endforeach()
    message( STATUS "" )
endif()

#[[ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#]]
if( PROVIDES_LIBRARIES )
    message( STATUS "  provides libraries .............. : ${PROVIDES_LIBRARIES}" )
    foreach( _item ${PROVIDES_LIBRARIES} )
    endforeach()
    message( STATUS "" )
endif()

#[[ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#]]
if( PROJECT_TEMPLATES )
    message( STATUS "  templates ....................... : ${PROJECT_TEMPLATES}" )
    message( STATUS "" )
endif()

#[[ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#]]
message( STATUS "  build type ...................... : ${CMAKE_BUILD_TYPE}" )
message( STATUS "  word size ....................... : ${HOST_ARCHITECTURE}" )
if( HOST_BIGENDIAN )
    message( STATUS "  platform ........................ : BIG ENDIAN" )
else()
    message( STATUS "  platform ........................ : LITTLE ENDIAN" )
endif()
message( STATUS "  install prefix .................. : ${CMAKE_INSTALL_PREFIX}" )
if( bin_dir )
    message( STATUS "    executables ................... : ${CMAKE_INSTALL_PREFIX}/${bin_dir}" )
endif()
if( inc_dir )
    message( STATUS "    include ....................... : ${CMAKE_INSTALL_PREFIX}/${inc_dir}" )
endif()
if( lib_dir )
    message( STATUS "    libraries ..................... : ${CMAKE_INSTALL_PREFIX}/${lib_dir}" )
endif()
if( bin_dir )
    message( STATUS "    modules ....................... : ${CMAKE_INSTALL_PREFIX}/${mod_dir}" )
endif()
if( shr_dir )
    message( STATUS "    data .......................... : ${CMAKE_INSTALL_PREFIX}/${shr_dir}" )
endif()
message( STATUS "" )

#[[ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#]]
message( STATUS "  CMake version ................... : ${CMAKE_VERSION}" )
set( _desc      "  CMake modules path .............. :" )
foreach( _item ${CMAKE_MODULE_PATH} )
    message( STATUS "${_desc} ${_item}" )
    set( _desc  "                                    :" )
endforeach()
message( STATUS "" )

#[[ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#]]
if( PKG_CONFIG_EXECUTABLE )
    message( STATUS "  pkg-config ...................... : ${PKG_CONFIG_EXECUTABLE} - ${PKG_CONFIG_VERSION_STRING}" )
endif()
if( NOT DEFINED PKG_CONFIG_PATH )
    set( PKG_CONFIG_PATH "$ENV{PKG_CONFIG_PATH}" )
endif()
if( PKG_CONFIG_PATH )
    string( REGEX REPLACE "[:]" ";" PKG_CONFIG_PATH "${PKG_CONFIG_PATH}")
    set( _desc      "  pkg-config path(s) .............. :" )
    foreach( _item ${PKG_CONFIG_PATH} )
        message( STATUS "${_desc} ${_item} " )
        set( _desc  "                                    :" )
    endforeach()
endif()
message( STATUS "" )

#[[ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#]]
get_program_version( "${CMAKE_BUILD_TOOL}")
get_filename_component( _name "${CMAKE_BUILD_TOOL}" NAME_WE)
string( TOUPPER "${_name}" _name )
message( STATUS "  generator ....................... : ${CMAKE_GENERATOR}" )
message( STATUS "  generator program ............... : ${CMAKE_BUILD_TOOL} - ${${_name}_VERSION_STRING}" )
message( STATUS "" )

#[[ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#]]
if( CCACHE )
    get_program_version( "${CCACHE}" )
    get_filename_component( _name "${CCACHE}" NAME_WE)
    string( TOUPPER "${_name}" _name )
    message( STATUS "  ccache program .................. : ${CCACHE} - ${${_name}_VERSION_STRING}" )
    message( STATUS "" )
endif()

#[[ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#]]
get_property( _languages GLOBAL PROPERTY ENABLED_LANGUAGES )
if( _languages )
    message( STATUS "  enabled languages ............... : ${_languages}" )
    message( STATUS "" )
    foreach( _lang ${_languages} )
        string( SUBSTRING "${_lang}   " 0 4 _text)
        message( STATUS "  ${_text}compiler .................... : ${CMAKE_${_lang}_COMPILER}" )
        message( STATUS "  ${_text}compiler ID ................. : ${CMAKE_${_lang}_COMPILER_ID}" )
        message( STATUS "  ${_text}compiler VERSION ............ : ${CMAKE_${_lang}_COMPILER_VERSION}" )
        # message( STATUS "" )
        if( "${CMAKE_BUILD_TYPE}" STREQUAL "" )
            string( TOUPPER "CMAKE_${_lang}_FLAGS" _flags )
        else()
            string( TOUPPER "CMAKE_${_lang}_FLAGS_${CMAKE_BUILD_TYPE}" _flags )
        endif()
        message( STATUS "  ${_text}compiler flags .............. : ${${_flags}}" )
        message( STATUS "" )
    endforeach()
endif()

#[[ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#]]
get_property( _opts DIRECTORY PROPERTY COMPILE_OPTIONS )
if( _opts )
    message( STATUS "  compile options ................. : ${_opts}" )
    message( STATUS "" )
endif()

#[[ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#]]
get_property( _defs DIRECTORY PROPERTY COMPILE_DEFINITIONS )
if( _defs )
    message( STATUS "  compile definitions ............. : ${_defs}" )
    message( STATUS "" )
endif()

#[[ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#]]
get_property( _dirs DIRECTORY PROPERTY INCLUDE_DIRECTORIES )
if( _dirs )
    set( _desc      "  include directories ............. :" )
    foreach( _dir ${_dirs} )
        message( STATUS "${_desc} ${_dir} " )
        set( _desc  "                                    :" )
    endforeach()
    message( STATUS "" )
endif()

#[[ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#]]
get_property( _dirs GLOBAL PROPERTY LINK_DIRECTORIES )
if( _dirs )
    set( _desc      "  link directories ............... :" )
    foreach( _dir ${_dirs} )
        message( STATUS "${_desc} ${_dir} " )
        set( _desc  "                                    :" )
    endforeach()
    message( STATUS "" )
endif()


#[[ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#]]
message( STATUS "  user options .................... :" )
message( STATUS "" )

#[[ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#]]
message( STATUS "end of configuration summary" )
message( STATUS "" )
message( "" )

