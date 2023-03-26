/*
------------------------------------------------------------------------------
rand.c: By Bob Jenkins.  My random number generator, ISAAC.  Public Domain.
MODIFIED:
  960327: Creation (addition of randinit, really)
  970719: use context, not global variables, for internal state
  980324: added main (ifdef'ed out), also rearranged randinit()
  010626: Note that this is public domain
------------------------------------------------------------------------------
*/
#ifdef NEVER
#include <stdio.h>
#endif


#include <stdint.h>

#include "rand.h"

#define ind(mm,x)  (*(uint32_t *)((uint8_t *)(mm) + ((x) & ((RANDSIZ-1)<<2))))
#define rngstep(mix,a,b,mm,m,m2,r,x) \
{ \
  x = *m;  \
  a = (a^(mix)) + *(m2++); \
  *(m++) = y = ind(mm,x) + a + b; \
  *(r++) = b = ind(mm,y>>RANDSIZL) + x; \
}

void    isaac(randContext * Context )
{
   register uint32_t a,b,x,y,*m,*mm,*m2,*r,*mend;
   mm=Context->randmem; r=Context->randrsl;
   a = Context->randa; b = Context->randb + (++Context->randc);
   for (m = mm, mend = m2 = m+(RANDSIZ/2); m<mend; )
   {
      rngstep( a<<13, a, b, mm, m, m2, r, x);
      rngstep( a>>6 , a, b, mm, m, m2, r, x);
      rngstep( a<<2 , a, b, mm, m, m2, r, x);
      rngstep( a>>16, a, b, mm, m, m2, r, x);
   }
   for (m2 = mm; m2<mend; )
   {
      rngstep( a<<13, a, b, mm, m, m2, r, x);
      rngstep( a>>6 , a, b, mm, m, m2, r, x);
      rngstep( a<<2 , a, b, mm, m, m2, r, x);
      rngstep( a>>16, a, b, mm, m, m2, r, x);
   }
   Context->randb = b; Context->randa = a;
}


#define mix(a,b,c,d,e,f,g,h) \
{ \
   a^=b<<11; d+=a; b+=c; \
   b^=c>>2;  e+=b; c+=d; \
   c^=d<<8;  f+=c; d+=e; \
   d^=e>>16; g+=d; e+=f; \
   e^=f<<10; h+=e; f+=g; \
   f^=g>>4;  a+=f; g+=h; \
   g^=h<<8;  b+=g; h+=a; \
   h^=a>>9;  c+=h; a+=b; \
}

/* if (flag==TRUE), then use the contents of randrsl[] to initialize mm[]. */
void randinit(randContext * Context, int flag)
{
   int32_t  i;
   uint32_t a,b,c,d,e,f,g,h;
   uint32_t *m,*r;
   Context->randa = Context->randb = Context->randc = 0;
   m=Context->randmem;
   r=Context->randrsl;
   a=b=c=d=e=f=g=h=0x9e3779b9;  /* the golden ratio */

   for (i=0; i<4; ++i)          /* scramble it */
   {
     mix(a,b,c,d,e,f,g,h);
   }

   if (flag)
   {
     /* initialize using the contents of r[] as the seed */
     for (i=0; i<RANDSIZ; i+=8)
     {
       a+=r[i  ]; b+=r[i+1]; c+=r[i+2]; d+=r[i+3];
       e+=r[i+4]; f+=r[i+5]; g+=r[i+6]; h+=r[i+7];
       mix(a,b,c,d,e,f,g,h);
       m[i  ]=a; m[i+1]=b; m[i+2]=c; m[i+3]=d;
       m[i+4]=e; m[i+5]=f; m[i+6]=g; m[i+7]=h;
     }
     /* do a second pass to make all of the seed affect all of m */
     for (i=0; i<RANDSIZ; i+=8)
     {
       a+=m[i  ]; b+=m[i+1]; c+=m[i+2]; d+=m[i+3];
       e+=m[i+4]; f+=m[i+5]; g+=m[i+6]; h+=m[i+7];
       mix(a,b,c,d,e,f,g,h);
       m[i  ]=a; m[i+1]=b; m[i+2]=c; m[i+3]=d;
       m[i+4]=e; m[i+5]=f; m[i+6]=g; m[i+7]=h;
     }
   }
   else
   {
     /* fill in m[] with messy stuff */
     for (i=0; i<RANDSIZ; i+=8)
     {
       mix(a,b,c,d,e,f,g,h);
       m[i  ]=a; m[i+1]=b; m[i+2]=c; m[i+3]=d;
       m[i+4]=e; m[i+5]=f; m[i+6]=g; m[i+7]=h;
     }
   }

   //isaac(Context);            /* fill in the first set of results */
   Context->randcnt=RANDSIZ;  /* prepare to use the first set of results */
}


#ifdef NEVER
int main(int argc, char *argv[])
{

  int flag = ( argc > 1 ) ? 1 : 0 ;

  uint32_t i,j;
  randContext Context;
  Context.randa=Context.randb=Context.randc=(uint32_t)0;
  for (i=0; i<256; ++i) Context.randrsl[i]=(uint32_t)0;
  randinit(&Context, flag );

    // isaac(&Context);
    for (j=0; j<RANDSIZ; ++j)
    {
      printf("%.8x ",Context.randrsl[j]);
      if ((j&7)==7) printf("\n");
    }
    printf("\n");

  printf("testing rand()\n");

  Context.randa=Context.randb=Context.randc=(uint32_t)0;
  for (i=0; i<256; ++i) Context.randrsl[i]=(uint32_t)0;
  randinit(&Context, flag );
  printf("%d\n",Context.randcnt);
  for (j=0; j<RANDSIZ; ++j)
    {
      Context.randcnt=0;
      printf("%.8x ",rand(&Context));
      if ((j&7)==7) printf("\n");
    }

}
#endif
