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

int pb_invmod(pb_poly *a, pb_poly *b, pb_poly *c)
{
   pb_poly m, n, p0, p1, p2, r, q, tmp;
   int err;
 
   if ((err = pb_init_multi(&(c->characteristic),  &m, &n, &p0,
                            &p1, &p2, &r, &q, &tmp, NULL)) != MP_OKAY) {
      return err;
   }

   /* init system */

   /* m = b */
   if ((err = pb_copy(b, &m)) != MP_OKAY)                          { goto _ERR; }

   /* n = a */
   if ((err = pb_copy(a, &n)) != MP_OKAY)                          { goto _ERR; }

   /* p0 = 1 */
   mp_set(&(p0.terms[0]), 1);
   p0.used = 1;

   /* m/n => p1,r */ 
   if ((err = pb_div(&m, &n, &p1, &r)) != MP_OKAY)                 { goto _ERR; }

   /* q = p1 */
   if ((err = pb_copy(&p1, &q)) != MP_OKAY)                        { goto _ERR; }

   while (r.used > 0) {
      if ((err = pb_copy(&n, &m)) != MP_OKAY)                      { goto _ERR; }
      if ((err = pb_copy(&r, &n)) != MP_OKAY)                      { goto _ERR; }
      if ((err = pb_div(&m, &n, &q, &r)) != MP_OKAY)               { goto _ERR; }
      if ((err = pb_mul(&q, &p1, &tmp)) != MP_OKAY)                { goto _ERR; }
      if ((err = pb_sub(&p0, &tmp, &p2)) != MP_OKAY)               { goto _ERR; }
      if ((err = pb_copy(&p1, &p0)) != MP_OKAY)                    { goto _ERR; }
      if ((err = pb_copy(&p2, &p1)) != MP_OKAY)                    { goto _ERR; }
   }

    /* If b has an inverse mod a, then n will be a constant */
    if (n.used > 1)                                                { goto _ERR; }
    if ((err = pb_div(&p0, &n, &q, &r)) != MP_OKAY)                { goto _ERR; }
    pb_exch(c, &q);   

   err = MP_OKAY;
_ERR:   pb_clear_multi(&m, &n, &p0, &p1, &p2, &r, &q, &tmp, NULL);
   return err;
}


