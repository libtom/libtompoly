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

int pb_init_copy(pb_poly *a, pb_poly *b)
{
   int err;
   if ((err = pb_init(a, &(b->characteristic))) != MP_OKAY) {
      return err;
   }
   return pb_copy(b, a);
}
