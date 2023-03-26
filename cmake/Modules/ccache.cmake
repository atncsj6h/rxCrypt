#[[ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    Copyright Enrico Sorichetti 2018 - 2019
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file BOOST_LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
#]]
include_guard( GLOBAL )

#[[ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#]]
option( WITH_CCACHE "Use ccache to speed up builds" ON )

#[[ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#]]
set( _generators "Unix Makefiles" "Ninja" )
if( "${CMAKE_GENERATOR}" IN_LIST _generators  )
    if( WITH_CCACHE )
        find_program(CCACHE ccache)
        if( CCACHE )
            set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE}")
        else()
            message( WARNING "
 CCACHE requested, but 'ccache' program not found " )
        endif()
    endif()
endif()

