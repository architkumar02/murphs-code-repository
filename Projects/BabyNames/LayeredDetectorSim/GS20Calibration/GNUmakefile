# --------------------------------------------------------------
# GNUmakefile for polymerFim (layeredPolymerTracking)
#
# Matthew J. Urffer (matthew.urffer@gmail.com)
#  10/8/2012
# --------------------------------------------------------------
name := GS20LightYieldCal
G4TARGET := $(name)
G4EXLIB := true

CPPFLAGS += -g

ifndef G4INSTALL
  G4INSTALL = ../../..
endif

.PHONY: all clean
all: lib bin

install:
	@cp ${G4WORKDIR}/bin/${G4SYSTEM}/${G4TARGET} .

#### G4ANALYSIS_USE := true

include $(G4INSTALL)/config/binmake.gmk

ifdef ROOTSYS
  CPPFLAGS += -I$(shell $(ROOTSYS)/bin/root-config --incdir)
  CPPFLAGS += $(shell $(ROOTSYS)/bin/root-config --cflags)
  LDLIBS += $(shell $(ROOTSYS)/bin/root-config --libs)

   ifeq ($(G4SYSTEM),Linux-g++)
     LDLIBS += -Wl,-rpath,$(shell $(ROOTSYS)/bin/root-config --libdir)
   endif
endif
