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

#ifdef MP_LOW_MEM
   #define TAB_SIZE 32
#else
   #define TAB_SIZE 256
#endif

int pb_exptmod (pb_poly * G, mp_int * X, pb_poly * P, pb_poly * Y)
{
  pb_poly  M[TAB_SIZE], res;
  mp_digit buf;
  int     err, bitbuf, bitcpy, bitcnt, mode, digidx, x, y, winsize;

  /* find window size */
  x = mp_count_bits (X);
  if (x <= 7) {
    winsize = 2;
  } else if (x <= 36) {
    winsize = 3;
  } else if (x <= 140) {
    winsize = 4;
  } else if (x <= 450) {
    winsize = 5;
  } else if (x <= 1303) {
    winsize = 6;
  } else if (x <= 3529) {
    winsize = 7;
  } else {
    winsize = 8;
  }

#ifdef MP_LOW_MEM
    if (winsize > 5) {
       winsize = 5;
    }
#endif

  /* init M array */
  /* init first cell */
  if ((err = pb_init(&M[1], &(Y->characteristic))) != MP_OKAY) {
     return err; 
  }

  /* now init the second half of the array */
  for (x = 1<<(winsize-1); x < (1 << winsize); x++) {
    if ((err = pb_init(&M[x], &(Y->characteristic))) != MP_OKAY) {
      for (y = 1<<(winsize-1); y < x; y++) {
        pb_clear (&M[y]);
      }
      pb_clear(&M[1]);
      return err;
    }
  }

  /* create M table
   *
   * The M table contains powers of the base, 
   * e.g. M[x] = G**x mod P
   *
   * The first half of the table is not 
   * computed though accept for M[0] and M[1]
   */

  if (X->sign == MP_ZPOS) {
     if ((err = pb_mod (G, P, &M[1])) != MP_OKAY)                             { goto __M; }
  } else {
     if ((err = pb_invmod(G, P, &M[1])) != MP_OKAY)                           { goto __M; }
  }

  /* compute the value at M[1<<(winsize-1)] by squaring 
   * M[1] (winsize-1) times 
   */
  if ((err = pb_copy (&M[1], &M[1 << (winsize - 1)])) != MP_OKAY)             { goto __M; }

  for (x = 0; x < (winsize - 1); x++) {
    if ((err = pb_mulmod (&M[1 << (winsize - 1)], &M[1 << (winsize - 1)],
                          P, &M[1 << (winsize - 1)])) != MP_OKAY)             { goto __M; }
  }

  /* create upper table, that is M[x] = M[x-1] * M[1] (mod P)
   * for x = (2**(winsize - 1) + 1) to (2**winsize - 1)
   */
  for (x = (1 << (winsize - 1)) + 1; x < (1 << winsize); x++) {
    if ((err = pb_mulmod (&M[x - 1], &M[1], P, &M[x])) != MP_OKAY)            { goto __M; }
  }

  /* setup result */
  if ((err = pb_init (&res, &(Y->characteristic))) != MP_OKAY)                { goto __M; }
  mp_set (&(res.terms[0]), 1); res.used = 1;

  /* set initial mode and bit cnt */
  mode   = 0;
  bitcnt = 1;
  buf    = 0;
  digidx = X->used - 1;
  bitcpy = 0;
  bitbuf = 0;

  for (;;) {
    /* grab next digit as required */
    if (--bitcnt == 0) {
      /* if digidx == -1 we are out of digits */
      if (digidx == -1) {
        break;
      }
      /* read next digit and reset the bitcnt */
      buf    = X->dp[digidx--];
      bitcnt = (int) DIGIT_BIT;
    }

    /* grab the next msb from the exponent */
    y     = (buf >> (mp_digit)(DIGIT_BIT - 1)) & 1;
    buf <<= (mp_digit)1;

    /* if the bit is zero and mode == 0 then we ignore it
     * These represent the leading zero bits before the first 1 bit
     * in the exponent.  Technically this opt is not required but it
     * does lower the # of trivial squaring/reductions used
     */
    if (mode == 0 && y == 0) {
      continue;
    }

    /* if the bit is zero and mode == 1 then we square */
    if (mode == 1 && y == 0) {
      if ((err = pb_mulmod (&res, &res, P, &res)) != MP_OKAY)                 { goto __RES; }
      continue;
    }

    /* else we add it to the window */
    bitbuf |= (y << (winsize - ++bitcpy));
    mode    = 2;

    if (bitcpy == winsize) {
      /* ok window is filled so square as required and multiply  */
      /* square first */
      for (x = 0; x < winsize; x++) {
        if ((err = pb_mulmod (&res, &res, P, &res)) != MP_OKAY)               { goto __RES; }
      }

      /* then multiply */
      if ((err = pb_mulmod (&res, &M[bitbuf], P, &res)) != MP_OKAY)           { goto __RES; }

      /* empty window and reset */
      bitcpy = 0;
      bitbuf = 0;
      mode   = 1;
    }
  }

  /* if bits remain then square/multiply */
  if (mode == 2 && bitcpy > 0) {
    /* square then multiply if the bit is set */
    for (x = 0; x < bitcpy; x++) {
      if ((err = pb_mulmod (&res, &res, P, &res)) != MP_OKAY)                 { goto __RES; }

      bitbuf <<= 1;
      if ((bitbuf & (1 << winsize)) != 0) {
        /* then multiply */
        if ((err = pb_mulmod (&res, &M[1], P, &res)) != MP_OKAY)              { goto __RES; }
      }
    }
  }

  pb_exch (&res, Y);
  err = MP_OKAY;
__RES:pb_clear (&res);
__M:
  pb_clear(&M[1]);
  for (x = 1<<(winsize-1); x < (1 << winsize); x++) {
    pb_clear (&M[x]);
  }
  return err;
}
