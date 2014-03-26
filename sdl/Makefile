# File:           Makefile
# Author:         Benjamin N. Summerton
# Description:    A Makefile for the SDL2 CHIP-8 Emualtor


CXX = g++
#CXXFLAGS = ggdb
CXXFLAGS = 
HEADERS = display.h sys_internals.h
SOURCES = main.cpp display.cpp sys_internals.cpp
LIBS = -lSDL2


c8_sdl: sys_internals.o display.o main.o
	$(CXX) -o c8_sdl main.o sys_internals.o display.o $(LIBS) $(CXXFLAGS)

sys_internals.o: sys_internals.h sys_internals.cpp
	$(CXX) -c sys_internals.cpp -o sys_internals.o $(CXXFLAGS)

display.o: display.h display.cpp
	$(CXX) -c display.cpp -o display.o $(CXXFLAGS)

main.o: $(HEADERS) main.cpp
	$(CXX) -c main.cpp -o main.o $(CXXFLAGS)

# Housekeeping targets
clean:
	rm -f *.o

realclean: clean
	rm -f c8_sdl
	rm -f a.out		# Cause by temp execs
