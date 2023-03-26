#[[ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    Copyright Enrico Sorichetti 2018 - 2019
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file BOOST_LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
#]]
include_guard( GLOBAL )

#[[ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#]]
unset( CMAKE_C_FLAGS )
unset( CMAKE_C_FLAGS CACHE )
unset( CMAKE_C_FLAGS_INIT )
unset( CMAKE_C_FLAGS_INIT CACHE )

set(   CMAKE_C_FLAGS                        "-g -O2 -DNDEBUG" CACHE STRING "" FORCE )
set(   CMAKE_C_FLAGS_INIT                   "-g -O2 -DNDEBUG" CACHE STRING "" FORCE )

#[[ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#]]
unset( CMAKE_C_FLAGS_RELEASE )
unset( CMAKE_C_FLAGS_RELEASE CACHE )
unset( CMAKE_C_FLAGS_RELEASE_INIT )
unset( CMAKE_C_FLAGS_RELEASE_INIT CACHE )

set(   CMAKE_C_FLAGS_RELEASE                "   -O2 -DNDEBUG" CACHE STRING "" FORCE )
set(   CMAKE_C_FLAGS_RELEASE_INIT           "   -O2 -DNDEBUG" CACHE STRING "" FORCE )

#[[ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#]]
unset( CMAKE_C_FLAGS_RELWITHDEBINFO )
unset( CMAKE_C_FLAGS_RELWITHDEBINFO CACHE )
unset( CMAKE_C_FLAGS_RELWITHDEBINFO_INIT )
unset( CMAKE_C_FLAGS_RELWITHDEBINFO_INIT CACHE )

set(   CMAKE_C_FLAGS_RELWITHDEBINFO         "-g -O2 -DNDEBUG" CACHE STRING "" FORCE )
set(   CMAKE_C_FLAGS_RELWITHDEBINFO_INIT    "-g -O2 -DNDEBUG" CACHE STRING "" FORCE )


#[[ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#]]
unset( CMAKE_C_FLAGS_DEBUG )
unset( CMAKE_C_FLAGS_DEBUG CACHE )
unset( CMAKE_C_FLAGS_DEBUG_INIT )
unset( CMAKE_C_FLAGS_DEBUG_INIT  CACHE )

set(   CMAKE_C_FLAGS_DEBUG                  "-g     -DDEBUG" CACHE STRING "" FORCE )
set(   CMAKE_C_FLAGS_DEBUG_INIT             "-g     -DDEBUG" CACHE STRING "" FORCE )


#[[ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#]]
unset( CMAKE_CXX_FLAGS )
unset( CMAKE_CXX_FLAGS CACHE )
unset( CMAKE_CXX_FLAGS_INIT )
unset( CMAKE_CXX_FLAGS_INIT CACHE )

set(   CMAKE_CXX_FLAGS                      "-g -O2 -DNDEBUG" CACHE STRING "" FORCE )
set(   CMAKE_CXX_FLAGS_INIT                 "-g -O2 -DNDEBUG" CACHE STRING "" FORCE )

#[[ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#]]
unset( CMAKE_CXX_FLAGS_RELEASE )
unset( CMAKE_CXX_FLAGS_RELEASE CACHE )
unset( CMAKE_CXX_FLAGS_RELEASE_INIT )
unset( CMAKE_CXX_FLAGS_RELEASE_INIT CACHE )

set(   CMAKE_CXX_FLAGS_RELEASE              "  -O2  -DNDEBUG" CACHE STRING "" FORCE )
set(   CMAKE_CXX_FLAGS_RELEASE_INIT         "  -O2  -DNDEBUG" CACHE STRING "" FORCE )

#[[ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#]]
unset( CMAKE_CXX_FLAGS_RELWITHDEBINFO )
unset( CMAKE_CXX_FLAGS_RELWITHDEBINFO CACHE )
unset( CMAKE_CXX_FLAGS_RELWITHDEBINFO_INIT )
unset( CMAKE_CXX_FLAGS_RELWITHDEBINFO_INIT CACHE )

set(   CMAKE_CXX_FLAGS_RELWITHDEBINFO       "-g -O2 -DNDEBUG" CACHE STRING "" FORCE )
set(   CMAKE_CXX_FLAGS_RELWITHDEBINFO_INIT  "-g -O2 -DNDEBUG" CACHE STRING "" FORCE )


#[[ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#]]
unset( CMAKE_CXX_FLAGS_DEBUG )
unset( CMAKE_CXX_FLAGS_DEBUG CACHE )
unset( CMAKE_CXX_FLAGS_DEBUG_INIT )
unset( CMAKE_CXX_FLAGS_DEBUG_INIT  CACHE )

set(   CMAKE_CXX_FLAGS_DEBUG                "-g     -DDEBUG" CACHE STRING "" FORCE )
set(   CMAKE_CXX_FLAGS_DEBUG_INIT           "-g     -DDEBUG" CACHE STRING "" FORCE )


