/*
------------------------------------------------------------------------------
rand.h: definitions for a random number generator
By Bob Jenkins, 1996, Public Domain
MODIFIED:
  960327: Creation (addition of randinit, really)
  970719: use context, not global variables, for internal state
  980324: renamed seed to flag
  980605: recommend RANDSIZL=4 for noncryptography.
  010626: note this is public domain
------------------------------------------------------------------------------
*/
#ifndef RAND
#define RAND

#include <stdint.h>

#define RANDSIZL   (8)
#define RANDSIZ    (1<<RANDSIZL)

/* context of random number generator */
struct randContext
{
  uint32_t randcnt;
  uint32_t randrsl[RANDSIZ];
  uint32_t randmem[RANDSIZ];
  uint32_t randa;
  uint32_t randb;
  uint32_t randc;
};
typedef  struct randContext  randContext;

/*
------------------------------------------------------------------------------
 If (flag==TRUE), then use the contents of randrsl[0..RANDSIZ-1] as the seed.
------------------------------------------------------------------------------
*/
void randinit(/*_ randContext *Context, int32_t flag _*/);

void isaac(/*_ randContext *Context _*/);


/*
------------------------------------------------------------------------------
 Call rand(/o_ randContext *Context _o/) to retrieve a single 32-bit random value
------------------------------------------------------------------------------
*/
#define rand(Context) \
   (!(Context)->randcnt-- ? \
     (isaac(Context), (Context)->randcnt=RANDSIZ-1, (Context)->randrsl[(Context)->randcnt]) : \
     (Context)->randrsl[(Context)->randcnt])


#define TRUE  1
#define FALSE 0

#endif  /* RAND */
