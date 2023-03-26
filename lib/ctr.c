/*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    ctr.c tr.h

    Implementation of AES( Rijndael ) CTR cipher.
    Depends on: aes.h, aes.c

    AES CTR is a stream using AES block cipher Counter mode.

    free and unencumbered, released into the public domain
*/

/*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
*/
#include <stdint.h>
#include <memory.h>

/*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
*/
#include "aes.h"
#include "ctr.h"

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
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  CreateCurrentCipherBlock
//
//  Takes the IV and the counter in the ctrContext and produces the cipher block (CurrentCipherBlock). The cipher
//  block is produced by first creating a 128 bit block with the IV as first 64 bits and the CurrentCipherBlockIndex
//  stored as the remaining 64bits in Network byte order (Big Endian)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static
void    CreateCurrentCipherBlock( ctrContext * Context)
{
    // Build block by first copying in the IV
    memcpy( Context->CurrentCipherBlock, Context->IV, CTR_IV_SIZE );

    // Now place in the counter in Big Endian form
    STORE64H( Context->CurrentCipherBlockIndex, Context->CurrentCipherBlock + CTR_IV_SIZE );

    // Perform AES encryption on the block
    aesEncryptInPlace( &Context->aes, Context->CurrentCipherBlock );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  XorBuffer
//
//  Takes two Source buffers and XORs them together and puts the result in Dest
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static
void    xorBuffers( uint8_t * Dest,
            uint8_t const * buff1, uint8_t const * buff2,
            uint32_t bufferSize )
{
    uint32_t    i;

    for( i=0; i<bufferSize; i++ )
    {
        Dest[i] = buff1[i] ^ buff2[i];
    }
}

/*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    public
*/

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  ctrInit
//
//  Initialises an ctrContext with an already initialised aesContext and a IV. This function can quickly be used
//  to change the IV without requiring the more length processes of reinitialising an AES key.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void    ctrInit( ctrContext * Context,
            aesContext const * aesContextInitialised,
            uint8_t const IV[CTR_IV_SIZE] )
{
    // Setup Context values
    Context->aes = *aesContextInitialised;
    memcpy( Context->IV, IV, CTR_IV_SIZE );
    Context->StreamIndex = 0;
    Context->CurrentCipherBlockIndex = 0;

    // Generate the first cipher block of the stream.
    CreateCurrentCipherBlock( Context );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  ctrInitWithKey
//
//  Initialises an ctrContext with an AES Key and an IV. This combines the initialising an AES Context and then
//  running ctrInit. KeySize must be 16, 24, or 32 (for 128, 192, or 256 bit key size)
//  Returns 0 if successful, or -1 if invalid KeySize provided
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int     ctrInitWithKey( ctrContext * Context,
            uint8_t const * Key, uint32_t KeySize,
            uint8_t const IV[CTR_IV_SIZE] )
{
    aesContext aes;

    // Initialise aes Context
    if( 0 != aesInit( &aes, Key, KeySize ) )
    {
        return -1;
    }

    // Now set-up ctrContext
    ctrInit( Context, &aes, IV );
    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  ctrSetStreamIndex
//
//  Sets the current stream index to any arbitrary position. Setting to 0 sets it to the beginning of the stream. Any
//  subsequent output will start from this position
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void    ctrSetStreamIndex( ctrContext * Context,
            uint64_t StreamIndex )
{
    uint64_t    blockIndex = StreamIndex / AES_BLOCK_SIZE;

    Context->StreamIndex = StreamIndex;
    if( blockIndex != Context->CurrentCipherBlockIndex )
    {
        // Update block index and generate new cipher block as the new StreamIndex is inside a different block to the
        // one we currently had.
        Context->CurrentCipherBlockIndex = blockIndex;
        CreateCurrentCipherBlock( Context );
    }
}

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
            void * oBuffer, void const * iBuffer, uint32_t bufferSize )
{
    uint32_t        firstChunkSize;
    uint32_t        amountAvailableInBlock;
    int             numIterations;
    int             i;
    uint64_t        loopStartingCipherBlockIndex;
    uint32_t        loopStartingOutputOffset;
    uint8_t         preCipherBlock [AES_KEY_SIZE_128];
    uint8_t         encCipherBlock [AES_KEY_SIZE_128];
    uint64_t        cipherBlockIndex = 0;

    // First determine how much is available in the current block.
    amountAvailableInBlock = AES_BLOCK_SIZE - (Context->StreamIndex % AES_BLOCK_SIZE);

    // Determine how much of the current block we will take, either all that is available, or less
    // if the amount requested is smaller.
    firstChunkSize = MIN( amountAvailableInBlock, bufferSize );

    // XOR the bytes from the cipher block
    xorBuffers( oBuffer,
        iBuffer, Context->CurrentCipherBlock + (AES_BLOCK_SIZE - amountAvailableInBlock),
        firstChunkSize );

    // Determine how many iterations will be needed for generating cipher blocks.
    // We always have to finish with a non-depleted cipher block.
    // Also calculate the cipher block index and the output offset for when we start the loop.
    // This function may be built with OpenMP and the loop will run in parallel. So we set-up variables that will
    // be common at the start of the loop.
    numIterations = ( (bufferSize - firstChunkSize) + AES_BLOCK_SIZE ) / AES_BLOCK_SIZE;
    loopStartingCipherBlockIndex = Context->CurrentCipherBlockIndex + 1;
    loopStartingOutputOffset = firstChunkSize;

    // Copy the IV into the first half of the preCipherBlock. When built for OpenMP preCipherBlock will be copied into
    // a local version within the loop.
    memcpy( preCipherBlock, Context->IV, CTR_IV_SIZE );

    // Now start generating new cipher blocks as required.
    #ifdef _OPENMP
        #pragma omp parallel for firstprivate( preCipherBlock, cipherBlockIndex ) lastprivate( encCipherBlock, cipherBlockIndex )
    #endif
    for( i=0; i<numIterations; i++ )
    {
        uint32_t outputOffset = loopStartingOutputOffset + (AES_BLOCK_SIZE * i);
        uint32_t amountLeft = bufferSize - outputOffset;
        uint32_t chunkSize = MIN( amountLeft, AES_BLOCK_SIZE );

        // Increment block index and regenerate cipher block
        cipherBlockIndex = loopStartingCipherBlockIndex + i;

        // Now place in the counter in Big Endian form in second half of preCipherBlock
        STORE64H( cipherBlockIndex, preCipherBlock + CTR_IV_SIZE );

        // Perform AES encryption on the preCipherBlock and put result in encCipherBlock
        aesEncrypt( &Context->aes, preCipherBlock, encCipherBlock );

        // XOR block out onto the buffer.
        xorBuffers( (uint8_t*)oBuffer + outputOffset,
            (uint8_t*)iBuffer + outputOffset, encCipherBlock,
            chunkSize );
    }

    // Update Context
    Context->StreamIndex += bufferSize;
    if( numIterations > 0 )
    {
        Context->CurrentCipherBlockIndex = cipherBlockIndex;
        memcpy( Context->CurrentCipherBlock, encCipherBlock, AES_BLOCK_SIZE );
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  ctrOutput
//
//  Outputs the stream of byte of the ctrContext from its current stream position. This will advance the stream
//  index by that number of bytes.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void    ctrOutput( ctrContext * Context,
            void * Buffer, uint32_t bufferSize )
{
    memset( Buffer, 0, bufferSize );
    ctrXor( Context, Buffer, Buffer, bufferSize );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  ctrXorWithKey
//
//  This function combines ctrInitWithKey and ctrXor. This is suitable when encrypting/decypting data in
//  one go with a key that is not going to be reused.
//  This will used the provided Key and IV and generate a stream that is XORed over Buffer.
//  iBuffer and oBuffer can point to the same location for inplace encrypting/decrypting
//  Returns 0 if successful, or -1 if invalid KeySize provided
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int     ctrXorWithKey( void * oBuffer,
            void const * iBuffer, uint32_t bufferSize ,
            uint8_t const * Key, uint32_t KeySize,
            uint8_t const IV[CTR_IV_SIZE] )
{
int error;
ctrContext   Context;

    error = ctrInitWithKey( &Context, Key, KeySize, IV );
    if( 0 == error )
    {
        ctrXor( &Context, oBuffer, iBuffer, bufferSize );
    }

    return error;
}
