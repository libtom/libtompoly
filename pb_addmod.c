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

int pb_addmod(pb_poly *a, pb_poly *b, pb_poly *c, pb_poly *d)
{
   int err;
   pb_poly tmp;

   if ((err = pb_init(&tmp, &(d->characteristic))) != MP_OKAY) {
      return err;
   }
   if ((err = pb_add(a, b, &tmp)) != MP_OKAY) {
      goto __TMP;
   }
   if ((err = pb_mod(&tmp, c, d)) != MP_OKAY) {
      goto __TMP;
   }

   err = MP_OKAY;
__TMP: pb_clear(&tmp);
   return err;
}



