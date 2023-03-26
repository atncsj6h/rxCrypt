/*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    ecb.c

    Implementation of AES ECB cipher.
    Depends on: aes.h, aes.c

    AES ECB is a cipher using AES in Electronic Code Book mode
    Encryption and decryption must be performed in multiples
        of the AES block size (128 bits).

    free and unencumbered, released into the public domain
*/

/*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
*/
#include <stdio.h>
#include <stdint.h>
#include <memory.h>

/*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
*/
#include "aes.h"
#include "ecb.h"

/*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    public
*/

/*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
*/
int     ecbEncrypt( aesContext const * Context,
            uint8_t * oBuffer, uint8_t const * iBuffer, uint32_t const bufferSize )
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
        // Encrypt to make new cipher block
        aesEncrypt( Context,
            (uint8_t*)oBuffer + offset, (uint8_t*)iBuffer + offset );
        offset += AES_BLOCK_SIZE;
    }

    return 0;
}

/*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
*/
int     ecbDecrypt( aesContext const * Context,
            uint8_t * oBuffer, uint8_t const * iBuffer, uint32_t const bufferSize )
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
        // Decrypt to make new cipher block
        aesDecrypt( Context,
            (uint8_t*)oBuffer + offset, (uint8_t*)iBuffer + offset );
        offset += AES_BLOCK_SIZE;
    }

    return 0 ;
}

