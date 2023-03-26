/*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    Copyright Enrico Sorichetti 2019 - 2019
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
*/

/*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
*/
#include <stdlib.h>

#include <stdint.h>
#include <sys/types.h>
#include <strings.h>

#include <stdio.h>

/*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
*/
#include "oorexxapi.h"

/*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
*/
#include "shvaccess.h"

/*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
*/
#define SD(_s) context->StringData(_s)
#define SL(_s) context->StringLength(_s)

/*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
*/
#include "aes.h"
#include "ecb.h"
#include "cbc.h"
#include "ctr.h"
#include "ofb.h"

#include "md5.h"
#include "sha1.h"
#include "sha2.h"

/*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
*/
char    RC[] = "RC";
char    ERROR[] = "ERRROR";

/*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
*/
RexxRoutine0( RexxObjectPtr, rxcryptVersion )
{
    return (RexxObjectPtr)context->NewStringFromAsciiz(VERSION_STRING);
}

#ifdef ENABLE_CSRAND
/*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
*/
RexxRoutine1(uint32_t, rxcsrand, OPTIONAL_uint64_t, Bytes)
{
    printf("Bytes %llu\n", Bytes);

    return arc4random();
    return Bytes;
}
#endif

/*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
*/
RexxRoutine3( RexxObjectPtr, rxaes, RexxStringObject, Action,
    RexxStringObject, iBuffer, RexxStringObject, Key )
{
aesContext  aesContext;
int     rc;
char    rcChar[16];

    uint8_t oBuffer[AES_BLOCK_SIZE];

    switch ( (uint8_t)SL(Key) )
    {
        case AES_KEY_SIZE_128 :
        case AES_KEY_SIZE_192 :
        case AES_KEY_SIZE_256 :
            break;
        default :
            context->InvalidRoutine();
            return NULLOBJECT;
    }
    if( (uint32_t)SL(iBuffer) != AES_BLOCK_SIZE )
    {
        context->InvalidRoutine();
        return NULLOBJECT;
    }

    aesInit( &aesContext,
        SD(Key), (uint32_t)SL(Key) );

    if ( ! strncasecmp( "Encrypt", SD(Action), SL(Action)) )
    {
        aesEncrypt( &aesContext,
            oBuffer, (uint8_t *)SD(iBuffer) );
        return (RexxObjectPtr)context->NewString((char *)oBuffer, AES_BLOCK_SIZE);
    }

    if ( ! strncasecmp( "Decrypt", SD(Action), SL(Action)) )
    {
        aesDecrypt( &aesContext,
            oBuffer, (uint8_t *)SD(iBuffer) );
        return (RexxObjectPtr)context->NewString((char *)oBuffer, AES_BLOCK_SIZE);
    }

    context->InvalidRoutine();
    return NULLOBJECT;
}

/*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
*/
RexxRoutine3( RexxObjectPtr, rxecb, RexxStringObject, Action,
    RexxStringObject, iBuffer, RexxStringObject, Key )
{
aesContext  ecbContext;
uint8_t * oBuffer;
RexxStringObject Result ;

int     rc;
char    rcChar[16];

    switch ( (uint8_t)SL(Key) )
    {
        case AES_KEY_SIZE_128 :
        case AES_KEY_SIZE_192 :
        case AES_KEY_SIZE_256 :
            break;
        default :
            context->InvalidRoutine();
            return NULLOBJECT;
    }
    if( ( (uint32_t)SL(iBuffer) % AES_BLOCK_SIZE ) != 0  )
    {
        sprintf(rcChar, "%d", -1002);
        vPut( RC, rcChar, strlen(rcChar) );
        return (RexxObjectPtr)context->NullString();
    }

    aesInit( &ecbContext,
        SD(Key), (uint32_t)SL(Key) );

    oBuffer = (uint8_t * )malloc(SL(iBuffer)+16);

    if ( ! strncasecmp( "Encrypt", SD(Action), SL(Action)) )
    {
        rc = ecbEncrypt( &ecbContext,
            oBuffer, (uint8_t *)SD(iBuffer), (uint32_t)SL(iBuffer) ) ;
        if ( rc != 0  )
        {
            free(oBuffer);
            context->InvalidRoutine();
            return NULLOBJECT;
        }
        Result = context->NewString((char *)oBuffer, (uint32_t)SL(iBuffer) );
        free(oBuffer) ;
        return Result ;
    }

    if ( ! strncasecmp( "Decrypt", SD(Action), SL(Action)) )
    {
        rc = ecbDecrypt( &ecbContext,
            oBuffer, (uint8_t *)SD(iBuffer), (uint32_t)SL(iBuffer) ) ;
        if ( rc != 0  )
        {
            free(oBuffer);
            context->InvalidRoutine();
            return NULLOBJECT;
        }
        Result = context->NewString((char *)oBuffer, (uint32_t)SL(iBuffer) );
        free(oBuffer) ;
        return Result ;
    }

    free(oBuffer) ;
    context->InvalidRoutine();
    return NULLOBJECT;
}


