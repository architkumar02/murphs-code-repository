# -----------------------------------------------------------
# Makefile for Chapter 1, Problem 2.  Matthew J. Urffer
# -----------------------------------------------------------

# Include General ROOT MAKE
include Makefile.arch
-include ../MyConfig.mk

# Object and source files

# Problem 1-6
PROB16_O       = Problem1-6.$(ObjSuf) LCG.$(ObjSuf)
PROB16_S       = Problem1-6.$(SrcSuf) LCG.$(SrcSuf)
PROB16        = Problem1-6$(ExeSuf)

# Problem 1-8
PROB18_O       = Problem1-8.$(ObjSuf) Discrepancy.$(ObjSuf)
PROB18_S       = Problem1-8.$(SrcSuf) Discrepancy.$(SrcSuf)
PROB18        = Problem1-8$(ExeSuf)

# Problem 1-9
PROB19_O       = Problem1-9.$(ObjSuf) LCG.$(ObjSuf) Discrepancy.$(ObjSuf)
PROB19_S       = Problem1-9.$(SrcSuf) LCG.$(SrcSuf) Discrepancy.$(SrcSuf)
PROB19        = Problem1-9$(ExeSuf)


# All Object and source Files
OBJS          = $(PROB16_O) $(PROB18_O) $(PROB19_O)

PROGRAMS      = $(PROB17) $(PROB18) $(PROB19) 

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
$(PROB16):       $(PROB16_O)
		$(LD) $(LDFLAGS) $^ $(LIBS) $(OutPutOpt)$@
		$(MT_EXE)
		@echo "$@ done"

$(PROB18):       $(PROB18_O)
		$(LD) $(LDFLAGS) $^ $(LIBS) $(OutPutOpt)$@
		$(MT_EXE)
		@echo "$@ done"

$(PROB19):       $(PROB19_O)
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

