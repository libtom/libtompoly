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

void pb_clear(pb_poly *a)
{
   int x;

   if (a->terms != NULL) {
      /* free stuff */
      for (x = 0; x < a->alloc; x++) {
          mp_clear(&(a->terms[x]));
      }
      free(a->terms);
      mp_clear(&(a->characteristic));
 
      /* prevent double frees */
      a->terms = NULL;
      a->alloc = a->used = 0;
   }
}


