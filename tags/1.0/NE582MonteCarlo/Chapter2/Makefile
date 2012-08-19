# -----------------------------------------------------------
# Makefile for Chapter 1, Problem 2.  Matthew J. Urffer
# -----------------------------------------------------------

# Include General ROOT MAKE
include Makefile.arch
-include ../MyConfig.mk

# Object and source files

# Problem 2-1
PROB21_O       = Problem2-1.$(ObjSuf)
PROB21_S       = Problem2-1.$(SrcSuf)
PROB21        = Problem2-1$(ExeSuf)

# Problem 2-2
PROB22_O       = Problem2-2.$(ObjSuf)
PROB22_S       = Problem2-2.$(SrcSuf)
PROB22        = Problem2-2$(ExeSuf)

# Problem 2-3
PROB23_O       = Problem2-3.$(ObjSuf)
PROB23_S       = Problem2-3.$(SrcSuf)
PROB23        = Problem2-3$(ExeSuf)

# Problem 2-4
PROB24_O       = Problem2-4.$(ObjSuf)
PROB24_S       = Problem2-4.$(SrcSuf)
PROB24        = Problem2-4$(ExeSuf)

# Problem 2-5
PROB25_O       = Problem2-5.$(ObjSuf)
PROB25_S       = Problem2-5.$(SrcSuf)
PROB25        = Problem2-5$(ExeSuf)

# Problem 2-6
PROB26_O       = Problem2-6.$(ObjSuf)
PROB26_S       = Problem2-6.$(SrcSuf)
PROB26        = Problem2-6$(ExeSuf)

# Problem 2-7
PROB27_O       = Problem2-7.$(ObjSuf)  HaltonSequence.$(ObjSuf)
PROB27_S       = Problem2-7.$(SrcSuf)  HaltonSequence.$(SrcSuf)
PROB27        = Problem2-7$(ExeSuf)

# Problem 2-8
PROB28_O       = Problem2-8.$(ObjSuf)
PROB28_S       = Problem2-8.$(SrcSuf)
PROB28        = Problem2-8$(ExeSuf)

# All Object and source Files
OBJS          = $(PROB21_O) $(PROB22_O) $(PROB23_O) $(PROB24_O) \
					 $(PROB25_O) $(PROB26_O) $(PROB27_O) $(PROB28_O)

PROGRAMS      =$(PROB21) $(PROB22) $(PROB23) $(PROB24) \
					 $(PROB25) $(PROB26) $(PROB27) $(PROB28)

# ----------- Acio Compile! ---------------------------------

.SUFFIXES: .$(SrcSuf) .$(ObjSuf) .$(DllSuf)
.PHONY:    

all:            $(PROGRAMS)

$(EVENTSO):     $(EVENTO)
ifeq ($(PLATFORM),macosx)
# We need to make both the .dylib and the .so
		$(LD) $(SOFLAGS)$@ $(LDFLAGS) $^ $(OutPutOpt) $@
ifneq ($(subst $(MACOSX_MINOR),,1234),1234)
ifeq ($(MACOSX_MINOR),4)
		ln -sf $@ $(subst .$(DllSuf),.so,$@)
else
		$(LD) -bundle -undefined $(UNDEFOPT) $(LDFLAGS) $^ \
		   $(OutPutOpt) $(subst .$(DllSuf),.so,$@)
endif
else
ifeq ($(PLATFORM),win32)
		bindexplib $* $^ > $*.def
		lib -nologo -MACHINE:IX86 $^ -def:$*.def \
		   $(OutPutOpt)$(EVENTLIB)
		$(LD) $(SOFLAGS) $(LDFLAGS) $^ $*.exp $(LIBS) \
		   $(OutPutOpt)$@
		$(MT_DLL)
else
		$(LD) $(SOFLAGS) $(LDFLAGS) $^ $(OutPutOpt) $@ $(EXPLLINKLIBS)
endif
		@echo "$@ done"

$(EVENTMTSO):     $(EVENTMTO)
ifeq ($(PLATFORM),macosx)
# We need to make both the .dylib and the .so
		$(LD) $(SOFLAGS)$@ $(LDFLAGS) $^ $(OutPutOpt) $@
ifneq ($(subst $(MACOSX_MINOR),,1234),1234)
ifeq ($(MACOSX_MINOR),4)
		ln -sf $@ $(subst .$(DllSuf),.so,$@)
else
		$(LD) -bundle -undefined $(UNDEFOPT) $(LDFLAGS) $^ \
		   $(OutPutOpt) $(subst .$(DllSuf),.so,$@)
endif
endif
else
ifeq ($(PLATFORM),win32)
		bindexplib $* $^ > $*.def
		lib -nologo -MACHINE:IX86 $^ -def:$*.def \
		   $(OutPutOpt)$(EVENTMTLIB)
		$(LD) $(SOFLAGS) $(LDFLAGS) $^ $*.exp $(LIBS) \
		   $(OutPutOpt)$@
		$(MT_DLL)
else
		$(LD) $(SOFLAGS) $(LDFLAGS) $^ $(OutPutOpt) $@ $(EXPLLINKLIBS)
endif
endif
endif
endif
		@echo "$@ done"

# Some More Magic
$(PROB21):       $(PROB21_O)
		$(LD) $(LDFLAGS) $^ $(LIBS) $(OutPutOpt)$@
		$(MT_EXE)
		@echo "$@ done"
	

$(PROB22):       $(PROB22_O)
		$(LD) $(LDFLAGS) $^ $(LIBS) $(OutPutOpt)$@
		$(MT_EXE)
		@echo "$@ done"

$(PROB23):       $(PROB23_O)
		$(LD) $(LDFLAGS) $^ $(LIBS) $(OutPutOpt)$@
		$(MT_EXE)
		@echo "$@ done"

$(PROB24):       $(PROB24_O)
		$(LD) $(LDFLAGS) $^ $(LIBS) $(OutPutOpt)$@
		$(MT_EXE)
		@echo "$@ done"

$(PROB25):       $(PROB25_O)
		$(LD) $(LDFLAGS) $^ $(LIBS) $(OutPutOpt)$@
		$(MT_EXE)
		@echo "$@ done"

$(PROB26):       $(PROB26_O)
		$(LD) $(LDFLAGS) $^ $(LIBS) $(OutPutOpt)$@
		$(MT_EXE)
		@echo "$@ done"

$(PROB27):       $(PROB27_O)
		$(LD) $(LDFLAGS) $^ $(LIBS) $(OutPutOpt)$@
		$(MT_EXE)
		@echo "$@ done"

$(PROB28):       $(PROB28_O)
		$(LD) $(LDFLAGS) $^ $(LIBS) $(OutPutOpt)$@
		$(MT_EXE)
		@echo "$@ done"

clean:
		@rm -f $(PROGRAMS) $(OBJS) core *Dict.*

distclean:      clean
		@rm -f $(EVENTSO) $(EVENTLIB) *Dict.* *.def *.exp \
		   *.root *.ps *.gif *.so *.lib *.dll *.d *.log .def so_locations \
		   files/*

.SUFFIXES: .$(SrcSuf)

