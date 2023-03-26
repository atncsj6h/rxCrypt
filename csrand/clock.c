#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <stdint.h>

#include <memory.h>

// struct timespec {
//    time_t   tv_sec;        /* seconds */
//    long     tv_nsec;       /* nanoseconds */
// };

int main ()
{

    union {
        int64_t     nSecs;
        uint8_t     bytes[8];
    } x64 ;


    struct timespec time;


    printf("...\n");
    printf("clock()              %d\n", clock());
    printf("time()               %d\n", time());

    printf("CLOCK_MONOTONIC_RAW\n");


/*  CLOCK_MONOTONIC_RAW
*/
    printf("CLOCK_MONOTONIC_RAW\n");
    clock_getres(CLOCK_MONOTONIC_RAW, &time);
    printf("        clock_getres\n");
    printf("        time.tv_sec  %ld\n",        time.tv_sec  );
    printf("        time.tv_nsec %ld\n",        time.tv_nsec  );

    clock_gettime(CLOCK_MONOTONIC_RAW, &time);
    printf("        clock_gettime\n");
    printf("        time.tv_sec  %ld\n",        time.tv_sec  );
    printf("        time.tv_nsec %ld\n",        time.tv_nsec  );

    x64.nSecs = time.tv_nsec;
    printf("        nanoseconds  %lld\n", x64.nSecs        );

    for ( int i = 0; i<8; i++)
        printf("%.2x", x64.bytes[i]);
    printf("\n");

    for ( int k = 0; k<32; k++)
    {
        clock_gettime(CLOCK_MONOTONIC_RAW, &time);
        x64.nSecs = time.tv_nsec+(time.tv_sec*1000000000);
        x64.nSecs = time.tv_nsec ;

        printf( "%3d %16lld - ", k, x64.nSecs);
        for ( int i = 0; i<8; i++)
            printf("%.2x", x64.bytes[i]);
        printf("\n");
    }

    return 0;
}
