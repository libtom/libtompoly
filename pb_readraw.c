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

int pb_readraw(pb_poly *a, unsigned char *buf, int len)
{
   int terms, x, y, z, err;

   /* zero poly */
   pb_zero(a);

   /* must be at least four bytes */
   if (len < 4) {
      return MP_VAL;
   }
 
   /* number of terms */
   terms = ((unsigned)buf[0]) | ((unsigned)buf[1]<<8);
   y     = 2;

   /* grow to the right size */
   if (a->alloc < terms) {
      if ((err = pb_grow(a, terms)) != MP_OKAY) {
         return err;
      }
   }

   /* read characteristic */
   z = ((unsigned)buf[y]) | ((unsigned)buf[y+1]<<8); y += 2;
   if (z + y > len)                                                                   { return MP_VAL; }
   if ((err = mp_read_signed_bin(&(a->characteristic), buf+y, z)) != MP_OKAY)         { return err; }
   y += z;


   /* read terms */
   for (x = 0; x < terms; x++) {
      z = ((unsigned)buf[y]) | ((unsigned)buf[y+1]<<8); y += 2;
      if (z + y > len)                                                                { return MP_VAL; }
      if ((err = mp_read_signed_bin(&(a->characteristic), buf+y, z)) != MP_OKAY)      { return err; }
      y += z;
   }

   return MP_OKAY;
}


