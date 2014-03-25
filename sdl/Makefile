# File:           Makefile
# Author:         Benjamin N. Summerton
# Description:    A Makefile for the SDL2 CHIP-8 Emualtor


CXX = g++
#CXXFLAGS = ggdb
CXXFLAGS = 
SOURCES = main.cpp
LIBS = -lSDL2


c8_sdl: main.o
	$(CXX) -o c8_sdl main.o $(LIBS) $(CXXFLAGS)


main.o: main.cpp
	$(CXX) -c main.cpp -o main.o $(CXXFLAGS)


# Housekeeping targets
clean:
	rm -f *.o

realclean: clean
	rm -f c8_sdl
	rm -f a.out		# Cause by temp execs
