/*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    sha2.h,sha2.c
    Implementation of the SHA256,SHA512,SHA384 hash(es)
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
*/
#define SHA256_BLOCK_SIZE          64
#define SHA256_HASH_SIZE           ( 256 / 8 )

#ifdef  ENABLE_SHA224
#define SHA224_BLOCK_SIZE          64
#define SHA224_HASH_SIZE           ( 224 / 8 )
#endif

#define SHA512_BLOCK_SIZE          128
#define SHA512_HASH_SIZE           ( 512 / 8 )

#ifdef  ENABLE_SHA384
#define SHA384_BLOCK_SIZE          128
#define SHA384_HASH_SIZE           ( 384 / 8 )
#endif

/*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
*/
BEGIN_EXTERN_C()

/*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    sha256
*/
typedef struct{
    uint64_t    length;
    uint32_t    state[8];
    uint32_t    curlen;
    uint8_t     buf[SHA256_BLOCK_SIZE];
} sha256Context;

typedef struct {
    uint8_t      bytes [SHA256_HASH_SIZE];
} SHA256_HASH;

void    sha256Initialise( sha256Context * Context ) ;
void    sha256Update( sha256Context * Context, void  const * Buffer, uint32_t BufferSize ) ;
void    sha256Finalise( sha256Context * Context);
void    sha256Last( sha256Context * Context, SHA256_HASH * Digest );


#ifdef  ENABLE_SHA224
/*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    sha224
    NOTE:
    since sha256Context and sha224Context have the same layout
    no need to define the sha224Context structure
*/

typedef struct{
    uint8_t      bytes [SHA224_HASH_SIZE];
} SHA224_HASH;

void    sha224Initialise( sha256Context * Context ) ;
void    sha224Update( sha256Context * Context, void  const* Buffer, uint32_t BufferSize ) ;
void    sha224Finalise( sha256Context * Context );
void    sha224Last( sha256Context * Context, SHA224_HASH * Digest );
#endif

/*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    sha512
*/
typedef struct{
    uint64_t    length;
    uint64_t    state[8];
    uint32_t    curlen;
    uint8_t     buf[SHA512_BLOCK_SIZE];
} sha512Context;

typedef struct{
    uint8_t      bytes [SHA512_HASH_SIZE];
} SHA512_HASH;

void    sha512Initialise( sha512Context * Context ) ;
void    sha512Update( sha512Context * Context, void  const * Buffer, uint32_t BufferSize ) ;
void    sha512Finalise( sha512Context * Context );
void    sha512Last( sha512Context * Context, SHA512_HASH * Digest );

#ifdef  ENABLE_SHA384
/*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    sha384
    NOTE:
    since sha512Context and sha384Context have the same layout
    no need to define the sha384Context structure
*/

typedef struct{
    uint8_t      bytes [SHA384_HASH_SIZE];
} SHA384_HASH;

void    sha384Initialise( sha512Context * Context ) ;
void    sha384Update( sha512Context * Context, void  const* Buffer, uint32_t BufferSize ) ;
void    sha384Finalise( sha512Context * Context );
void    sha384Last( sha512Context * Context, SHA384_HASH * Digest );
#endif


/*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
*/
END_EXTERN_C()
