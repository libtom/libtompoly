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

int pb_lshd(pb_poly *a, int x)
{
   int err, y;

   if (x <= 0) {
      return MP_OKAY;
   }

   /* grow as required */
   if (a->alloc < (a->used + x)) {
      if ((err = pb_grow(a, a->used + x)) != MP_OKAY) {
         return err;
      }
   }
  
   /* shift */
   for (y = a->used + x; y >= x; y--) {
       mp_exch(&(a->terms[y-x]), &(a->terms[y]));
   }

   /* zero lower digits */
   for (y = 0; y < x; y++) {
       mp_zero(&(a->terms[y]));
   }
   a->used += x;

   return MP_OKAY;
}

