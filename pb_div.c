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


/* Divides a by b (uses the char of b as the char of the result) only for 
 * polynomials over GF(p^k)[x] 
 */
int pb_div(pb_poly *a, pb_poly *b, pb_poly *c, pb_poly *d)
{
   int     err, x;
   pb_poly p, q, r;
   mp_int  tmp, tmp2;   

   /* if ||b|| > ||a|| it's a simple copy over */
   if (b->used > a->used) {
      if (d != NULL) {
         pb_copy(a, d); 
      }
      if (c != NULL) {
         pb_zero(c);
      }
   }

   /* zero divisor */
   if (b->used == 0) {
      return MP_VAL;
   }

   /* compare chars */
   if (mp_cmp(&(a->characteristic), &(b->characteristic)) != MP_EQ) {
      return MP_VAL;
   }

   /* we don't support char zero! */
   if (mp_iszero(&(b->characteristic)) == MP_YES) {
      return MP_VAL;
   }

   /* get a copy of "a" to work with */
   if ((err = pb_init_copy(&p, a)) != MP_OKAY) {
      return err;
   }

   /* init a temp quotient */
   if ((err = pb_init_size(&q, &(b->characteristic), a->used - b->used + 1)) != MP_OKAY) {
      goto __P;
   }

   /* init a temp polynomial we can work with */
   if ((err = pb_init_size(&r, &(b->characteristic), a->used)) != MP_OKAY) {
      goto __Q;
   }

   /* now init an mp_int we can work with */
   if ((err = mp_init(&tmp)) != MP_OKAY) {
      goto __R;
   }

   /* and the inverse of the leading digit of b(x) */
   if ((err = mp_init(&tmp2)) != MP_OKAY) {
      goto __TMP;
   }
   if ((err = mp_invmod(&(b->terms[b->used - 1]), &(b->characteristic), &tmp2)) != MP_OKAY) {
      goto __TMP2;
   }

   /* now let's reduce us some digits */
   for (x = a->used - 1; x >= b->used-1; x--) { 
      /* skip zero digits */
      if (mp_iszero(&(p.terms[x])) == MP_YES) {
          continue;
      }

      /* now we have a leading digit of p(x), call it A
         and a leading digit of b(x), call it B
     
         Now we want to reduce it so we need A + CB = 0 which turns into
         A + CB = 0
         CB = -A
         C = -A/B

         So we multiply b(x) by C * x^k [where k = ||p(x)|| - ||b(x)||], add that to p(x) and 
         we must reduced one digit 
      */
      
      /* multiply 1/B [tmp2] by A */
      if ((err = mp_mulmod(&tmp2, &(p.terms[x]), &(b->characteristic), &tmp)) != MP_OKAY)    { goto __TMP2; }

      /* tmp is now a term of the quotient */
      if ((err = mp_copy(&tmp, &(q.terms[x - b->used + 1]))) != MP_OKAY)                     { goto __TMP2; }
       
      /* create r(x) = C */
      pb_zero(&r);
      if ((err = mp_copy(&tmp, &(r.terms[0]))) != MP_OKAY)                                   { goto __TMP2; }
      r.used = 1;

      /* now multiply r(x) by b(x)*x^k and subtract from p(x) */
      if ((err = pb_mul(b, &r, &r)) != MP_OKAY)                                              { goto __TMP2; }
      if ((err = pb_lshd(&r, x - b->used + 1)) != MP_OKAY)                                   { goto __TMP2; }
      if ((err = pb_sub(&p, &r, &p)) != MP_OKAY)                                             { goto __TMP2; }
   }

   /* setup q properly (so q.used is valid) */
   q.used = a->used;
   pb_clamp(&q);

   /* ok so now p(x) is the remainder and q(x) is the quotient */
   if (c != NULL) {
      pb_exch(&q, c);
   } 
   if (d != NULL) {
      pb_exch(&p, d);
   }
  
   err = MP_OKAY;     

__TMP2: mp_clear(&tmp2);
__TMP : mp_clear(&tmp);
__R   : pb_clear(&r);   
__Q   : pb_clear(&q);
__P   : pb_clear(&p);
   return err;
}


