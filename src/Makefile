CXX = g++
CXXFLAGS = -Wall -O3
LIBS = -lwiringPi
TARGET = liblorapi.a
OBJS = lora.o packet.o
INC = -I include
LIB = ../lib

all: $(TARGET)

clean:
	rm -f $(TARGET) *.o

$(TARGET): $(OBJS)
	mkdir -p $(LIB)
	ar rcs $(LIB)/$@ $^

lora.o: lora.cpp include/lora.h
	g++ -c $(CXXFLAGS) $(INC) $<

packet.o: packet.cpp include/packet.h
	g++ -c $(CXXFLAGS) $(INC) $<
