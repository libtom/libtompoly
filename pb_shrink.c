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

int pb_shrink(pb_poly *a)
{
   int err, x;
   mp_int *tmp;

   /* first resize the array of terms */
   if (a->used != a->alloc && a->used > 0) {
      /* free the mp_ints */
      for (x = a->used; x < a->alloc; x++) {
          mp_clear(&(a->terms[x]));
      }

      /* resize the array of pointers */
      tmp = realloc(a->terms, sizeof(mp_int) * a->used);
      if (tmp == NULL) {
         return MP_MEM;
      }
      a->terms = tmp;
      a->alloc = a->used;
   }

   /* now shrink each mp_int */
   for (x = 0; x < a->used; x++) {
       if ((err = mp_shrink(&(a->terms[x]))) != MP_OKAY) {
          return err;
       }
   }
   return MP_OKAY;
}


