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
   mp_int  inv;
   int err, x; 

   if (mp_iszero(&(c->characteristic)) == MP_YES) {
      return MP_VAL;
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
   if ((err = mp_init(&inv)) != MP_OKAY) {
      goto __B;
   }

   while (B.used > 0) {
       if ((err = pb_mod(&A, &B, &tmp)) != MP_OKAY) {
          goto __INV;
       }
       /* A = B, B = tmp */
       if ((err = pb_copy(&B, &A)) != MP_OKAY) {
          goto __INV;
       }
       if ((err = pb_copy(&tmp, &B)) != MP_OKAY) {
          goto __INV;
       }
   }

   /* if degree of A is one we set to constant */
   if (A.used == 1) {
      mp_set(&(A.terms[0]), 1);
   }

   /* ensure it's monic */
   if (mp_cmp_d(&(A.terms[A.used-1]), 1) != MP_EQ) {
      /* work it up... */
      if ((err = mp_invmod(&(A.terms[A.used-1]), &(c->characteristic), &inv)) != MP_OKAY) {
         goto __INV;
      }
      for (x = 0; x < A.used; x++) {
          if ((err = mp_mulmod(&(A.terms[x]), &inv, &(c->characteristic), &(A.terms[x]))) != MP_OKAY) { 
             goto __INV;
          }
      }
   }
   
   pb_exch(c, &A);
   err = MP_OKAY;

__INV: mp_clear(&inv);
__B  : pb_clear(&B);
__A  : pb_clear(&A);
__TMP: pb_clear(&tmp);
   return err;
}


