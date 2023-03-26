/*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    sha1.c,sha1.h
    Implementation of the SHA1 hash
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
#include <sys/types.h>

/*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    sha1Context - This must be initialised using sha1Initialise.
*/
typedef struct
{
    uint32_t        State[5];
    uint32_t        Count[2];
    uint8_t         Buffer[64];
} sha1Context;

#define SHA1_HASH_SIZE           ( 160 / 8 )

typedef struct
{
    uint8_t         bytes[SHA1_HASH_SIZE];
} SHA1_HASH;


BEGIN_EXTERN_C()

void    sha1Initialise( sha1Context* Context ) ;

void    sha1Update( sha1Context* Context, void  const* Buffer, uint32_t BufferSize ) ;

void    sha1Finalise( sha1Context* Context, SHA1_HASH* Digest );

void    sha1( void const* Buffer, uint32_t BufferSize, SHA1_HASH* Digest );

END_EXTERN_C()
