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

int pb_add(pb_poly *a, pb_poly *b, pb_poly *c)
{
   int err, x, y, z, characteristic;
   pb_poly *tmp;

   /* grow c to be the max size */
   y = MAX(a->used, b->used);
   if (c->alloc < y) {
      if ((err = pb_grow(c, y)) != MP_OKAY) {
         return err;
      }
   }
   
   /* do we need to concern char */
   characteristic = mp_iszero(&(c->characteristic));

   /* add the terms */
   z = MIN(a->used, b->used);
   for (x = 0; x < z; x++) {
       if ((err = mp_add(&(a->terms[x]), &(b->terms[x]), &(c->terms[x]))) != MP_OKAY) {
          return err;
       }
       if (characteristic == MP_NO) {
          if ((err = mp_mod(&(c->terms[x]), &(c->characteristic), &(c->terms[x]))) != MP_OKAY) {
             return err;
          }
       }
   }

   /* excess digits? */
   if (y != z) {
       if (a->used == y) {
          tmp = a;
       } else {
          tmp = b;
       }
       for (x = z; x < y; x++) {
          if (characteristic == MP_NO) {
             if ((err = mp_mod(&(tmp->terms[x]), &(c->characteristic), &(c->terms[x]))) != MP_OKAY) {
                return err;
             }
          } else {
             if ((err = mp_copy(&(tmp->terms[x]), &(c->terms[x]))) != MP_OKAY) {
                return err;
             }
          }
       }
   }
   
   /* zero excess */
   for (x = y; x < c->used; x++) {
       mp_zero(&(c->terms[x]));
   }
   c->used = y;
   pb_clamp(c);

   return MP_OKAY;
}


