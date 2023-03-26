/*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    ofb.c
    Implementation of AES OFB stream cipher.
    Depends on: aes.h, aes.c
    AES OFB is a stream cipher using the AES block cipher in output feedback mode.

    free and unencumbered, released into the public domain
*/

/*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
*/
#include <stdint.h>
#include <memory.h>

/*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
*/
#include "aes.h"
#include "ofb.h"

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
void    xorBuffers( uint8_t * dest,
            uint8_t const * buff1, uint8_t const * buff2,
            uint32_t bSize )
{
    for( uint32_t i=0; i<bSize; i++ )
    {
        dest[i] = buff1[i] ^ buff2[i];
    }
}

/*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    public
*/

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  ofbInit
//
//  Initialises an ofbContext with an already initialised aesContext and a IV. This function can quickly be used
//  to change the IV without requiring the more lengthy processes of reinitialising an AES key.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void    ofbInit( ofbContext * Context,
            aesContext const *  InitialisedAesContext,
            uint8_t const IV[OFB_IV_SIZE] )
{
    // Setup context values
    Context->aes = *InitialisedAesContext;
    memcpy( Context->CurrentCipherBlock, IV, sizeof(Context->CurrentCipherBlock) );
    Context->IndexWithinCipherBlock = 0;

    // Generate the first cipher block of the stream.
    aesEncryptInPlace( &Context->aes, Context->CurrentCipherBlock );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  ofbInitWithKey
//
//  Initialises an ofbContext with an AES Key and an IV. This combines the initialising an AES Context and then
//  running ofbInit. KeySize must be 16, 24, or 32 (for 128, 192, or 256 bit key size)
//  Returns 0 if successful, or -1 if invalid KeySize provided
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int     ofbInitWithKey( ofbContext * Context,
            uint8_t const * Key, uint32_t KeySize,
            uint8_t const IV[OFB_IV_SIZE] )
{
    aesContext aes;

    // Initialise AES Context
    if( 0 != aesInit( &aes, Key, KeySize ) )
    {
        return -1;
    }

    // Now set-up ofbContext
    ofbInit( Context, &aes, IV );
    return 0;
}

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
void    ofbXor( ofbContext * Context,
            void * oBuffer, void const * iBuffer, uint32_t BufferSize )
{
    uint32_t    amountAvailableInBlock;
    uint32_t    amountLeft = BufferSize;
    uint32_t    outputOffset = 0;
    uint32_t    chunkSize;

    // First determine how much is available in the current block.
    amountAvailableInBlock = AES_BLOCK_SIZE - Context->IndexWithinCipherBlock;

    // Determine how much of the current block we will take, either all that is available, or less
    // if the amount requested is smaller.
    chunkSize = MIN( amountAvailableInBlock, amountLeft );

    // XOR the bytes from the cipher block
    xorBuffers( oBuffer,
        iBuffer, Context->CurrentCipherBlock + (AES_BLOCK_SIZE - amountAvailableInBlock),
        chunkSize );

    amountLeft -= chunkSize;
    outputOffset += chunkSize;
    Context->IndexWithinCipherBlock += chunkSize;

    // Now start generating new cipher blocks as required.
    while( amountLeft > 0 )
    {
        // Generate new cipher block
        aesEncryptInPlace( &Context->aes, Context->CurrentCipherBlock );

        // Determine how much of the current block we need and XOR it out onto the buffer
        chunkSize = MIN( amountLeft, AES_BLOCK_SIZE );
        xorBuffers( (uint8_t*)oBuffer + outputOffset,
            (uint8_t*)iBuffer + outputOffset, Context->CurrentCipherBlock,
            chunkSize );

        amountLeft -= chunkSize;
        outputOffset += chunkSize;
        Context->IndexWithinCipherBlock = chunkSize;    // Note: Not incremented
    }

    // If we ended up completely reading the last cipher block we need to generate a new one for next time.
    if( AES_BLOCK_SIZE == chunkSize )
    {
        aesEncryptInPlace( &Context->aes, Context->CurrentCipherBlock );
        Context->IndexWithinCipherBlock = 0;
    }
}

/*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    ofbOutput

    Outputs the stream of byte of the ofbContext from its current stream position.
    This will advance the stream index by that number of bytes.
*/
void    ofbOutput( ofbContext * Context,
            void * Buffer, uint32_t BufferSize )
{
    memset( Buffer, 0, BufferSize );
    ofbXor( Context, Buffer, Buffer, BufferSize );
}

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
int     ofbXorWithKey(void * oBuffer,
            void const * iBuffer, uint32_t BufferSize ,
            uint8_t const * Key, uint32_t KeySize,
            uint8_t const IV[OFB_IV_SIZE] )
{
    int             error;
    ofbContext   context;

    error = ofbInitWithKey( &context, Key, KeySize, IV );
    if( 0 == error )
    {
        ofbXor( &context, oBuffer, iBuffer, BufferSize );
    }

    return error;
}


