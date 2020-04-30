# $Id: GNUmakefile 68058 2013-03-13 14:47:43Z gcosmo $
# --------------------------------------------------------------
# GNUmakefile for examples module.  Gabriele Cosmo, 06/04/98.
# --------------------------------------------------------------

name := mmtMain
G4TARGET := $(name)
G4EXLIB := true

ifndef G4INSTALL
  G4INSTALL = ../../..
endif

LDFLAGS      := $(shell `root-config --libs` `python2.7-config --ldflags`)
ROOTCFLAGS   := $(shell `root-config --cflags`)
ROOTLIBS     := $(shell `root-config --libs`)
ROOTGLIBS    := $(shell `root-config --glibs`)
#ROOTSYS      := $(shell echo $(ROOTSYS))

#CPPFLAGS  := -I/share/apps/python/2.7.3/include/python2.7 -I$(CRYHOME)/src `root-config --cflags`
#EXTRALIBS := -L/share/apps/python/2.7.3/lib/python2.7 -lpthread -ldl -lutil -lm  -lpython2.7 -Xlinker -export-dynamic -L$(CRYHOME)/lib -lCRY $(ROOTLIBS)

CPPFLAGS  := -I/share/apps/python/3.7.2/include/python3.7m -I$(CRYHOME)/src `root-config --cflags`
EXTRALIBS := -L/share/apps/python/3.7.2/lib/python3.7 -lpthread -ldl -lutil -lm  -lpython3.7 -Xlinker -export-dynamic -L$(CRYHOME)/lib -lCRY $(ROOTLIBS)

#INCLUDES = -I/share/apps/python/2.7.3/include
#CFLAGS   = -c $(INCLUDES) -O3
#LDFLAGS  = `root-config --libs` -lXmu #-L/share/apps/python/2.7.3/lib -lpython2.7

.PHONY: all
all: lib bin

include $(G4INSTALL)/config/binmake.gmk

visclean:
	rm -f g4*.prim g4*.eps g4*.wrl
	rm -f .DAWN_*
