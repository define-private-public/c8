// File:         display.h 
// Author:       Benjamin N. Summerton 
// Description:  Used to manage a "virtual display," for the CHIP-8 emualtor.  Should not have any
//               SDL code in it, it's just an abstract view where someone can put/get pixel info.

#ifndef C8_DISPLAY_H
#define C8_DISPLAY_H

/*==
* Default CHIP-8 supports a 64 by 32 pixel display (monochrome).  Of course, there are alternatives
* but it's best juse to use that one.
*
* A pixel set to `0` is off
* A pixel set to `1` is on,  everything else is undefined
*/


// Holds pixel information
class Display {
public:
	// Consrtuctors
	Display(int width, int height);
	Display(const Display &other);
	~Display();

	// Copy assingment
	Display &operator=(const Display &other);

	
	// Accessors
	int getWidth();
	int getHeight();
	int getPixel(int x, int y);

	// Mutators	
	void clearDisplay();
	int setPixel(int x, int y, int value);		// Bad convienece method
	int drawSprite(int x, int y, int sHeight, unsigned char sprite[]);

	// For save states
	void load(int width, int height, unsigned char *pixels);
	unsigned char *dump();		// Dumps the pixel data



private:
	int _width;
	int _height;
	int *_pixels;
};


#endif	// C8_DISPLAY_H

