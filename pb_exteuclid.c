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

/* Extended euclidean algorithm of (a, b) produces 
   a*u1 + b*u2 = u3
 */
int pb_exteuclid(pb_poly *a, pb_poly *b, pb_poly *U1, pb_poly *U2, pb_poly *U3)
{
   pb_poly u1,u2,u3,v1,v2,v3,t1,t2,t3,q,tmp;
   int err;

   if ((err = pb_init_multi(&(b->characteristic),
        &u1, &u2, &u3, &v1, &v2, &v3, &t1, &t2, &t3, &q, &tmp, NULL)) != MP_OKAY) {
      return err;
   }

   /* initialize, (u1,u2,u3) = (1,0,a) */
   mp_set(&(u1.terms[0]), 1); u1.used = 1;
   if ((err = pb_copy(a, &u3)) != MP_OKAY)                                        { goto _ERR; }

   /* initialize, (v1,v2,v3) = (0,1,b) */
   mp_set(&(v2.terms[0]), 1); v2.used = 1;
   if ((err = pb_copy(b, &v3)) != MP_OKAY)                                        { goto _ERR; }

   /* loop while v3 != 0 */
   while (v3.used > 0) {
       /* q = u3/v3 */
       if ((err = pb_div(&u3, &v3, &q, NULL)) != MP_OKAY)                         { goto _ERR; }

       /* (t1,t2,t3) = (u1,u2,u3) - (v1,v2,v3)q */
       if ((err = pb_mul(&v1, &q, &tmp)) != MP_OKAY)                              { goto _ERR; }
       if ((err = pb_sub(&u1, &tmp, &t1)) != MP_OKAY)                             { goto _ERR; }
       if ((err = pb_mul(&v2, &q, &tmp)) != MP_OKAY)                              { goto _ERR; }
       if ((err = pb_sub(&u2, &tmp, &t2)) != MP_OKAY)                             { goto _ERR; }
       if ((err = pb_mul(&v3, &q, &tmp)) != MP_OKAY)                              { goto _ERR; }
       if ((err = pb_sub(&u3, &tmp, &t3)) != MP_OKAY)                             { goto _ERR; }

       /* (u1,u2,u3) = (v1,v2,v3) */
       if ((err = pb_copy(&v1, &u1)) != MP_OKAY)                                  { goto _ERR; }
       if ((err = pb_copy(&v2, &u2)) != MP_OKAY)                                  { goto _ERR; }
       if ((err = pb_copy(&v3, &u3)) != MP_OKAY)                                  { goto _ERR; }

       /* (v1,v2,v3) = (t1,t2,t3) */
       if ((err = pb_copy(&t1, &v1)) != MP_OKAY)                                  { goto _ERR; }
       if ((err = pb_copy(&t2, &v2)) != MP_OKAY)                                  { goto _ERR; }
       if ((err = pb_copy(&t3, &v3)) != MP_OKAY)                                  { goto _ERR; }
   }

   /* reduce U3 to monic but we have to mangle all three to remain consistent */
   pb_zero(&v1);
   if ((err = mp_copy(&(u3.terms[u3.used-1]), &(v1.terms[0]))) != MP_OKAY)        { goto _ERR; }
   v1.used = 1;

   /* copy result out */
   if (U1 != NULL) { if ((err = pb_div(&u1, &v1, U1, NULL)) != MP_OKAY)           { goto _ERR; }}
   if (U2 != NULL) { if ((err = pb_div(&u2, &v1, U2, NULL)) != MP_OKAY)           { goto _ERR; }}
   if (U3 != NULL) { if ((err = pb_div(&u3, &v1, U3, NULL)) != MP_OKAY)           { goto _ERR; }}

   err = MP_OKAY;
_ERR: pb_clear_multi(&u1, &u2, &u3, &v1, &v2, &v3, &t1, &t2, &t3, &q, &tmp, NULL);
   return err;
}


