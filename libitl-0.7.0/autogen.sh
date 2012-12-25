#!/bin/sh
#--
# $Id: autogen.sh,v 1.4 2006/09/21 16:26:58 thamer Exp $
#--

EXIT=no

if test "$1" = "clean"; then
    echo "Removing auto-generated files..."
    rm -rf configure config.log config.status build/config.mk config.guess config.sub ltmain.sh libtool
    EXIT="yes"
fi

if test "$EXIT" = "yes"; then
    exit
fi

echo "Running autoconf..."
autoconf configure.in > configure && chmod +x configure
if test "$1" = "libitl-libtool"; then
    libtoolize
    autoconf configure.in.libtool > configure && chmod +x configure
fi
rm -rf autom4te.cache

echo "You can now run \"./configure\" and then \"make\"."
