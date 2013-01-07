# Makefile for ROOT Analayis Macros
# Matthew J. Urffer (matthew.urffer@gmail.com)
#
# Jan. 7,  2013 - Finally got it to compile.
# Dec. 26, 2012
# Dec. 30, 2012
#

# --------------------------------------------------------------
# GENERAL OPTIONS
# --------------------------------------------------------------
CC     =g++
CFLAGS =-g -Wall
INC    =
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
	INC     += $(ROOTINCLUDE)
endif

# --------------------------------------------------------------
# TARGETS
# --------------------------------------------------------------
all: energyDep
	@echo "$@ done"

energyDep: DetectorEDep.C 
	@echo "Building $@"
	$(CC) $(CPPFLAGS) -I $(INC) -o energyDep DetectorEDep.C $(LDFLAGS) $(LIBS) 
	@echo "$@ done"
	@echo

.PHONY: clean all install

install: all
	@echo "Copy files to install directory:" $(INSTALLDIR)
	@cp energyDep $(INSTALLDIR)
	@echo "$@ done"
	@echo

clean:
	@echo "Cleaning up files and executables ..."
	rm -f *.d energyDep $(INSTALLDIR)/energyDep
	@echo "$@ done"
#
