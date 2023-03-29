#include <stdio.h>
#include <stdint.h>

int main(int argc, char *argv[])
{
    printf("System is   %zu bit\n\n", sizeof(void*) * 8);

    printf("uchar       %zu\n", sizeof(unsigned char));
    printf("uint8_t     %zu\n", sizeof(uint8_t));
    printf("uint16_t    %zu\n", sizeof(uint16_t));
    printf("uint32_t    %zu\n", sizeof(uint32_t));
    printf("uint64_t    %zu\n\n", sizeof(uint64_t));

    printf("char        %zu\n", sizeof(char));
    printf("int8_t      %zu\n", sizeof(int8_t));
    printf("int16_t     %zu\n", sizeof(int16_t));
    printf("int32_t     %zu\n", sizeof(int32_t));
    printf("int64_t     %zu\n\n", sizeof(int64_t));

    printf("size_t      %zu\n\n", sizeof(size_t));
    printf("off_t       %zu\n\n", sizeof(off_t));


    int i;
    long int l;
    long long int ll;

    printf("int         %zu\n\n", sizeof(i));
    printf("long        %zu\n\n", sizeof(l));
    printf("long long   %zu\n\n", sizeof(ll));

#define RANDSIZL   (4)
#define RANDSIZ    (1<<RANDSIZL)

    printf("RANDSIZL         %d\n", RANDSIZL);
    printf("RANDSIZ          %d\n", RANDSIZ );


    return 0;
}
