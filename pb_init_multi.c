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

int pb_init_multi(mp_int *characteristic, pb_poly *pb, ...) 
{
    mp_err res = MP_OKAY;      /* Assume ok until proven otherwise */
    int n = 0;                 /* Number of ok inits */
    pb_poly* cur_arg = pb;
    va_list args;

    va_start(args, pb);        /* init args to next argument from caller */
    while (cur_arg != NULL) {
        if (pb_init(cur_arg, characteristic) != MP_OKAY) {
            /* Oops - error! Back-track and mp_clear what we already
               succeeded in init-ing, then return error.
            */
            va_list clean_args;
            
            /* end the current list */
            va_end(args);
            
            /* now start cleaning up */            
            cur_arg = pb;
            va_start(clean_args, pb);
            while (n--) {
                pb_clear(cur_arg);
                cur_arg = va_arg(clean_args, pb_poly*);
            }
            va_end(clean_args);
            res = MP_MEM;
            break;
        }
        n++;
        cur_arg = va_arg(args, pb_poly*);
    }
    va_end(args);
    return res;                /* Assumed ok, if error flagged above. */
}

