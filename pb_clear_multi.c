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
#include <stdarg.h>

void pb_clear_multi(pb_poly *mp, ...) 
{
    pb_poly* next_mp = mp;
    va_list args;
    va_start(args, mp);
    while (next_mp != NULL) {
        pb_clear(next_mp);
        next_mp = va_arg(args, pb_poly*);
    }
    va_end(args);
}
