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

/* dest = src */ 
int pb_copy(pb_poly *src, pb_poly *dest)
{
   int err, x;

   /* avoid trivial copies */
   if (src == dest) {
      return MP_OKAY;
   }

   /* grow dest as required */
   if (dest->alloc < src->used) {
      if ((err = pb_grow(dest, src->used)) != MP_OKAY) {
         return err;
      }
   }

   /* set the characteristic */
   if ((err = mp_copy(&(src->characteristic), &(dest->characteristic))) != MP_OKAY) {
      return err;
   }

   /* copy digits */
   for (x = 0; x < src->used; x++) {
       if ((err = mp_copy(&(src->terms[x]), &(dest->terms[x]))) != MP_OKAY) {
          return err;
       }
   }

   /* zero excess digits */
   for (x = src->used; x < dest->used; x++) {
       mp_zero(&(dest->terms[x]));
   }
   dest->used = src->used;
   return MP_OKAY;
}


