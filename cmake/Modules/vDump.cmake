#[[ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    Copyright Enrico Sorichetti 2018 - 2019
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file BOOST_LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
#]]
include_guard( GLOBAL )

#[[ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#]]
function( vdump _arglist _argline )

if( ${_argline} MATCHES   "^[0-9]+$" )
    set( _line "0000${_argline}" )
    string( LENGTH "${_line}"  _len )
    math( EXPR _indx "${_len} - 4" )
    string(SUBSTRING ${_line} ${_indx} -1 _line )
else()
    string( REGEX REPLACE "[^a-zA-Z0-9_]" "_" _line  "${_argline}" )
endif()

get_filename_component( _list "${_arglist}" NAME_WE)
if( "${_line}" STREQUAL "" )
    string( REGEX REPLACE "[^a-zA-Z0-9_]" "_"
            _out "vars_for_${_list}")
else()
    string( REGEX REPLACE "[^a-zA-Z0-9_]" "_"
            _out "vars_for_${_list}_at_${_line}")
endif()

set( _out "${CMAKE_BINARY_DIR}/${_out}.txt" )
file( REMOVE ${_out} )

set( _buf "" )

get_cmake_property( _vars VARIABLES )

set( heads
    "ARG" "_CMAKE" "__" "_CURRENT" "_INCLUDED" "_SET" "_IN_" "CMAKE_Fortran"
    "BISON_version_" )
set( tails
    "_CONTENT" "_COMPILER_ID_TOOL_MATCH_REGEX" )

foreach( _var IN LISTS _vars )

    # lower case variables are short SCOPED variables
    string( TOLOWER "${_var}" _lc )
    if( "${_var}" STREQUAL "${_lc}" )
        continue()
    endif()

    unset( skip )
    unset( skip  CACHE )
    foreach( head IN LISTS heads )
        if( "${_var}" MATCHES "^(${head})" )
            set( skip "1" )
            break()
        endif()
    endforeach()
    if( skip )
        continue()
    endif()

    unset( skip )
    unset( skip  CACHE )
    foreach( tail IN LISTS tails )
        if( "${_var}" MATCHES "(${tail})$" )
            set( skip "1" )
            break()
        endif()
    endforeach()
    if( skip )
        continue()
    endif()

    if( "${_var}" STREQUAL "OUTPUT" )
        continue()
    endif()

    string( APPEND _buf "[[ ${_var}='${${_var}}'\n" )
endforeach()

file( WRITE "${_out}" "${_buf}" )

endfunction()
