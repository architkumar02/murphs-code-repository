# -------------------------------------------------------------------------
# Makefile for exprTree.c
#
# Matthew J. Urffer (matthew.urffer@gmail.com)
#  10/19/2012
# -------------------------------------------------------------------------

INCLUDE =include
CC = gcc
CFLAGS += -Wall -g -I $(INCLUDE)
LDFLAGS += -lm
EXEC = exprTree
SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)

# Main Target
$(EXEC): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXEC)

# Object Files
%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -rf $(OBJECTS) $(EXEC)

# DO NOT DELETE
