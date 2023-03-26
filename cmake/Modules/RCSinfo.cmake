#[[ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    Copyright Enrico Sorichetti 2018 - 2019
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file BOOST_LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
#]]
include_guard( GLOBAL )

#[[ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#]]
macro( RC_check _pgm )
    if( "${_res}" STREQUAL 0 )
    else()
        message( FATAL_ERROR "
 Command '${_pgm}' in '${CMAKE_SOURCE_DIR}' failed with error:
 ${_err} " )
    endif()
endmacro()

#[[ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    # the commands should be executed with the C locale,
    # otherwise the messages (which are parsed) might be translated
    set( _lc_all "$ENV{LC_ALL}" )
    set( ENV{LC_ALL} "C" )
    execute_process ... ...
    set( ENV{LC_ALL} "${_lc_all}" )
#]]

#[[ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#]]
set( RCS_NAME "" )
set( RCS_VERSION "" )
set( RCS_WC_REVISION "Final" )
set( RCS_WC_COMMITS "0" )

#[[ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    Subversion
#]]
if( EXISTS ${CMAKE_SOURCE_DIR}/.svn )
    find_package( Svn )
    if( SVN_FOUND )
        set( RCS_NAME "Subversion" )
        set( RCS_VERSION "${SVN_VERSION_STRING}" )

#[[ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    the revision
#]]
        set( _lc_all "$ENV{LC_ALL}" )
        set( ENV{LC_ALL} "C" )
        execute_process( COMMAND ${SVN_EXECUTABLE} info --show-item last-changed-revision
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            RESULT_VARIABLE _res
            ERROR_VARIABLE  _err
            OUTPUT_VARIABLE RCS_WC_COMMITS
            OUTPUT_STRIP_TRAILING_WHITESPACE )
        set( ENV{LC_ALL} "${_lc_all}" )

        RC_check( "${SVN_EXECUTABLE}" )

#[[ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    the dirty flag
#]]
        set( _lc_all "$ENV{LC_ALL}" )
        set( ENV{LC_ALL} "C" )
        execute_process( COMMAND ${SVN_EXECUTABLE} diff --summarize
         WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            RESULT_VARIABLE _res
            ERROR_VARIABLE  _err
            OUTPUT_VARIABLE _out
            OUTPUT_STRIP_TRAILING_WHITESPACE )
        set( ENV{LC_ALL} "${_lc_all}" )

        RC_check( "${SVN_EXECUTABLE}" )

        if( "${_out}" STREQUAL "" )
            set( RCS_WC_REVISION "Svn[${RCS_WC_COMMITS}]" )
        else()
            set( RCS_WC_REVISION "Svn[${RCS_WC_COMMITS}+]" )
        endif()

#[[ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#]]

    endif( SVN_FOUND )

    return()

endif( EXISTS ${CMAKE_SOURCE_DIR}/.svn )

#[[ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    Mercurial
#]]
if( EXISTS ${CMAKE_SOURCE_DIR}/.hg )
    find_package( Hg )
    if( HG_FOUND )
        set( RCS_NAME "Mercurial" )
        set( RCS_VERSION "${HG_VERSION_STRING}" )

#[[ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    the commit count
#]]
        set( _lc_all "$ENV{LC_ALL}" )
        set( ENV{LC_ALL} "C" )
        execute_process( COMMAND ${HG_EXECUTABLE} id -n
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            RESULT_VARIABLE _res
            ERROR_VARIABLE  _err
            OUTPUT_VARIABLE RCS_WC_COMMITS
            OUTPUT_STRIP_TRAILING_WHITESPACE )
        set( ENV{LC_ALL} "${_lc_all}" )
        RC_check( "${HG_EXECUTABLE}" )

        string( REGEX MATCH "([^\\+]*)" RCS_WC_COMMITS ${RCS_WC_COMMITS} )

#[[ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    the revision  (hash )
#]]
        set( _lc_all "$ENV{LC_ALL}" )
        set( ENV{LC_ALL} "C" )
        execute_process( COMMAND ${HG_EXECUTABLE} id -i
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            RESULT_VARIABLE _res
            ERROR_VARIABLE  _err
            OUTPUT_VARIABLE RCS_WC_REVISION
            OUTPUT_STRIP_TRAILING_WHITESPACE )
        set( ENV{LC_ALL} "${_lc_all}" )
        RC_check( "${HG_EXECUTABLE}" )

        string( REGEX MATCH "([^\\+]*)" RCS_WC_REVISION ${RCS_WC_REVISION} )

#[[ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    the dirty flag
#]]
        set( _lc_all "$ENV{LC_ALL}" )
        set( ENV{LC_ALL} "C" )
        execute_process( COMMAND ${HG_EXECUTABLE} status
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            RESULT_VARIABLE _res
            ERROR_VARIABLE  _err
            OUTPUT_VARIABLE _out
            OUTPUT_STRIP_TRAILING_WHITESPACE )
        set( ENV{LC_ALL} "${_lc_all}" )
        RC_check( "${GIT_EXECUTABLE}" )

        if( "${_out}" STREQUAL "" )
            set( RCS_WC_REVISION "Hg[${RCS_WC_REVISION}]" )
        else()
            set( RCS_WC_REVISION "Hg[${RCS_WC_REVISION}+]" )
        endif()

#[[ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#]]

    endif( HG_FOUND )

    return()

endif( EXISTS ${CMAKE_SOURCE_DIR}/.hg )

#[[ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    Git
#]]
if( EXISTS ${CMAKE_SOURCE_DIR}/.git )
    find_package( Git )
    if( GIT_FOUND )
        set( RCS_NAME "Git" )
        set( RCS_VERSION "${GIT_VERSION_STRING}" )

#[[ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    the commit count
#]]
        set( _lc_all "$ENV{LC_ALL}" )
        set( ENV{LC_ALL} "C" )
        execute_process( COMMAND ${GIT_EXECUTABLE} rev-list HEAD --count
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            RESULT_VARIABLE _res
            ERROR_VARIABLE  _err
            OUTPUT_VARIABLE _out
            OUTPUT_STRIP_TRAILING_WHITESPACE )
        set( ENV{LC_ALL} "${_lc_all}" )
        RC_check( "${GIT_EXECUTABLE}" )

        set( RCS_WC_COMMITS "${_out}" )

#[[ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    the revision  (hash )
#]]
        set( _lc_all "$ENV{LC_ALL}" )
        set( ENV{LC_ALL} "C" )
        execute_process( COMMAND ${GIT_EXECUTABLE} log -1 --pretty=format:%H
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            RESULT_VARIABLE _res
            ERROR_VARIABLE  _err
            OUTPUT_VARIABLE _out
            OUTPUT_STRIP_TRAILING_WHITESPACE )
        set( ENV{LC_ALL} "${_lc_all}" )
        RC_check( "${GIT_EXECUTABLE}" )

        string( SUBSTRING "${_out}" 0  12 RCS_WC_REVISION )

#[[ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    the dirty flag
#]]
        set( _lc_all "$ENV{LC_ALL}" )
        set( ENV{LC_ALL} "C" )
        execute_process( COMMAND ${GIT_EXECUTABLE} status --porcelain
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            RESULT_VARIABLE _res
            ERROR_VARIABLE  _err
            OUTPUT_VARIABLE _out
            OUTPUT_STRIP_TRAILING_WHITESPACE )
        set( ENV{LC_ALL} "${_lc_all}" )
        RC_check( "${GIT_EXECUTABLE}" )

        if( "${_out}" STREQUAL "" )
            set( RCS_WC_REVISION "Git[${RCS_WC_REVISION}]" )
        else()
            set( RCS_WC_REVISION "Git[${RCS_WC_REVISION}+]" )
        endif()

#[[ - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#]]

    endif( GIT_FOUND )

    return()

endif( EXISTS ${CMAKE_SOURCE_DIR}/.git )


