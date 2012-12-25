# $Id: Makefile,v 1.8 2006/09/21 16:41:43 thamer Exp $

include ../../build/config.mk

all: demo_prayer

demo_prayer: astro.o prayer.o demo_prayer.c
	@echo "==> Building demo..."
	$(CC) $(CFLAGS) astro.o prayer.o demo_prayer.c -o demo_prayer -lm

prayer.o: prayer.c prayer.h astro.h
	@echo "==> Building libprayer components..."
	$(CC) $(CFLAGS) -c prayer.c -o prayer.o

astro.o: astro.c astro.h prayer.h
	$(CC) $(CFLAGS) -c astro.c -o astro.o

libitl: all
	@echo "==> Create symbolic links for library generation..."
	test -d ../../build/itl/ \
	|| mkdir -p ../../build/itl/ \
	|| exit 1;
	ln -sf ../../prayertime/src/prayer.h ../../build/itl/prayer.h
	ln -sf ../prayertime/src/prayer.o ../../build/prayer.o
	ln -sf ../prayertime/src/astro.o ../../build/astro.o

libitl-libtool:
	libtool --mode=compile $(CC) -c prayer.c
	libtool --mode=compile $(CC) -c astro.c

clean:
	rm -f *.o *~ demo_prayer
	rm -f *.lo
	rm -rf .libs