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

/* is a(x) irreducible?  */
int pb_isirreduc(pb_poly *a, int *res)
{
   pb_poly u, tmp, fm, d;
   int err, i;

   /* default to no */
   *res = MP_NO;

   /* init temps */
   if ((err = pb_init_multi(&(a->characteristic), 
       &u, &tmp, &fm, &d, NULL)) != MP_OKAY) {
       return err;
   }

   /* fm = monic(a(x)) */
   if ((err = pb_monic(a, &fm)) != MP_OKAY)                                   { goto _ERR; }
  
   /* u  = x */
   mp_set(&(u.terms[1]), 1); u.used = 2;

   /* loop */
   for (i = 1; i <= (a->used / 2); i++) {
       /* u = u^p mod fm */
       if ((err = pb_exptmod(&u, &(a->characteristic), &fm, &u)) != MP_OKAY)  { goto _ERR; }
 
       /* tmp = u - x */
       pb_zero(&tmp);
       mp_set(&(tmp.terms[1]), 1); tmp.used = 2;
       if ((err = pb_sub(&u, &tmp, &tmp)) != MP_OKAY)                         { goto _ERR; }

       /* d = gcd(fm, tmp) */
       if ((err = pb_gcd(&fm, &tmp, &d)) != MP_OKAY)                          { goto _ERR; }
       
       /* if d != 1 then reducible */
       if (d.used > 1)                                                        { err = MP_OKAY; goto _ERR; }
   }
   
   /* irreducible */
   *res = MP_YES;
   err  = MP_OKAY;
_ERR: pb_clear_multi(&u, &tmp, &fm, &d, NULL);
   return err;
}


