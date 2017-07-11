CFLAGS=-Wall -O3 -g -std=c++11
CXXFLAGS=$(CFLAGS)
OBJECTS=karin.o Ripple.o ADDA.o FFTFilter.o DB.o
BINARIES=karin

# Where our library resides. You mostly only need to change the
# RGB_LIB_DISTRIBUTION, this is where the library is checked out.
RGB_LIB_DISTRIBUTION=..
RGB_INCDIR=$(RGB_LIB_DISTRIBUTION)/include
LIBDIR=$(RGB_LIB_DISTRIBUTION)/lib
SRC_DIR=$(RGB_LIB_DISTRIBUTION)/src
LIBRARYS=$(LIBDIR)/librgbmatrix.a /home/pi/aquila-build/lib/libOoura_fft.a
LDFLAGS+=-L$(LIBDIR) -L/home/pi/aquila-build/lib -lrgbmatrix -lportaudio -lAquila -lOoura_fft -lrt -lm -lpthread

all : $(BINARIES)

%.o : %.cpp
	$(CXX) -I$(RGB_INCDIR) -I$(SRC_DIR) $(CXXFLAGS) -c -o $@ $<

karin : $(OBJECTS) $(LIBRARYS)
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)

clean:
	rm -f $(OBJECTS) $(BINARIES)

FORCE:
.PHONY: FORCE
