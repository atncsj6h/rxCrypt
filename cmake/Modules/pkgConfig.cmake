#[[ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    Copyright Enrico Sorichetti 2018 - 2019
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file BOOST_LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
#]]
include_guard( GLOBAL )

#[[ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#]]
set( PKG_CONFIG_EXECUTABLE "$ENV{PKG_CONFIG_EXECUTABLE}" )
if( NOT PKG_CONFIG_EXECUTABLE )
    set( PKG_CONFIG_EXECUTABLE "pkgconf" )
endif()

unset( PKG_CONFIG_FOUND )
unset( PKG_CONFIG_FOUND CACHE )
include( FindPkgConfig )
if( NOT PKG_CONFIG_FOUND )
    message( WARNING "
 pkgconf/pkg-config not available/installed " )
endif()

#[[ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#]]
function( pkg_config)
    string( TOLOWER "${ARGV}" args )

    list( REMOVE_DUPLICATES args )
    list( FIND args "required" indx )
    if( indx GREATER_EQUAL 0 )
        list( REMOVE_AT args "${indx}" )
        set( required "1" )
    endif()

    foreach( argv ${args} )
        pkg_check_modules( "${argv}" "${argv}" )
        if( required AND NOT ${argv}_FOUND )
            message( SEND_ERROR "
 package/library ${argv} not available/installed " )
        endif()
    endforeach()
endfunction()
