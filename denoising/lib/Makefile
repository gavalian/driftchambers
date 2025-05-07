#********************************************************
# MAKEFILE for BUILDING EXAMPLES FOR HIPO4 LIBRARY
# AUTHOR: GAVALIAN DATE: 10/24/2018
#********************************************************

#ifndef HIPO_DIR
#$(error HIPO_DIR is not set)
#HIPO_DIR=../..
#endif

FRUGALLY  := -I$(HIPO_DIR)/hipo4 -I../../install/include -I$(HIPO_DIR)/include

CXX       := g++
CXXFLAGS  += -Wall -fPIC -std=c++17 -Wunknown-pragmas
LD        := g++
LDFLAGS   :=

OBJECTS  = denoise3.o drift.o
OBJECTS2 = denoise2.o drift.o
OBJECTS_LIB = drift3.o
OBJECTS_BEN = drift3.o benchmark.o

all:  slib

lib: $(OBJECTS_LIB)
	ar cr libdenoiser.a $(OBJECTS_LIB)

bench: $(OBJECTS_BEN)
	$(CXX) -o benchmark.exe $(OBJECTS_BEN) -I../../install/include

slib: $(OBJECTS_LIB)
	$(CXX) -shared $(OBJECTS_LIB) -o libdenoiser.so

install:
	mkdir -p ../../install/bin
	install denoise2.exe ../../install/bin

denoise: $(OBJECTS)
	$(CXX) -o denoise3.exe $(OBJECTS)  $(HIPOLIBS) $(LZ4LIBS)

denoise2: $(OBJECTS2)
	$(CXX) -o denoise2.exe $(OBJECTS2)  $(HIPOLIBS) $(LZ4LIBS)

clean:
	@echo 'Removing all build files'
	@rm -rf *.o *~ *.exe example*hipo 

%.o: %.cc
	$(CXX) -c $< -O3 $(CXXFLAGS) 
