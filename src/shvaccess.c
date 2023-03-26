/*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    shvaccess.h,shvaccess.c
    Copyright Enrico Sorichetti 2019 - 2019
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
*/

/*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
*/
#include <stdint.h>
#include <sys/types.h>
#include <strings.h>

/*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
*/
#include <oorexxapi.h>

/*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
*/
uint32_t vGet( char * vName, char * Value, size_t * Length )
{

SHVBLOCK    vBlock;
int32_t     rc;

    /* initialize the shared variable block */
    vBlock.shvnext = NULL;
    vBlock.shvname.strptr = vName;
    vBlock.shvname.strlength = strlen(vName);
    vBlock.shvnamelen = vBlock.shvname.strlength;
    vBlock.shvvalue.strptr = NULL;
    vBlock.shvvalue.strlength = 0;
    vBlock.shvvaluelen = vBlock.shvvalue.strlength;
    vBlock.shvcode = RXSHV_SYFET;
    vBlock.shvret = RXSHV_OK;

    /* fetch variable from pool */
    rc = RexxVariablePool(&vBlock);

    /* test return code */
    if (rc == RXSHV_OK | rc == RXSHV_NEWV)
    {
        if ( vBlock.shvvalue.strlength > *Length )
        {
            rc = RXSHV_MEMFL;
        }
        else
        {
            /* copy to new buffer and zero-terminate */
            *Length = vBlock.shvvalue.strlength ;
            memcpy( Value, vBlock.shvvalue.strptr, vBlock.shvvalue.strlength);
            Value[(int)vBlock.shvvalue.strlength] = '\0' ;
            rc = RXSHV_OK;
        }
    }
    return rc;
}

/*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
*/
uint32_t vPut( char * vName, char * Value, size_t Length )
{

/* local function data */
SHVBLOCK    vBlock;
int32_t     rc;

    /* initialize the shared variable block */
    vBlock.shvnext = NULL;
    vBlock.shvname.strptr = vName;
    vBlock.shvname.strlength = strlen(vName);
    vBlock.shvnamelen = vBlock.shvname.strlength;
    vBlock.shvvalue.strptr = Value;
    vBlock.shvvalue.strlength = Length;
    vBlock.shvvaluelen = vBlock.shvvalue.strlength;
    vBlock.shvcode = RXSHV_SYSET;
    vBlock.shvret = RXSHV_OK;

    /* set variable in pool */
    rc = RexxVariablePool(&vBlock);

    /* set the return code */
    if (rc == RXSHV_NEWV)
    {
        rc = RXSHV_OK;
    }

    return rc;
}
