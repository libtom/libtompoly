#Makefile for GCC by Tom St Denis
CFLAGS += -I. -Os -Wall -W 

VERSION=0.04

#default files to install
LIBNAME=libtompoly.a
HEADERS=tompoly.h

#LIBPATH-The directory for libtomcrypt to be installed to.
#INCPATH-The directory to install the header files for libtommath.
#DATAPATH-The directory to install the pdf docs.
DESTDIR=
LIBPATH=/usr/lib
INCPATH=/usr/include
DATAPATH=/usr/share/doc/libtompoly/pdf

default: libtompoly.a

OBJECTS = pb_init.o pb_clear.o pb_init_size.o pb_grow.o pb_copy.o pb_clamp.o pb_init_copy.o \
pb_add.o pb_sub.o pb_mul.o pb_div.o pb_zero.o pb_lshd.o pb_rshd.o pb_exch.o pb_mod.o \
pb_addmod.o pb_submod.o pb_mulmod.o pb_gcd.o pb_init_multi.o pb_clear_multi.o pb_invmod.o \
pb_cmp.o pb_shrink.o pb_exteuclid.o pb_monic.o pb_exptmod.o pb_isirreduc.o pb_rawsize.o \
pb_toraw.o pb_readraw.o

libtompoly.a: $(OBJECTS)
	ar $(ARFLAGS) libtompoly.a $(OBJECTS)

install: libtompoly.a
	install -d -g root -o root $(DESTDIR)$(LIBPATH)
	install -d -g root -o root $(DESTDIR)$(INCPATH)
	install -g root -o root $(LIBNAME) $(DESTDIR)$(LIBPATH)
	install -g root -o root $(HEADERS) $(DESTDIR)$(INCPATH)

demo: demo/demo.o libtompoly.a
	gcc demo/demo.o libtompoly.a -ltommath -o pbdemo

mandvi: pb.tex
	echo "hello" > pb.ind
	latex pb > /dev/null
	latex pb > /dev/null
	makeindex pb
	latex pb > /dev/null

manual:	mandvi
	pdflatex pb >/dev/null

clean:
	rm -f *.o *.a *.obj *.lib *.exe pbdemo demo/*.o demo/*.obj demo/*.exe
	rm -f *.idx *.ilg *.ind *.lof *.out *.toc *.dvi *.log *.aux

zipup: manual clean
	cd .. ; rm -rf ltp* libtompoly-$(VERSION) ; mkdir libtompoly-$(VERSION) ; \
	cp -R ./libtompoly/* ./libtompoly-$(VERSION)/ ; \
	tar -c libtompoly-$(VERSION)/* | bzip2 -9vvc > ltp-$(VERSION).tar.bz2 ; \
	zip -9 -r ltp-$(VERSION).zip libtompoly-$(VERSION)/*
