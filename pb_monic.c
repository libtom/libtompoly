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

/* makes b equal to the monic polynomial form of a */
int pb_monic(pb_poly *a, pb_poly *b)
{
   mp_int tmp;
   int err, x;

   /* must be in GF(p)[x] */
   if (mp_iszero(&(b->characteristic)) == MP_YES) {
      return MP_VAL;
   }

   /* if it's already monic  just copy */
   if (a->used == 0 || mp_cmp_d(&(a->terms[a->used - 1]), 1) == MP_EQ) {
      return pb_copy(a, b);
   }

   /* grow b to hold result */
   if (b->alloc < a->used) {
      if ((err = pb_grow(b, a->used)) != MP_OKAY) {
         return err;
      }
   }

   /* now init tmp and find the inverse of the leading digit */
   if ((err = mp_init(&tmp)) != MP_OKAY) {
      return err;
   }

   if ((err = mp_invmod(&(a->terms[a->used-1]), &(b->characteristic), &tmp)) != MP_OKAY)             { goto _ERR; }

   /* now reduce each coefficient */
   for (x = 0; x < a->used; x++) {
       if ((err = mp_mulmod(&(a->terms[x]), &tmp, &(b->characteristic), &(b->terms[x]))) != MP_OKAY) { goto _ERR; }
   }

   /* zero excess digits */
   for (x = a->used; x < b->used; x++) {
       mp_zero(&(b->terms[x]));
   }
   b->used = a->used;

   err = MP_OKAY;
_ERR: mp_clear(&tmp);
   return err;
}