/*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
*/
RexxRoutine4( RexxObjectPtr, rxcbc, RexxStringObject, Action ,
    RexxStringObject, iBuffer, RexxStringObject, Key, RexxStringObject, IV )
{
cbcContext  cbcContext;
uint8_t * oBuffer;
RexxStringObject Result ;

int     rc;
char    rcChar[16];

    switch ( (uint8_t)SL(Key) )
    {
        case AES_KEY_SIZE_128 :
        case AES_KEY_SIZE_192 :
        case AES_KEY_SIZE_256 :
            break;
        default :
            context->InvalidRoutine();
            return NULLOBJECT;
    }

    if( ( (uint32_t)SL(iBuffer) % AES_BLOCK_SIZE ) != 0  )
    {
        context->InvalidRoutine();
        return NULLOBJECT;
    }

    rc = cbcInitWithKey( &cbcContext,
             (uint8_t *)SD(Key), (uint32_t)SL(Key),
             (uint8_t *)SD(IV) );
    if ( rc != 0 )
    {
        context->InvalidRoutine();
        return NULLOBJECT;
    }

    oBuffer = (uint8_t * )malloc(SL(iBuffer)+16);

    if ( ! strncasecmp( "Encrypt", SD(Action), SL(Action)) )
    {

        rc = cbcEncrypt( &cbcContext,
                  oBuffer, (uint8_t *)SD(iBuffer), (uint32_t)SL(iBuffer) ) ;
        if ( rc != 0  )
        {
            free(oBuffer);
            context->InvalidRoutine();
            return NULLOBJECT;
        }

        Result = context->NewString((char *)oBuffer, (uint32_t)SL(iBuffer) );
        free(oBuffer) ;
        return Result ;
    }

    if ( ! strncasecmp( "Decrypt", SD(Action), SL(Action)) )
    {
        rc = cbcDecrypt( &cbcContext,
                oBuffer, (uint8_t *)SD(iBuffer), (uint32_t)SL(iBuffer) ) ;
        if ( rc != 0  )
        {
            free(oBuffer);
            context->InvalidRoutine();
            return NULLOBJECT;
        }

        Result = context->NewString((char *)oBuffer, (uint32_t)SL(iBuffer) );
        free(oBuffer) ;
        return Result ;
    }

    free(oBuffer);
    context->InvalidRoutine();
    return NULLOBJECT;
}

/*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
*/
RexxRoutine3( RexxObjectPtr, rxofb,
    RexxStringObject, iBuffer, RexxStringObject, Key, RexxStringObject, IV )
{
ofbContext  ofbContext;
uint8_t * oBuffer;
RexxStringObject Result ;

int     rc;
char    rcChar[16];

    switch ( (uint8_t)SL(Key) )
    {
        case AES_KEY_SIZE_128 :
        case AES_KEY_SIZE_192 :
        case AES_KEY_SIZE_256 :
            break;
        default :
            context->InvalidRoutine();
            return NULLOBJECT;
    }

    oBuffer = (uint8_t * )malloc(SL(iBuffer)+16);

    ofbXorWithKey( oBuffer,
        (uint8_t *)SD(iBuffer), (uint32_t)SL(iBuffer) ,
        (uint8_t *)SD(Key), (uint32_t)SL(Key),
        (uint8_t *)SD(IV) );

    Result = context->NewString((char *)oBuffer, (uint32_t)SL(iBuffer) );
    free(oBuffer) ;
    return Result ;
}


/*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
*/
RexxRoutine1(RexxObjectPtr, rxmd5, RexxStringObject, Input)
{
    md5Context  md5Context;
    MD5_HASH    md5Hash ;

    md5Initialise( &md5Context );
    md5Update( &md5Context, SD(Input), (uint32_t)SL(Input) );
    md5Finalise( &md5Context, &md5Hash );

    return (RexxObjectPtr)context->NewString((char *)md5Hash.bytes, MD5_HASH_SIZE);
}

