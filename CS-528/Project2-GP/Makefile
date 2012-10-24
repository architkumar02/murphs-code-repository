# -------------------------------------------------------------------------
# Makefile for exprTree.c
#
# Matthew J. Urffer (matthew.urffer@gmail.com)
#  10/19/2012
# -------------------------------------------------------------------------
APP 	= murffer-Project2

OBJDIR 	= obj

SRCS 	:= $(shell find . -name '*.c')
SRCDIRS	:= $(shell find . -name '*.c' -exec dirname {} \; | uniq)
HEADERS := $(shell find . -name '*.h')
OBJS	:= $(patsubst %.c,$(OBJDIR)/%.o,$(SRCS))
DEPS	 =
ALLFILES:= $(SRCS) $(HEADERS) 

CC = gcc
DEBUG	 = -g
INCLUDES = -I ./include
CFLAGS	 = $(DEBUG) -Wall -pedantic $(INCLUDES) -c
LDFLAGS  = -lm
LIBS	 =

#DEPENDS  = -MT $@ -MD -MP -MF $(subst .o,.d,$@)
DEPENDS  = 

.PHONY: all clean distclean dist

all: buildrepo $(APP)

$(APP): $(OBJS)
	$(CC) $(LDFLAGS) $^ $(LIBS) -o $@

$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) $(DEPENDS) $< -o $@

clean:
	rm -r $(OBJDIR)

distclean: clean
	rm $(APP)

buildrepo:
	@$(call make-repo)

dist:
	@tar czf murffer_project2.tgz $(ALLFILES) Makefile README proj2-data.dat bestTree.postfix

define make-repo
	for dir in $(SRCDIRS); \
	do \
		mkdir -p $(OBJDIR)/$$dir;\
	done
endef

ifneq "$(MAKECMDGOALS)" "distclean"
ifneq "$(MAKECMDGOALS)" "clean"
-include $(DEPS)
endif
endif

# DO NOT DELETE
