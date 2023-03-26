/*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    aes.h
    Implementation of the AES ( Rijndael ) cipher.
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
*/
#define AES_KEY_SIZE_128        16
#define AES_KEY_SIZE_192        24
#define AES_KEY_SIZE_256        32
#define AES_BLOCK_SIZE          16


/*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    aesContext, must be initialised using aesInit
    Do not modify the contents of this structure directly
*/
typedef struct
{
    uint32_t        eK[60];
    uint32_t        dK[60];
    uint_fast32_t   Nr;
} aesContext;


/*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
*/
BEGIN_EXTERN_C()

#define AesInitialise( _aesCtx, _Key, _KeySz ) \
        aesInit( (_aesCtx), (_Key), (_KeySz) )

int     aesInit( aesContext * Context,
            void const * Key, uint32_t KeySize );

#define AesEncrypt( _aesCtx, _iBuf, _oBuf ) \
        aesEncrypt( (_aesCtx), (_oBuf), (_iBuf) )

void    aesEncrypt( aesContext const * Context,
            uint8_t * oBuffer, uint8_t const * iBuffer );

#define AesEncryptInPlace( _aesCtx, _oBuf ) \
        aesEncryptInPlace( (_aesCtx), (_oBuf) )
void    aesEncryptInPlace( aesContext const * Context,
            uint8_t * Buffer );

#define AesDecrypt( _aesCtx, _iBuf, _oBuf ) \
        aesDecrypt( (_aesCtx), (_oBuf), (_iBuf) )

void    aesDecrypt( aesContext const * Context,
            uint8_t * oBuffer, uint8_t const * iBuffer );

#define AesDecryptInPlace( _aesCtx, _oBuf ) \
        aesDecryptInPlace( (_aesCtx), (_oBuf) )

void    aesDecryptInPlace( aesContext const * Context,
            uint8_t * Buffer );

END_EXTERN_C()

