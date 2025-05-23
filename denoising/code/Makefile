#********************************************************
# MAKEFILE for BUILDING EXAMPLES FOR HIPO4 LIBRARY
# AUTHOR: GAVALIAN DATE: 10/24/2018
#********************************************************

# check for $HIPO environment variable
ifndef HIPO
$(error ERROR: HIPO environment variable is not set; it should point to the HIPO installation prefix)
endif

# check if pkg-config can find HIPO
HIPO_PKGCONFIG := pkg-config --with-path $(HIPO)/lib/pkgconfig hipo4
HIPO_FOUND := $(shell $(HIPO_PKGCONFIG); echo $$?)
ifneq ($(HIPO_FOUND),0)
$(error "pkg-config cannot find HIPO, or you do not have pkg-config; tried looking in $$HIPO = $(HIPO)")
endif

INSTALL_PREFIX = $(shell cd $(CURDIR)/../../install && pwd -P)

HIPOCFLAGS := $(shell $(HIPO_PKGCONFIG) --cflags) -I$(INSTALL_PREFIX)/include
HIPOLIBS := -L$(shell $(HIPO_PKGCONFIG) --variable libdir) -l:libhipo4.a

LZ4LIBS     := -llz4 -lpthread

CXX       := g++
CXXFLAGS  += -Wall -fPIC -std=c++17 -Wunknown-pragmas
LD        := g++
LDFLAGS   :=

OBJECTS  = denoise.o drift.o
OBJECTS2 = denoise2.o drift.o

all:  denoise2 install

install:
	mkdir -p $(INSTALL_PREFIX)/bin
	install denoise2.exe $(INSTALL_PREFIX)/bin
	mkdir -p $(INSTALL_PREFIX)/share/denoising
	cp -v network/*.json $(INSTALL_PREFIX)/share/denoising
	@echo "================================="
	@echo "done installation; you may want to set the environment variable 'DENOISING_NETWORKS' to:"
	@echo "  $(INSTALL_PREFIX)/share/denoising"
	@echo "================================="

denoise: $(OBJECTS)
	$(CXX) -o denoise.exe $(OBJECTS)  $(HIPOLIBS) $(LZ4LIBS)

denoise2: $(OBJECTS2)
	$(CXX) -o denoise2.exe $(OBJECTS2)  $(HIPOLIBS) $(LZ4LIBS)

clean:
	@echo 'Removing all build files'
	@rm -rf *.o *~ *.exe example*hipo

%.o: %.cc
	$(CXX) -c $< -O3 $(CXXFLAGS) $(HIPOCFLAGS)
