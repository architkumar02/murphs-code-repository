# -------------------------------------------------------------------------
# Makefile for exprTree.c
#
# Matthew J. Urffer (matthew.urffer@gmail.com)
#  10/19/2012
# -------------------------------------------------------------------------


IDIR = ../include
CC = gcc
CFLAGS += -Wall -g -I$(IDIR)
LDFLAGS += -lm
SOURCES = exprTree.c
OJBECTS = $(patsubst %.c %.o,$(SOURCES))


all: exprTree

exprTree: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

%.o : %.c
	$(CC) $(CFLAGS) -c $<


clean:
	rm -rf *.o 

# DO NOT DELETE
