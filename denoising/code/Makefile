#********************************************************
# MAKEFILE for BUILDING EXAMPLES FOR HIPO4 LIBRARY
# AUTHOR: GAVALIAN DATE: 10/24/2018
#********************************************************

ifndef HIPO_DIR
$(error HIPO_DIR is not set)
HIPO_DIR=../..
endif

HIPOCFLAGS  := -I$(HIPO_DIR)/hipo4 -I../../install/include
#HIPOLIBS    := -L$(HIPO_DIR)/lib -lhipo4
HIPOLIBS    := $(HIPO_DIR)/lib/libhipo4.a

LZ4LIBS     := -L$(HIPO_DIR)/lz4/lib -llz4 -lpthread
LZ4INCLUDES := -I$(HIPO_DIR)/lz4/lib

CXX       := g++
CXXFLAGS  += -Wall -fPIC -std=c++17 -Wunknown-pragmas
LD        := g++
LDFLAGS   :=

OBJECTS  = denoise.o drift.o
OBJECTS2 = denoise2.o drift.o

all:  denoise2 install

install:
	mkdir -p ../../install/bin
	install denoise2.exe ../../install/bin

denoise: $(OBJECTS)
	$(CXX) -o denoise.exe $(OBJECTS)  $(HIPOLIBS) $(LZ4LIBS)

denoise2: $(OBJECTS2)
	$(CXX) -o denoise2.exe $(OBJECTS2)  $(HIPOLIBS) $(LZ4LIBS)

clean:
	@echo 'Removing all build files'
	@rm -rf *.o *~ *.exe example*hipo 

%.o: %.cc
	$(CXX) -c $< -O3 $(CXXFLAGS) $(HIPOCFLAGS) $(LZ4INCLUDES)
