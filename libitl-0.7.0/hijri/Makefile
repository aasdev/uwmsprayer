# $Id: Makefile,v 1.8 2006/09/21 16:34:31 thamer Exp $

include ../build/config.mk

all \
libitl \
libitl-libtool \
demo_hijri:
	@cd src && $(MAKE) $@

install: all
	@echo "==> Installing hijri component..."
	test -d $(PREFIX)/include/itl \
	|| install -d $(PREFIX)/include/itl \
	|| exit 1;
	install src/hijri.h $(PREFIX)/include/itl

uninstall:
	@echo "==> Uninstalling hijri component..."
	rm -f $(PREFIX)/include/itl/hijri.h

clean:
	@echo "==> Cleaning hijri component files..."
	@cd src && $(MAKE) $@
