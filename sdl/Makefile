# File:           Makefile
# Author:         Benjamin N. Summerton
# Description:    A Makefile for the SDL2 CHIP-8 Emualtor


CXX = g++
#CXXFLAGS = -ggdb
CXXFLAGS = 
HEADERS = display.h sys_internals.h buzzer.h perform_operation.h
SOURCES = main.cpp display.cpp sys_internals.cpp buzzer.cpp perform_operation.cpp
LIBS = -lSDL2 -lSDL2_mixer


c8_sdl: sys_internals.o display.o buzzer.o main.o perform_operation.o
	$(CXX) -o c8_sdl main.o sys_internals.o buzzer.o display.o perform_operation.o $(LIBS) $(CXXFLAGS)

sys_internals.o: sys_internals.h sys_internals.cpp
	$(CXX) -c sys_internals.cpp -o sys_internals.o $(CXXFLAGS)

display.o: display.h display.cpp
	$(CXX) -c display.cpp -o display.o $(CXXFLAGS)

perform_operation.o: sys_internals.o display.o perform_operation.h perform_operation.cpp
	$(CXX) -c perform_operation.cpp -o perform_operation.o $(CXXFLAGS)

buzzer.o: buzzer.h buzzer.cpp
	$(CXX) -c buzzer.cpp -o buzzer.o $(CXXFLAGS)

main.o: $(HEADERS) main.cpp
	$(CXX) -c main.cpp -o main.o $(CXXFLAGS)

# Housekeeping targets
clean:
	rm -f *.o

realclean: clean
	rm -f c8_sdl
	rm -f a.out		# Caused by temp execs


#all: $(HEADERS) $(SOURCES)
#	$(CXX) -o a.out $(SOURCES) $(CXXFLAGS) $(LIBS)

