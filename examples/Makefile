# Makefile example

CFLAGS=`pkg-config groomrl --cflags`
LIBS=`pkg-config groomrl --libs`

all: wrapper_test example

%: %.cc
	$(CXX) $(CFLAGS) -o $@ $< $(LIBS)

clean:
	rm -rf wrapper_test example
