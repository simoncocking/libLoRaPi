CXX = g++
CXXFLAGS = -Wall -O3
LIBS = -lwiringPi
EXE = lora
OBJS = lora.o lorapacket.o
INC = -I ../../../libraries/wiringPi/wiringPi

all: $(EXE)

clean:
	rm -f $(EXE) *.o

$(EXE): $(OBJS)
	g++ $(OBJS) $(LIBS) -o $@

lora.o: lora.cpp lora.h
	g++ -c $(CXXFLAGS) $(INC) lora.cpp

lorapacket.o: lorapacket.cpp lorapacket.h
	g++ -c $(CXXFLAGS) $(INC) lorapacket.cpp

