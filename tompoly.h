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
#ifndef TOMPOLY_H_
#define TOMPOLY_H_

#include <tommath.h>

/* this structure holds a polynomial */
typedef struct {
   int    used,                  /* number of terms */
          alloc;                 /* number of terms available (total) */
   mp_int characteristic,        /* characteristic, zero if not finite */
          *terms;                /* terms of polynomial */
} pb_poly;


/* default number of terms */
#define PB_TERMS        4

/* Compare codes */
#define PB_EQ           0        /* They're exactly equal */
#define PB_DEG_LT       1        /* The left has a lower degree */
#define PB_DEG_EQ       2        /* same degree  */
#define PB_DEG_GT       3        /* The left has a higher degree */

int pb_init(pb_poly *a, mp_int *characteristic);
int pb_init_size(pb_poly *a, mp_int *characteristic, int size);
int pb_init_copy(pb_poly *a, pb_poly *b);
int pb_init_multi(mp_int *characteristic, pb_poly *pb, ...);
void pb_clear_multi(pb_poly *mp, ...);
void pb_clear(pb_poly *a);

int pb_grow(pb_poly *a, int size);
void pb_clamp(pb_poly *a);

/* dest(x) := src(x) */
int pb_copy(pb_poly *src, pb_poly *dest);

/* compare these */
int pb_cmp(pb_poly *a, pb_poly *b);

/* swap contents of a(x) and b(x) */
void pb_exch(pb_poly *a, pb_poly *b);

/* a(x) = 0 */
void pb_zero(pb_poly *a);

/* a(x) = a(x) / I(x)^x */
int pb_rshd(pb_poly *a, int x);

/* a(x) = a(x) * I(x)^x */
int pb_lshd(pb_poly *a, int x);

/* c(x) = a(x) + b(x) */
int pb_add(pb_poly *a, pb_poly *b, pb_poly *c);

/* c(x) = a(x) - b(x) */
int pb_sub(pb_poly *a, pb_poly *b, pb_poly *c);

/* c(x) = a(x) * b(x) */
int pb_mul(pb_poly *a, pb_poly *b, pb_poly *c);

/* c(x) * b(x) + d(x) = a(x) */
int pb_div(pb_poly *a, pb_poly *b, pb_poly *c, pb_poly *d);

/* c(x) = a(x) mod b(x) */
int pb_mod(pb_poly *a, pb_poly *b, pb_poly *c);

/* d(x) = (a(x) + b(x)) mod c(x) */
int pb_addmod(pb_poly *a, pb_poly *b, pb_poly *c, pb_poly *d);

/* d(x) = (a(x) - b(x)) mod c(x) */
int pb_submod(pb_poly *a, pb_poly *b, pb_poly *c, pb_poly *d);

/* d(x) = (a(x) * b(x)) mod c(x) */
int pb_mulmod(pb_poly *a, pb_poly *b, pb_poly *c, pb_poly *d);

/* returns the monic GCD of a,b in GF(p^k)[x] */
int pb_gcd(pb_poly *a, pb_poly *b, pb_poly *c);

int pb_invmod(pb_poly *a, pb_poly *b, pb_poly *c);


#endif
