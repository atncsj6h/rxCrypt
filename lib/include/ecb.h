/*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    ecb.h

    Implementation of AES ECB cipher.
    Depends on: aes.h, aes.c

    AES ECB is a cipher using AES in Electronic Code Book mode
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
BEGIN_EXTERN_C()

int ecbEncrypt( aesContext const * Context,
        uint8_t * oBuffer, uint8_t const * iBuffer, uint32_t const bufferSize );


int ecbEncryptInPlace( aesContext const * Context,
        uint8_t * Buffer, uint32_t const bufferSize );

int ecbDecrypt( aesContext const * Context,
        uint8_t * oBuffer, uint8_t const * iBuffer, uint32_t const bufferSize );


int ecbDecryptInPlace( aesContext const * Context,
        uint8_t * Buffer, uint32_t const bufferSize );

END_EXTERN_C()

