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

int pb_rawsize(pb_poly *a)
{
   int x, y;

   /* 2 bytes for # of coefficients */
   y = 2;
   
   /* characteristic */
   y += 2 + mp_signed_bin_size(&(a->characteristic));

   /* all of the terms */
   for (x = 0; x < a->used; x++) {
       y += 2 + mp_signed_bin_size(&(a->terms[x]));
   }
   
   return y;
}


