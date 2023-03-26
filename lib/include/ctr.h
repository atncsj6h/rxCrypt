////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  WjCryptLib_ctr
//
//  Implementation of AES CTR stream cipher.
//
//  Depends on: CryptoLib_Aes
//
//  AES CTR is a stream cipher using the AES block cipher in counter mode.
//  This implementation works on both little and big endian architectures.
//
//  This is free and unencumbered software released into the public domain - November 2017 waterjuice.org
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
#define CTR_IV_SIZE             8

/*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    ctrContext
    Do not modify directly the contents of this structure .
*/
typedef struct
{
    aesContext      aes;
    uint8_t         IV [CTR_IV_SIZE];
    uint64_t        StreamIndex;
    uint64_t        CurrentCipherBlockIndex;
    uint8_t         CurrentCipherBlock [AES_BLOCK_SIZE];
} ctrContext;

/*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
*/

BEGIN_EXTERN_C()

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  ctrInit
//
//  Initialises a ctrContext with an already initialised aesContext and a IV.
//  This function can be used to change quickly the IV
//  without requiring the more length processes of reinitialising an AES key.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void    ctrInit( ctrContext * Context,
            aesContext const * aesContextInitialised,
            uint8_t const IV[CTR_IV_SIZE] );

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  ctrInitWithKey
//
//  Initialises a ctrContext with an AES Key and an IV.
//  This combines the initialising an AES Context and then running ctrInit.
//  KeySize must be 16, 24, or 32 (for 128, 192, or 256 bit key size)
//  Returns 0 if successful, or -1 if invalid KeySize provided
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int     ctrInitWithKey( ctrContext * Context,
            uint8_t const * Key, uint32_t KeySize,
            uint8_t const IV[CTR_IV_SIZE] );

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  ctrSetStreamIndex
//
//  Sets the current stream index to any arbitrary position.
//  Setting to 0 sets it to the beginning of the stream.
//  Any subsequent output will start from this position
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void    ctrSetStreamIndex( ctrContext * Context,
            uint64_t StreamIndex );

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  ctrXor
//
//  XORs the stream of byte of the ctrContext from its current stream position onto the specified buffer. This will
//  advance the stream index by that number of bytes.
//  Use once over data to encrypt it. Use it a second time over the same data from the same stream position and the
//  data will be decrypted.
//  iBuffer and oBuffer can point to the same location for in-place encrypting/decrypting
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void    ctrXor( ctrContext * Context,
            void * oBuffer, void const * iBuffer, uint32_t bufferSize );

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  ctrOutput
//
//  Outputs the stream of byte of the ctrContext from its current stream position. This will advance the stream
//  index by that number of bytes.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void    ctrOutput( ctrContext * Context,
            void * Buffer, uint32_t bufferSize );

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  ctrXorWithKey
//
//  This function combines ctrInitWithKey and ctrXor. This is suitable when encrypting/decypting data in
//  one go with a key that is not going to be reused.
//  This will used the provided Key and IV and generate a stream that is XORed over Buffer.
//  Returns 0 if successful, or -1 if invalid KeySize provided
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int     ctrXorWithKey( void * oBuffer,
            void const * iBuffer, uint32_t bufferSize ,
            uint8_t const * Key, uint32_t KeySize,
            uint8_t const IV[CTR_IV_SIZE] );

END_EXTERN_C()
