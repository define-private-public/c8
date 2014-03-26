// File:         display.cpp
// Author:       Benjamin N. Summerton <bns4412@rit.edu>
// Description:  Implementation code for the Display module


#include "display.h"
#include <cstring>


// Constructor
Display::Display(int width, int height) :
	_width(width), _height(height)
{
	// Allocate the pixels and clear them out
	_pixels = new int[_width * _height];
}


// Copy Constructor
Display::Display(const Display &other) :
	_width(other._width), _height(other._height)
{
	// Allocate the pixels and copy them over
	_pixels = new int[_width * _height];
	memcpy(_pixels, other._pixels, _width * _height);
}


// Deconstructor
Display::~Display() {
	delete[] _pixels;
}


// The assignment operator
Display &Display::operator=(const Display &other) {
	// Make sure we don't duplicate
	if (this == &other)
		return *this;
	
	_width = other._width;
	_height = other._height;

	// Allocate the pixels and copy them over
	_pixels = new int[_width * _height];
	memcpy(_pixels, other._pixels, _width * _height);
	
	return *this;
}


// Get the display's width
int Display::getWidth() {
	return _width;
}


// Get the display's height
int Display::getHeight() {
	return _height;
}


// Clears the screen (i.e. set all pixels to 0)
void Display::clearDisplay() {
	memset(_pixels, 0, _width * _height);
}


// Get a single pixel's value
//
// Returns -1 if `x` or `y` are out of range, else 0 or 1 are returned
int Display::getPixel(int x, int y) {
	if ((x > _width) || (y > _height))
		return -1;
	
	return _pixels[(y * _width) + x];
}


// Sets a pixel to be drawn
//
// Returns -1 if `x` or `y` are out of range
// Returns -2 if `value is not a 1 or a 0
// Returns 1 if ther was a collision, else 0
int Display::setPixel(int x, int y, int value) {
	// Sanity checks
	if ((x > _width) || (y > _height))
		return -1;
	
	if ((value != 1) && (value != 0))
		return -2;

	// Set the pixel and return success
	_pixels[(y * _width) + x] = value;
	return 0;
}


// Sets a sprite to be drawn
// All sprites should be in the dimensions of ( 8 x `sHeight`), where the the individual bits in
// `sprite` are used as the pixels, ordered based upon "Most Significate Bit First"
// Modifys V[F] register on collision
//
// Returns -1 if `x` or `y` are out of range
// Returns -2 if `height` is not between 0 and 15
// Returns 1 if there was a collision, else 0
int Display::drawSprite(int x, int y, int sHeight, unsigned char pixels[]) {
	int collision = 0;

	if ((x > _width) || (y > _height))
		return -1;
	
	if ((sHeight < 0) || (sHeight > 15))
		return -2;

	// Draw the sprite
	// TODO: What to do in case of overflow?
	for (int i = 0; i < sHeight; i++) {
		for (int j = 0, mask = 0x80; j < 8; j++, mask >>= 1) {
			// Check for the collision
			if (_pixels[(_width * (y + i)) + (x + j)])
				collision = 1;

			_pixels[(_width * (y + i)) + (x + j)] ^= ((pixels[i] & mask) == mask);
		}
	}

	return collision; 	// Success!
}

