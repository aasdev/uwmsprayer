SHELL = /bin/sh
sys = $(shell uname -s)
CC = gcc
CXX = g++
MAKE = make


TARGET = uwmsprayer
STATICTARGET = $(TARGET)-static
MAJOR = 1
MINOR = 0


ITLLIBDIR = libitl-0.7.0
LIBS = ./libitl/build/libitl.so
LIBDIRS = $(ITLLIBDIR)

SRC = uwmsprayer.c
INCS = uwmsprayer.h $(LIBS)/build/hijri.h $(LIBS)/build/prayer.h

CFLAGS = -I./$(ITLLIBDIR)/build -Wall
OBJS = ${SRC:%.c=%.o}
LDFLAGS = -L./$(ITLLIBDIR)/build -litl

OTHERFLAGS =

all: $(LIBS) $(TARGET)


$(TARGET): $(OBJS) $(INCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

$(OBJS): $(SRC)
	$(CC) $(CFLAGS) -c $<

$(LIBS) $(INCS):
	@for dir in $(LIBDIRS); do \
	    echo Making $$dir; \
	    cd $$dir; ./configure; $(MAKE); cd ..; \
	done

static: $(OBJS) $(INCS)
	$(CC) -static $(CFLAGS) -o $(STATICTARGET) $(OBJS) $(LDFLAGS) -lm

clean:
	rm -f $(TARGET) $(STATICTARGET) $(OBJS) *~
	@for dir in $(LIBDIRS); do \
	    echo Cleaning $$dir; \
	    cd $$dir; $(MAKE) clean; cd ..; \
	done

