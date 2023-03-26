/*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    cbc.h cbc.c

    Implementation of AES( Rijndael ) CBC cipher.
    Depends on: aes.h, aes.c

    AES CBC is a cipher using AES in Cipher Block Chaining mode.
    Encryption and decryption must be performed in multiples
        of the AES block size (128 bits).

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
#define CBC_IV_SIZE             AES_BLOCK_SIZE


// cbcContext
// Do not modify the contents of this structure directly.
typedef struct
{
    aesContext      aes;
    uint8_t         PreviousCipherBlock[AES_BLOCK_SIZE];
} cbcContext;

/*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
*/
BEGIN_EXTERN_C()

/*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    cbcInit

    Initialises a cbcContext with an already initialised aesContext and a IV.
    This function can quickly be used to change the IV without requiring
    the more lengthy processes of reinitialising an AES key.
*/
#define AesCbcInitialise( _cbcCtx, _aesCtx, _iv ) \
        cbcInit( (_cbcCtx), (_aesCtx), (_iv) )

void    cbcInit( cbcContext * Context,
        aesContext const * aesContext,
        uint8_t const IV[CBC_IV_SIZE] );

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  cbcInitWithKey
//
//  Initialises an cbcContext with an AES Key and an IV. This combines the initialising an AES Context and then
//  running cbcInit. KeySize must be 16, 24, or 32 (for 128, 192, or 256 bit key size)
//  Returns 0 if successful, or -1 if invalid KeySize provided
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define AesCbcInitialiseWithKey( _cbcCtx, _Key, _KeySz, _iv ) \
        cbcInitWithKey( (_cbcCtx), (_Key), (_KeySz), (_iv) )

int     cbcInitWithKey( cbcContext* Context,
            uint8_t const * Key, uint32_t const KeySize,
            uint8_t const IV[CBC_IV_SIZE] );

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  cbcEncrypt
//
//  Encrypts a buffer of data using an AES CBC context. The data buffer must be a multiple of 16 bytes (128 bits)
//  in size. The "position" of the context will be advanced by the buffer amount. A buffer can be encrypted in one
//  go or in smaller chunks at a time. The result will be the same as long as data is fed into the function in the
//  same order.
//  iBuffer and oBuffer can point to the same location for in-place encrypting.
//  Returns 0 if successful, or -1 if bufferSize is not a multiple of 16 bytes.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define AesCbcEncrypt( _cbcCtx, _iBuf, _oBuf, _bufSz ) \
        cbcEncrypt( (_cbcCtx), (_oBuf), (_iBuf), (_bufSz) )

int     cbcEncrypt( cbcContext * Context,
            void * oBuffer, void const * iBuffer, uint32_t bufferSize );

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  cbcDecrypt
//
//  Decrypts a buffer of data using an AES CBC context. The data buffer must be a multiple of 16 bytes (128 bits)
//  in size. The "position" of the context will be advanced by the buffer amount.
//  iBuffer and oBuffer can point to the same location for in-place decrypting.
//  Returns 0 if successful, or -1 if bufferSize is not a multiple of 16 bytes.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define AesCbcDecrypt( _cbcCtx, _iBuf, _oBuf, _bufSz ) \
        cbcDecrypt( (_cbcCtx), (_oBuf), (_iBuf), (_bufSz) )

int     cbcDecrypt( cbcContext * Context,
            void * oBuffer, void const * iBuffer, uint32_t bufferSize );

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  cbcEncryptWithKey
//
//  This function combines cbcInitWithKey and cbcEncrypt. This is suitable when encrypting data in one go
//  with a key that is not going to be reused.
//  iBuffer and oBuffer can point to the same location for inplace encrypting.
//  Returns 0 if successful, or -1 if invalid KeySize provided or BufferSize not a multiple of 16 bytes.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define AesCbcEncryptWithKey( _Key, _KeySz, _iv, _iBuf, _oBuf, _bufSz ) \
        cbcEncryptWithKey( (_oBuf), (_iBuf), (_bufSz), (_Key), (_KeySz), (_iv) )

int     cbcEncryptWithKey( void * oBuffer,
        void const * iBuffer, uint32_t BufferSize,
        uint8_t const * Key, uint32_t KeySize,
        uint8_t const IV[CBC_IV_SIZE]);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  cbcDecryptWithKey
//
//  This function combines cbcInitWithKey and cbcDecrypt. This is suitable when decrypting data in one go
//  with a key that is not going to be reused.
//  iBuffer and oBuffer can point to the same location for inplace decrypting.
//  Returns 0 if successful, or -1 if invalid KeySize provided or BufferSize not a multiple of 16 bytes.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define AesCbcDecryptWithKey( _Key, _KeySz, _iv, _iBuf, _oBuf, _bufSz ) \
        cbcDecryptWithKey( (_oBuf), (_iBuf), (_bufSz), (_Key), (_KeySz), (_iv) )

int     cbcDecryptWithKey( void * oBuffer,
        void const * iBuffer, uint32_t BufferSize,
        uint8_t const * Key, uint32_t KeySize,
        uint8_t const IV[CBC_IV_SIZE] );

END_EXTERN_C()
