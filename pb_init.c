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

/* initialize a */
int pb_init(pb_poly *a, mp_int *characteristic)
{
   int x, y, err;

   /* init characteristic */
   if ((err = mp_init_copy(&(a->characteristic), characteristic)) != MP_OKAY) {
      return err;
   }

   /* now allocate an array of mp_ints */
   if ((a->terms = calloc(PB_TERMS, sizeof(mp_int))) == NULL) {
      mp_clear(&(a->characteristic));
      return MP_MEM;
   }

   /* now initialize them all */
   for (x = 0; x < PB_TERMS; x++) {
       if ((err = mp_init(&(a->terms[x]))) != MP_OKAY) {
          /* free stuff */
          for (y = 0; y < x; y++) {
              mp_clear(&(a->terms[y]));
          }
          free(a->terms);
          mp_clear(&(a->characteristic));
          return err;
      }
   }

   /* set our parameters */
   a->used  = 0;
   a->alloc = PB_TERMS;
   return MP_OKAY;
}


