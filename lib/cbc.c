/*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    cbc.c cbc.h

    Implementation of AES( Rijndael ) CBC cipher.
    Depends on: aes.h, aes.c

    AES CBC is a cipher using AES in Cipher Block Chaining mode.
    Encryption and decryption must be performed in multiples
        of the AES block size (128 bits).

    free and unencumbered, released into the public domain
*/

/*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
*/
#include <stdint.h>
#include <memory.h>

/*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
*/
#include "aes.h"
#include "cbc.h"

/*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
*/
#define MIN( x, y ) ( ((x)<(y))?(x):(y) )

#define STORE64H( x, y )                                                       \
   { (y)[0] = (uint8_t)(((x)>>56)&255); (y)[1] = (uint8_t)(((x)>>48)&255);     \
     (y)[2] = (uint8_t)(((x)>>40)&255); (y)[3] = (uint8_t)(((x)>>32)&255);     \
     (y)[4] = (uint8_t)(((x)>>24)&255); (y)[5] = (uint8_t)(((x)>>16)&255);     \
     (y)[6] = (uint8_t)(((x)>>8)&255);  (y)[7] = (uint8_t)((x)&255); }

/*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    private
*/
static
void    xorAesBlock ( uint8_t * Block1, uint8_t const * Block2 )
{
    for( uint_fast32_t i=0; i<AES_BLOCK_SIZE; i++ )
    {
        Block1[i] ^= Block2[i];
    }
}

/*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    public
*/

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  cbcInit
//
//  Initialises an cbcContext with an already initialised aesContext and a IV. This function can quickly be used
//  to change the IV without requiring the more lengthy processes of reinitialising an AES key.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void    cbcInit( cbcContext * Context,
            aesContext const * aesContextInitialised,
            uint8_t const IV[CBC_IV_SIZE] )
{
    // Setup Context values
    Context->aes = *aesContextInitialised;
    memcpy( Context->PreviousCipherBlock, IV, sizeof(Context->PreviousCipherBlock) );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  cbcInitWithKey
//
//  Initialises an cbcContext with an AES Key and an IV. This combines the initialising an AES Context and then
//  running cbcInit. KeySize must be 16, 24, or 32 (for 128, 192, or 256 bit key size)
//  Returns 0 if successful, or -1 if invalid KeySize provided
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int     cbcInitWithKey( cbcContext * Context,
            uint8_t const * Key, uint32_t KeySize,
            uint8_t const IV[CBC_IV_SIZE] )
{
    aesContext aes;

    // Initialise AES Context
    if  ( 0 != aesInit( &aes, Key, KeySize ) )
    {
        return -1;
    }

    // set-up cbcContext
    cbcInit( Context, &aes, IV );
    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  cbcEncrypt
//
//  Encrypts a buffer of data using an AES CBC Context. The data buffer must be a multiple of 16 bytes (128 bits)
//  in size. The "position" of the Context will be advanced by the buffer amount. A buffer can be encrypted in one
//  go or in smaller chunks at a time. The result will be the same as long as data is fed into the function in the
//  same order.
//  iBuffer and oBuffer can point to the same location for in-place encrypting.
//  Returns 0 if successful, or -1 if bufferSize is not a multiple of 16 bytes.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int     cbcEncrypt( cbcContext * Context,
            void * oBuffer, void const * iBuffer, uint32_t bufferSize )
{
    uint32_t    numBlocks = bufferSize / AES_BLOCK_SIZE;
    uint32_t    offset = 0;
    uint32_t    i;

    if  ( 0 != bufferSize % AES_BLOCK_SIZE )
    {
        // bufferSize not a multiple of AES block size (16 bytes).
        return -1;
    }

    for ( i=0; i<numBlocks; i++ )
    {
        // XOR on the next block of data onto the previous cipher block
        xorAesBlock( Context->PreviousCipherBlock, (uint8_t*)iBuffer + offset );

        // Encrypt to make new cipher block
        aesEncryptInPlace( &Context->aes, Context->PreviousCipherBlock );

        // oBuffer cipher block
        memcpy( (uint8_t*)oBuffer + offset, Context->PreviousCipherBlock, AES_BLOCK_SIZE );

        offset += AES_BLOCK_SIZE;
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  cbcDecrypt
//
//  Decrypts a buffer of data using an AES CBC Context. The data buffer must be a multiple of 16 bytes (128 bits)
//  in size. The "position" of the Context will be advanced by the buffer amount.
//  iBuffer and oBuffer can point to the same location for in-place decrypting.
//  Returns 0 if successful, or -1 if bufferSize is not a multiple of 16 bytes.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int     cbcDecrypt( cbcContext * Context,
            void * oBuffer, void const * iBuffer, uint32_t bufferSize )
{
    uint32_t    numBlocks = bufferSize / AES_BLOCK_SIZE;

    uint32_t    offset = 0;
    uint32_t    i;
    uint8_t     previousCipherBlock [AES_BLOCK_SIZE];

    if  ( 0 != bufferSize % AES_BLOCK_SIZE )
    {
        // bufferSize not a multiple of AES block size (16 bytes).
        return -1;
    }

    for ( i=0; i<numBlocks; i++ )
    {
        // Copy previous cipher block and place current one in Context
        memcpy( previousCipherBlock, Context->PreviousCipherBlock, AES_BLOCK_SIZE );
        memcpy( Context->PreviousCipherBlock, (uint8_t*)iBuffer + offset, AES_BLOCK_SIZE );

        // Decrypt cipher block
        aesDecrypt( &Context->aes, (uint8_t*)oBuffer + offset, Context->PreviousCipherBlock );

        // XOR on previous cipher block
        xorAesBlock( (uint8_t*)oBuffer + offset, previousCipherBlock );

        offset += AES_BLOCK_SIZE;
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  cbcEncryptWithKey
//
//  This function combines cbcInitWithKey and cbcEncrypt. This is suitable when encrypting data in one go
//  with a key that is not going to be reused.
//  iBuffer and oBuffer can point to the same location for inplace encrypting.
//  Returns 0 if successful, or -1 if invalid KeySize provided or BufferSize not a multiple of 16 bytes.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int     cbcEncryptWithKey( void * oBuffer,
            void const * iBuffer, uint32_t BufferSize,
            uint8_t const * Key, uint32_t KeySize,
            uint8_t const IV[CBC_IV_SIZE] )
{
    int error;
    cbcContext   Context;

    error = cbcInitWithKey( &Context, Key, KeySize, IV );
    if  ( error == 0 )
    {
        error = cbcEncrypt( &Context, oBuffer, iBuffer, BufferSize );
    }

    return error;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  cbcDecryptWithKey
//
//  This function combines cbcInitWithKey and cbcDecrypt. This is suitable when decrypting data in one go
//  with a key that is not going to be reused.
//  iBuffer and oBuffer can point to the same location for inplace decrypting.
//  Returns 0 if successful, or -1 if invalid KeySize provided or BufferSize not a multiple of 16 bytes.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int     cbcDecryptWithKey( void * oBuffer,
            void const * iBuffer, uint32_t BufferSize,
            uint8_t const * Key, uint32_t KeySize,
            uint8_t const IV[CBC_IV_SIZE] )
{
    int error;
    cbcContext   Context;

    error = cbcInitWithKey( &Context, Key, KeySize, IV );
    if( 0 == error )
    {
        error = cbcDecrypt( &Context, oBuffer, iBuffer,  BufferSize );
    }

    return error;
}
