/* LibTomPoly, Polynomial Basis Math -- Tom St Denis 
 * 
 * LibTomPoly is a public domain library that provides
 * polynomial basis arithmetic support.  It relies on
 * LibTomMath for large integer support.
 *
 * This library is free for all purposes without any
 * express guarantee that it works.
 *
 * Tom St Denis, tomstdenis@iahu.ca, http://poly.libtomcrypt.org
 */
#include <tompoly.h>

int pb_cmp(pb_poly *a, pb_poly *b)
{
   int x;

   if (a->used > b->used) {
      return PB_DEG_GT;
   } else if (a->used < b->used) {
      return PB_DEG_LT;
   }

   for (x = 0; x < a->used; x++) {
       if (mp_cmp(&(a->terms[x]), &(b->terms[x])) != MP_EQ) {
          return PB_DEG_EQ;
       }
   }
   return PB_EQ;
}


