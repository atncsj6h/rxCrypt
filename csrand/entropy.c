#include <stdlib.h>
#include <stdint.h>
#include <sys/random.h>

#include <stdio.h>
#define show(_e) \
do { \
for ( int x = 0; x < sizeof(_e); x++ ) \
    printf("%.2x",_e[x]); \
    printf("\n\n"); \
} while ( 0 );

int main()
{
    uint8_t key[64] ;
    uint8_t IV[32];
    uint8_t buffer[256];

    for ( int i = 0; i < 16; i++ )
    {
        getentropy(key, sizeof(key) );
        show(key);
        getentropy(IV, sizeof(IV) );
        show(IV);
        getentropy(buffer, sizeof(buffer) );
        show(buffer)
    }

    return 0;
}
