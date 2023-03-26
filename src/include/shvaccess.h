/*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    shvaccess.h,shvaccess.c
    Copyright Enrico Sorichetti 2019 - 2019
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
*/

#pragma once

#ifdef __cplusplus
#define BEGIN_EXTERN_C() extern "C" {
#define END_EXTERN_C() }
#else
#define BEGIN_EXTERN_C()
#define END_EXTERN_C()
#endif

/*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
*/
#include <stdint.h>
#include <sys/types.h>

/*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
*/
BEGIN_EXTERN_C()

uint32_t vGet( char * vName, char * Value, size_t * Length) ;
uint32_t vPut( char * vName, char * Value, size_t Length) ;

END_EXTERN_C()

