# $Id: Makefile,v 1.10 2006/09/21 16:37:51 thamer Exp $

include ../build/config.mk

all \
libitl \
libitl-libtool \
demo_prayer:
	@cd src && $(MAKE) $@

install: all
	@echo "==> Installing prayertime component..."
	test -d $(PREFIX)/include/itl \
	|| install $(PREFIX)/include/itl \
	|| exit 1;
	install src/prayer.h $(PREFIX)/include/itl

uninstall:
	@echo "==> Uninstalling prayertime component..."
	rm -f $(PREFIX)/include/itl/prayer.h

clean:
	@echo "==> Cleaning prayertime component files..."
	@cd src && $(MAKE) $@
