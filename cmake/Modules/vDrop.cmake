#[[ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    Copyright Enrico Sorichetti 2018 - 2019
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file BOOST_LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
#]]
include_guard( GLOBAL )

#[[ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#]]
function( vdrop )

    set( _args "${ARGV}" )
    list( SORT _args )
    list( REMOVE_DUPLICATES _args )

    string( TOLOWER "${_args}" _temp )
    list( FIND _temp "quiet" _indx )
    if( _indx GREATER_EQUAL 0 )
        list( REMOVE_AT _args "${_indx}" )
        set( _msg FALSE )
    else()
        set( _msg TRUE )
    endif()

    set( _selvars "" )

    get_cmake_property( _allvars VARIABLES )

    foreach( _argv IN LISTS _args )

        string( REGEX MATCH "([^\\*]*)" _head "${_argv}" )

        if( _head STREQUAL _argv )
            list( APPEND _selvars "${_argv}" )
            continue()
        endif()

        string (REGEX MATCHALL "(^|;)${_head}[A-Za-z0-9_]*" _match "${_allvars}")

        foreach( _var IN LISTS _match )

            if( "${_var}" STREQUAL "" )
                continue()
            endif()

            list( APPEND _selvars "${_var}" )

        endforeach()

    endforeach()

    list( SORT _selvars )

    list( REMOVE_DUPLICATES _selvars )

    foreach( _var IN LISTS _selvars )
        if( _msg )
            message( "[[ Removing ${_var} '${${_var}}' " )
        endif()
        unset( ${_var} )
        unset( ${_var} CACHE )
        unset( ${_var} PARENT_SCOPE )
    endforeach()

    return()

endfunction()
