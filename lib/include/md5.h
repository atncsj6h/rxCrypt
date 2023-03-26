/*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    md5.c,md5.h
    Implementation of the MD5 hash
    free and unencumbered, released into the public domain
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

/*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    md5Context - This must be initialised using md5Initialise.
*/
typedef struct
{
    uint32_t     lo;
    uint32_t     hi;
    uint32_t     a;
    uint32_t     b;
    uint32_t     c;
    uint32_t     d;
    uint8_t      buffer[64];
    uint32_t     block[16];
} md5Context;

#define MD5_HASH_SIZE           ( 128 / 8 )

typedef struct
{
    uint8_t      bytes [MD5_HASH_SIZE];
} MD5_HASH;


BEGIN_EXTERN_C()

void    md5Initialise( md5Context* Context ) ;

void    md5Update( md5Context * Context, void  const * Buffer, uint32_t BufferSize ) ;

void    md5Finalise( md5Context * Context, MD5_HASH * Digest );

void    md5( void const * Buffer, uint32_t BufferSize, MD5_HASH * Digest );

END_EXTERN_C()
