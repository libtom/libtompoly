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

int pb_grow(pb_poly *a, int size)
{
   mp_int *tmp;
   int x, err;

   if (a->alloc < size) {
      /* pad size upwards */
      size += (PB_TERMS - (size % PB_TERMS));

      /* regrow terms */
      tmp = realloc(a->terms, sizeof(mp_int) * size);
      if (tmp == NULL) {
         return MP_MEM;
      }
      a->terms = tmp;
  
      /* zero the new stuff (prevents mp_clear from double freeing) */
      memset(a->terms + a->alloc, 0, (size - a->alloc) * sizeof(mp_int));

      /* now init the terms */
      for (x = a->alloc; x < size; x++) {
          if ((err = mp_init(&(a->terms[x]))) != MP_OKAY) {
             return err;
          }
      }

      /* update info */
      a->alloc = size;
   }
   return MP_OKAY;
}


