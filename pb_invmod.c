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

int pb_invmod(pb_poly *a, pb_poly *b, pb_poly *c)
{
   int err;
   pb_poly tmp;
  
   if ((err = pb_init(&tmp, &(c->characteristic))) != MP_OKAY) {
      return err;
   }

   if ((err = pb_exteuclid(a, b, c, NULL, &tmp)) != MP_OKAY)    { goto _ERR; }

   /* if deg(tmp(x)) > 0 then there is no invmod */
   if (tmp.used > 1)                                            { err = MP_VAL; goto _ERR; }

   err = MP_OKAY;
_ERR: pb_clear(&tmp);
   return err;
}


