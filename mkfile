</$objtype/mkfile
LIB=/$objtype/lib/ape/libtompoly.a
OFILES=pb_add.$O pb_addmod.$O pb_clamp.$O pb_clear.$O pb_clear_multi.$O pb_cmp.$O \
	pb_copy.$O pb_div.$O pb_exch.$O pb_exptmod.$O pb_exteuclid.$O pb_gcd.$O \
	pb_grow.$O pb_init.$O pb_init_copy.$O pb_init_multi.$O pb_init_size.$O \
	pb_invmod.$O pb_isirreduc.$O pb_lshd.$O pb_mod.$O pb_monic.$O pb_mul.$O \
	pb_mulmod.$O pb_rawsize.$O pb_readraw.$O pb_rshd.$O pb_shrink.$O pb_sub.$O \
	pb_submod.$O pb_toraw.$O pb_zero.$O 
HFILES=tompoly.h /sys/include/ape/tommath_class.h /sys/include/ape/tommath_superclass.h \
	/sys/include/ape/tommath_class.h /sys/include/ape/tommath_superclass.h \
	/sys/include/ape/tommath_class.h /sys/include/ape/tommath_superclass.h \
	/sys/include/ape/tommath_class.h /sys/include/ape/limits.h /sys/include/ape/ctype.h \
	/sys/include/ape/stddef.h /sys/include/ape/stdlib.h /sys/include/ape/bsd.h \
	/sys/include/ape/stddef.h /sys/include/ape/string.h /sys/include/ape/sys/types.h \
	/sys/include/ape/stddef.h /$objtype/include/ape/stdarg.h /sys/include/ape/stdio.h \
	/sys/include/ape/tommath.h 

</sys/src/cmd/mksyslib

CC=pcc
LD=pcc
CFLAGS=-c -I. -I/sys/include/ape -I/$objtype/include/ape -B -D_POSIX_SOURCE -D_SUSV2_SOURCE \
	-D_BSD_EXTENSION 

install:V:
	cp tompoly.h /sys/include/ape/

nuke:V:
	mk clean
	rm -f $LIB
	rm -f /sys/include/ape/tompoly.h
