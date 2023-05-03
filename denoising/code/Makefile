#********************************************************
# MAKEFILE for BUILDING EXAMPLES FOR HIPO4 LIBRARY
# AUTHOR: GAVALIAN DATE: 10/24/2018
#********************************************************

ifndef HIPO_DIR
$(error HIPO_DIR is not set)
HIPO_DIR=../..
endif

HIPOCFLAGS  := -I$(HIPO_DIR)/hipo4 -I./submodules/install/include
HIPOLIBS    := -L$(HIPO_DIR)/lib -lhipo4

LZ4LIBS     := -L$(HIPO_DIR)/lz4/lib -llz4 -lpthread
LZ4INCLUDES := -I$(HIPO_DIR)/lz4/lib

CXX       := g++
CXXFLAGS  += -Wall -fPIC -std=c++14 -Wunknown-pragmas
LD        := g++
LDFLAGS   :=

OBJECTS  = denoise.o drift.o
OBJECTS2 = denoise2.o drift.o

all:  submodules denoise2

denoise: $(OBJECTS)
	$(CXX) -o denoise.exe $(OBJECTS)  $(HIPOLIBS) $(LZ4LIBS)

denoise2: $(OBJECTS2)
	$(CXX) -o denoise2.exe $(OBJECTS2)  $(HIPOLIBS) $(LZ4LIBS)

#analysis: analysis.o
#	$(CXX) -o analysis $<   $(HIPOLIBS) $(LZ4LIBS)

#clas12event_example: clas12event_example.o
#	$(CXX) -o clas12event_example $<   $(HIPOLIBS) $(LZ4LIBS)

clean:
	@echo 'Removing all build files'
	@rm -rf *.o *~ *.exe example*hipo 
	$(MAKE) -C submodules clean

.PHONY: submodules
submodules:
	$(MAKE) -C submodules all

%.o: %.cc
	$(CXX) -c $< -O3 $(CXXFLAGS) $(HIPOCFLAGS) $(LZ4INCLUDES)

