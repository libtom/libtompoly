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

int pb_mul(pb_poly *a, pb_poly *b, pb_poly *c)
{
   int err, x, y;
   pb_poly tmp;
   mp_int  tmp2;

   /* make temp */
   if ((err = pb_init_size(&tmp, &(c->characteristic), a->used + b->used)) != MP_OKAY) {
      return err;
   }
 
   if ((err = mp_init(&tmp2)) != MP_OKAY) {
      goto __TMP;
   }

   /* multiply the terms */
   for (x = 0; x < a->used; x++) {
       for (y = 0; y < b->used; y++) {
           if ((err = mp_mul(&(a->terms[x]), &(b->terms[y]), &tmp2)) != MP_OKAY) {
              goto __TMP2;
           }
           if ((err = mp_add(&tmp2, &(tmp.terms[x+y]), &(tmp.terms[x+y]))) != MP_OKAY) { 
              goto __TMP2;
           }
       }
   }
   
   /* now reduce the terms as required */
   if (mp_iszero(&(c->characteristic)) == MP_NO) {
      for (x = 0; x < (a->used + b->used); x++) {
          if ((err = mp_mod(&(tmp.terms[x]), &(c->characteristic), &(tmp.terms[x]))) != MP_OKAY) {
             goto __TMP2;
          }
      }
   }

   /* set digit count */
   tmp.used = a->used + b->used;
   pb_clamp(&tmp);

   /* exchange tmp and c */
   pb_exch(&tmp, c);

   err = MP_OKAY;

__TMP2:  mp_clear(&tmp2);
__TMP :  pb_clear(&tmp);
   return err;
}
