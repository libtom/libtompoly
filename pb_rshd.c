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

int pb_rshd(pb_poly *a, int x)
{
   int y;

   if (x >= a->used) {
      pb_zero(a);
      return MP_OKAY;
   }

   if (x <= 0) {
      return MP_OKAY;
   }

   for (y = x; y < a->used; y++) {
       mp_exch(&(a->terms[y]), &(a->terms[y-x]));
   }

   for (y = a->used - x; y < a->used; y++) {
       mp_zero(&(a->terms[y]));
   }
   a->used -= x;
   
   return MP_OKAY; 
}

