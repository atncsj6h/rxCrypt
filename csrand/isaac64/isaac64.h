/*
------------------------------------------------------------------------------
isaac64.h: definitions for a random number generator
Bob Jenkins, 1996, Public Domain
------------------------------------------------------------------------------
*/
#ifndef isaac64_h_included
#define isaac64_h_included

#include "standard.h"

#define RANDSIZL   (8)
#define RANDSIZ    (1<<RANDSIZL)

ub8 randrsl[RANDSIZ], randcnt;

/*
------------------------------------------------------------------------------
 If (flag==TRUE), then use the contents of randrsl[0..255] as the seed.
------------------------------------------------------------------------------
*/
void randinit(/*_ word flag _*/);

void isaac64();


/*
------------------------------------------------------------------------------
 Call rand() to retrieve a single 64-bit random value
------------------------------------------------------------------------------
*/
#define rand() \
   (!randcnt-- ? (isaac64(), randcnt=RANDSIZ-1, randrsl[randcnt]) : \
                 randrsl[randcnt])

#endif  //  isaac64_h_included

