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

/* returns the monic GCD only for GF(p^k)[x] */
int pb_gcd(pb_poly *a, pb_poly *b, pb_poly *c)
{
   pb_poly A, B, tmp;
   int err;

   if (mp_iszero(&(c->characteristic)) == MP_YES) {
      return MP_VAL;
   }

   /* special cases (one or both are zero) */
   if (a->used == 0 && b->used == 0) {
      /* both zero, set to 1 */
      pb_zero(c);
      c->used = 1;
      mp_set(&(c->terms[0]), 1);
      return MP_OKAY;
   } else if (a->used == 0) {
      return pb_copy(b, c);
   } else if (b->used == 0) {
      return pb_copy(a, c);
   }

   if ((err = pb_init(&tmp, &(c->characteristic))) != MP_OKAY) {
      return err;
   }
   if ((err = pb_init_copy(&A, a)) != MP_OKAY) {
      goto __TMP;
   }
   if ((err = pb_init_copy(&B, b)) != MP_OKAY) {
      goto __A;
   }

   while (B.used > 0) {
       if ((err = pb_mod(&A, &B, &tmp)) != MP_OKAY) {
          goto __B;
       }
       /* A = B, B = tmp */
       if ((err = pb_copy(&B, &A)) != MP_OKAY) {
          goto __B;
       }
       if ((err = pb_copy(&tmp, &B)) != MP_OKAY) {
          goto __B;
       }
   }

   /* ensure it's monic */
   err = pb_monic(&A, c);

__B  : pb_clear(&B);
__A  : pb_clear(&A);
__TMP: pb_clear(&tmp);
   return err;
}


