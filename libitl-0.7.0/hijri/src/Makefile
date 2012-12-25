# $Id: Makefile,v 1.5 2006/09/21 16:35:46 thamer Exp $

include ../../build/config.mk

all: demo_hijri

demo_hijri: hijri.o umm_alqura.o demo_hijri.c
	@echo "==> Building hijri demo..."
	$(CC) $(CFLAGS) hijri.o umm_alqura.o demo_hijri.c -o demo_hijri -lm

hijri.o: hijri.c hijri.h
	@echo "==> Building hijri component..."
	$(CC) $(CFLAGS) -c hijri.c -o hijri.o

umm_alqura.o: umm_alqura.c hijri.h
	@echo "==> Building umm_alqura component..."
	$(CC) $(CFLAGS) -c umm_alqura.c -o umm_alqura.o

libitl: all
	@echo "==> Create symbolic links for library generation..."
	test -d ../../build/itl/ \
	|| mkdir -p ../../build/itl/ \
	|| exit 1;
	ln -sf ../../hijri/src/hijri.h ../../build/itl/hijri.h
	ln -sf ../hijri/src/hijri.o ../../build/hijri.o
	ln -sf ../hijri/src/umm_alqura.o ../../build/umm_alqura.o

libitl-libtool:
	libtool --mode=compile $(CC) -c hijri.c
	libtool --mode=compile $(CC) -c umm_alqura.c

clean:
	rm -f *.o *~ demo_hijri
	rm -f *.lo
	rm -rf .libs