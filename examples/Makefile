CXX = g++
CXXFLAGS = -Wall -O3 -L../lib
INC = -I ../src/include
LIBS = -llorapi -lwiringPi
TARGETS = tx rx

all: $(TARGETS)

clean:
	rm -f $(TARGETS) *.o

tx: tx.cpp
	$(CXX) $(CXXFLAGS) $(INC) $< $(LIBS) -o $@

rx: rx.cpp
	$(CXX) $(CXXFLAGS) $(INC) $< $(LIBS) -o $@
