# -------------------------------------------------------------------------
# Makefile for exprTree.c
#
# Matthew J. Urffer (matthew.urffer@gmail.com)
#  10/19/2012
# -------------------------------------------------------------------------


CC=gcc
CFLAGS+=-Wall -g
LDFLAGS+=-lm
SOURCES=exprTree.c
OJBECTS=$(SOURCES:.c=.o)
EXECUTABLE=exprTree

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *o $(EXECUTABLE)

# DO NOT DELETE