/*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
*/
RexxRoutine1(RexxObjectPtr, rxsha1, RexxStringObject, Input)
{
    sha1Context sha1Context;
    SHA1_HASH   sha1Hash ;

    sha1Initialise( &sha1Context );
    sha1Update( &sha1Context, SD(Input), (uint32_t)SL(Input) );
    sha1Finalise( &sha1Context, &sha1Hash );

    return (RexxObjectPtr)context->NewString((char *)sha1Hash.bytes, SHA1_HASH_SIZE);
}

/*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
*/
RexxRoutine1(RexxObjectPtr, rxsha256, RexxStringObject, Input)
{
    sha256Context sha256Context;
    SHA256_HASH    sha256Hash ;

    sha256Initialise( &sha256Context );
    sha256Update( &sha256Context, SD(Input), (uint32_t)SL(Input) );
    sha256Finalise( &sha256Context );
    sha256Last( &sha256Context, &sha256Hash );

    return (RexxObjectPtr)context->NewString((char *)sha256Hash.bytes, SHA256_HASH_SIZE);
}

#ifdef ENABLE_SHA224
/*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
*/
RexxRoutine1(RexxObjectPtr, rxsha224, RexxStringObject, Input)
{

    sha256Context sha224Context;
    SHA224_HASH   sha224Hash ;

    sha224Initialise( &sha224Context );
    sha256Update( &sha224Context, SD(Input), (uint32_t)SL(Input) );
    sha256Finalise( &sha224Context );
    sha224Last( &sha224Context, &sha224Hash );

    return (RexxObjectPtr)context->NewString((char *)sha224Hash.bytes, SHA224_HASH_SIZE);
}
#endif

/*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
*/
RexxRoutine1(RexxObjectPtr, rxsha512, RexxStringObject, Input)
{
    sha512Context sha512Context;
    SHA512_HASH   sha512Hash ;

    sha512Initialise( &sha512Context );
    sha512Update( &sha512Context, SD(Input), (uint32_t)SL(Input) );
    sha512Finalise( &sha512Context );
    sha512Last( &sha512Context, &sha512Hash );

    return (RexxObjectPtr)context->NewString((char *)sha512Hash.bytes, SHA512_HASH_SIZE);
}

#ifdef ENABLE_SHA384
/*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
*/
RexxRoutine1(RexxObjectPtr, rxsha384, RexxStringObject, Input)
{

    sha512Context sha384Context;
    SHA384_HASH   sha384Hash ;

    sha384Initialise( &sha384Context );
    sha512Update( &sha384Context, SD(Input), (uint32_t)SL(Input) );
    sha512Finalise( &sha384Context );
    sha384Last( &sha384Context, &sha384Hash );

    return (RexxObjectPtr)context->NewString((char *)sha384Hash.bytes, SHA384_HASH_SIZE);
}
#endif

/*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
*/
RexxRoutineEntry rxcryptRoutines[] =
{
    REXX_TYPED_ROUTINE(cryptVersion,  rxcryptVersion ),

#ifdef  ENABLE_CSRAND
    REXX_TYPED_ROUTINE(csrand,          rxcsrand ),
#endif

    REXX_TYPED_ROUTINE(aes,             rxaes  ),
    REXX_TYPED_ROUTINE(ecb,             rxecb  ),
    REXX_TYPED_ROUTINE(cbc,             rxcbc  ),

    REXX_TYPED_ROUTINE(ofb,             rxofb  ),

    REXX_TYPED_ROUTINE(md5,             rxmd5     ),
    REXX_TYPED_ROUTINE(sha1,            rxsha1    ),
    REXX_TYPED_ROUTINE(sha256,          rxsha256  ),
#ifdef  ENABLE_SHA224
    REXX_TYPED_ROUTINE(sha224,          rxsha224 ),
#endif

    REXX_TYPED_ROUTINE(sha512,          rxsha512  ),
#ifdef  ENABLE_SHA384
    REXX_TYPED_ROUTINE(sha384,          rxsha384 ),
#endif

    REXX_LAST_ROUTINE()
};

/*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
*/
RexxPackageEntry rxcrypt_package_entry =
{
    STANDARD_PACKAGE_HEADER
    REXX_INTERPRETER_5_0_0,             // anything after 4.0.0 will work
    "rxcrypt",                          // name of the package
    "1.0.0",                            // package information
    NULL,                               // no loader function (run once at load time)
    NULL,                               // no unloader function (run once at shutdown time)
    rxcryptRoutines,                    // the exported native routines
    NULL                                // no native methods

};

/*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
*/
OOREXX_GET_PACKAGE(rxcrypt);

