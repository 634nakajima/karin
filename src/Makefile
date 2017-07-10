CFLAGS=-Wall -O3 -g -std=c++11
CXXFLAGS=$(CFLAGS)
OBJECTS=karin.o Ripple.o
BINARIES=karin

# Where our library resides. You mostly only need to change the
# RGB_LIB_DISTRIBUTION, this is where the library is checked out.
RGB_LIB_DISTRIBUTION=..
RGB_INCDIR=$(RGB_LIB_DISTRIBUTION)/include/led-matrix
RGB_LIBDIR=$(RGB_LIB_DISTRIBUTION)/lib
SRC_DIR=$(RGB_LIB_DISTRIBUTION)/src
RGB_LIBRARY_NAME=rgbmatrix
RGB_LIBRARY=$(RGB_LIBDIR)/lib$(RGB_LIBRARY_NAME).a
LDFLAGS+=-L$(RGB_LIBDIR) -l$(RGB_LIBRARY_NAME) -lrt -lm -lpthread

all : $(BINARIES)

%.o : %.cpp
	$(CXX) -I$(RGB_INCDIR) -I$(SRC_DIR) $(CXXFLAGS) -c -o $@ $<

karin : $(OBJECTS) $(RGB_LIBRARY)
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)

clean:
	rm -f $(OBJECTS) $(BINARIES)

FORCE:
.PHONY: FORCE
