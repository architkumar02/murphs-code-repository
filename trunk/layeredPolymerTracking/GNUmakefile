# --------------------------------------------------------------
# GNUmakefile for polymerFim (layeredPolymerTracking)
#
# Matthew J. Urffer (matthew.urffer@gmail.com)
#  10/8/2012
# Revision Log:
# 2012 - 12 - 26
# 	Added on the ROOT Macro option
# --------------------------------------------------------------

CPPFLAGS+=-g
# --------------------------------------------------------------
# MPI USAGE
# --------------------------------------------------------------
ifdef G4MIUSE
	name := mpiPolymerFilm
	G4TARGET := $(name)
	G4EXLIB := true
	CPPFLAGS += -I$(MPI_INCLUDE)  -DG4MPIUSE=1
else
	name := polymerFilm
	G4TARGET := $(name)
	G4EXLIB := true
endif

# --------------------------------------------------------------
# GEANT4 Linking
# --------------------------------------------------------------
## G4ANALYSIS_USE := true
ifndef G4INSTALL
	G4INSTALL = ../../..
endif
include $(G4INSTALL)/config/binmake.gmk
ifdef G4MPIUSE
	include /home/murffer/MPI/mpi_interface/G4MPI.gmk
	LDLIBS1 += -lG4UImpi
endif

# --------------------------------------------------------------
# ROOT Linking
# --------------------------------------------------------------
ifdef ROOTSYS
	CPPFLAGS += -I$(shell $(ROOTSYS)/bin/root-config --incdir)
	CPPFLAGS += $(shell $(ROOTSYS)/bin/root-config --cflags)
	LDFTAGS += $(shell $(ROOTSYS)/bin/root-config --glibs)
	LDLIBS += $(shell $(ROOTSYS)/bin/root-config --libs)
	ifeq ($(G4SYSTEM),Linux-g++)
		LDLIBS += -Wl,-rpath,$(shell $(ROOTSYS)/bin/root-config --libdir)
	endif

endif

# --------------------------------------------------------------
# TARGETS
# --------------------------------------------------------------
.PHONY: all
all: lib bin

macros:
	$(MAKE) -C ./ROOTMacros/ install

histclean:
	rm ${G4WORKDIR}/tmp/${G4SYSTEM}/${G4TARGET}/HistoManager.o

