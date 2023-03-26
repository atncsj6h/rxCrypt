/*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    ofb.h
    Implementation of AES OFB stream cipher.
    Depends on: aes.h, aes.c
    AES OFB is a stream cipher using the AES block cipher in output feedback mode.

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
#include "aes.h"

/*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
*/
#define OFB_IV_SIZE AES_BLOCK_SIZE

/*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    ofbContext
    Do not modify the contents of this structure directly.
*/
typedef struct
{
    aesContext      aes;
    uint8_t         CurrentCipherBlock[AES_BLOCK_SIZE];
    uint32_t        IndexWithinCipherBlock;
} ofbContext;

/*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
*/
BEGIN_EXTERN_C()

/*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    ofbInit

    Initialises an ofbContext with an already initialised aesContext and a IV.
    This function can quickly be used to change the IV
    without requiring the more lengthy processes of reinitialising an AES key.
*/
#define AesOfbInitialise( _ofbCtx, _aesCtx, _iv ) \
        ofbInit( (_ofbCtx), (_aesCtx), (_iv) )

void    ofbInit( ofbContext * Context,
            aesContext const * aesContext ,
            uint8_t const IV[OFB_IV_SIZE] );

/*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    ofbInitWithKey

    Initialises an ofbContext with an AES Key and an IV.
    This combines the initialising an AES Context and then running ofbInit.
    KeySize must be 16, 24, or 32 (for 128, 192, or 256 bit key size)
    Returns 0 if successful, or -1 if invalid KeySize provided
*/
#define AesOfbInitWithKey( _ofbCtx, _Key, _KeySz, _iv ) \
        ofbInitWithKey( (_ofbCtx), (_Key), (_KeySz), (_iv) )

int     ofbInitWithKey( ofbContext * Context,
            uint8_t const * Key, uint32_t KeySize,
            uint8_t const IV[OFB_IV_SIZE] ) ;

/*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    ofbXor

    XORs the stream of byte of the ofbContext from its current stream position
    onto the specified buffer.
    This will   advance the stream index by that number of bytes.
    Use once over data to encrypt it.
    Use it a second time over the same data from the same stream position
    and the data will be decrypted.
    iBuffer and oBuffer can point to the same location for in-place operation
*/
#define AesOfbXor( _ofbCtx, _iBuf, _oBuf, _bufSz ) \
        ofbXor( (_ofbCtx), (_oBuf), (_iBuf), (_bufSz) )

void    ofbXor( ofbContext * Context,
        void * oBuffer, void const * iBuffer, uint32_t BufferSize );

/*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    ofbOutput

    Outputs the stream of byte of the ofbContext from its current stream position.
    This will advance the stream index by that number of bytes.
*/
#define AesOfbOutput( _ofbCtx, _oBuf, _bufSz) \
        ofbOutput( (_ofbCtx), (_oBuf), (_bufSz) )

void    ofbOutput( ofbContext * Context,
            void * Buffer, uint32_t BufferSize );

/*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    ofbXorWithKey

    This function combines aesOfbInitWithKey and aesOfbXor.
    This is suitable when encrypting/decypting data in one go with a key
    that is not going to be reused.
    This will used the provided Key and IV and generate a stream
    that is XORed over Buffer.
    iBuffer and oBuffer can point to the same location for inplace operation

    Returns 0 if successful, or -1 if invalid KeySize provided
*/
#define AesOfbXorWithKey( _Key, _KeySz, _iv, _iBuf, _oBuf, _bufSz ) \
        ofbXorWithKey( (_oBuf), (_iBuf), (_bufSz), (_Key), (_KeySz), (_iv) )

int     ofbXorWithKey(void * oBuffer,
            void const * iBuffer, uint32_t BufferSize ,
            uint8_t const * Key, uint32_t KeySize,
            uint8_t const IV[OFB_IV_SIZE] );

/*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
*/
END_EXTERN_C()
