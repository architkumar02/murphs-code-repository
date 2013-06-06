# Makefile for ROOT Analayis Macros
# Matthew J. Urffer (matthew.urffer@gmail.com)
#
#

# --------------------------------------------------------------
# GENERAL OPTIONS
# --------------------------------------------------------------
INCLUDEDIR = ./include
CC     =g++
CFLAGS =-g -Wall
INC    = -I $(INCLUDEDIR)
LIBS   =
LDFLAGS=
ifdef G4WORKDIR
	INSTALLDIR = $(G4WORKDIR)
else
	INSTALLDIR = ..
endif
# --------------------------------------------------------------
# ROOT OPTIONS
# --------------------------------------------------------------
ifdef ROOTSYS
	ROOTCFLAGS := $(shell root-config --cflags)
	ROOTLIBS     := $(shell root-config --libs)
	ROOTGLIBS    := $(shell root-config --glibs)
	ROOTLDFLAGS  := $(shell root-config --glibs)
	ROOTINCLUDE  := $(shell root-config --incdir)

	# Adding ROOT Options
	LIBS    += $(ROOTLIBS)
	CFLAGS  += $(ROOTCFLAGS)
	LDFLAGS += $(ROOTLDFLAGS)
	INC     += -I$(ROOTINCLUDE)
endif

# --------------------------------------------------------------
# TARGETS
# --------------------------------------------------------------
all: secondaryElectrons
	@echo "$@ done"

headers: $(wildcard $(INCLUDEDIR)/*.hh)

secondaryElectrons: SecondaryElectrons.C 
	@echo "Building $@"
	$(CC) $(CPPFLAGS) $(INC) -o secondaryElectrons SecondaryElectrons.C $(LDFLAGS) $(LIBS) 
	@echo "$@ done"


.PHONY: clean all install

install: all
	@echo "Copy files to install directory:" $(INSTALLDIR)
	@mv secondaryElectrons $(INSTALLDIR)
	@echo "$@ done"

clean:
	@echo "Cleaning up files and executables ..."
	rm -f *.d secondaryElectrons $(INSTALLDIR)/secondaryElectrons
	@echo "$@ done"
#
