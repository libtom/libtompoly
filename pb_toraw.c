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

int pb_toraw(pb_poly *a, unsigned char *dst)
{
   int err, x, y, z;

   /* store the # of terms */
   dst[0] = a->used & 255;
   dst[1] = (a->used >> 8) & 255;
   y      = 2;

   /* store the characteristic */
   z = mp_signed_bin_size(&(a->characteristic));
   dst[y++] = z&255;
   dst[y++] = (z>>8)&255;
   if ((err = mp_to_signed_bin(&(a->characteristic), dst+y)) != MP_OKAY)          { return err; }
   y += z;

   for (x = 0; x < a->used; x++) {
      z = mp_signed_bin_size(&(a->terms[x]));
      dst[y++] = z&255;
      dst[y++] = (z>>8)&255;
      if ((err = mp_to_signed_bin(&(a->terms[x]), dst+y)) != MP_OKAY)             { return err; }
      y += z;
   }

   return MP_OKAY;
}



