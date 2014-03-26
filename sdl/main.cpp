// File:         main.cpp
// Author:       Benjamin N. Summerton 
// Description:  Main source file for c8's SDL2 emulator


// C++ includes
#include <iostream>
using namespace std;

// SDL2 includes
#include <SDL2/SDL.h>

// d8 includes
#include "sys_internals.h"
#include "display.h"


// Defitions
#define BEEP_PATH "../beep.wav"


// System internals
Stack stack(STACK_SIZE);
Memory mem(MEM_4KB);

// for the screen
Display disp(64, 32);
const int scale = 4;
const int screenWidth = disp.getWidth() * scale;
const int screenHeight = disp.getHeight() * scale;


// Main program function
int main(int argc, char *argv[]) {
	// SDL Variables
	bool quit = false;
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Event event;
	SDL_Color onClr = {0x00, 0xD0, 0x00}, offClr = {0x00, 0x20, 0x00};
	SDL_Rect pixelRect;
	SDL_AudioSpec beepSpec;

	// Init all of the sub systems
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
		cerr << "Could not initialize SDL2." << endl;
		exit(1);
	}

	// Create the window
	window = SDL_CreateWindow("CHIP-8 Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
							  screenWidth, screenHeight, 0);
	if (!window) {
		cerr << "Could not create the window." << endl;
		exit(1);
	}

	// Create the renderer
	renderer = SDL_CreateRenderer(window, -1, 0);
	if (!renderer) {
		cerr << "Could not create the renderer." << endl;
		exit(1);
	}

	// Memory file test
	cout << "Mem size: " << mem.getMemorySize() << endl;
	cout << "Prog size: " << mem.getProgramSize() << endl;
	cout << "FB: " << (int)mem.readByte(MEM_PROG_START) << endl;
	cout << "Load File: " << mem.loadCHIP8Program(argv[1]) << endl;
	cout << "Prog size: " << mem.getProgramSize() << endl;
	cout << "FB: " << (int)mem.readByte(MEM_PROG_START) << endl;
	cout << "WB: " << mem.writeByte(MEM_PROG_START, 0xFF) << endl;
	cout << "FB: " << (int)mem.readByte(MEM_PROG_START) << endl;

	// Alter one byte and see what happens
	unsigned char dest[MEM_4KB];
	cout << "Dump: " << mem.dump(dest) << endl;
	dest[MEM_PROG_START] = 0xFE;
	cout << "Load: " << mem.load(dest, MEM_4KB) << endl;
	cout << "FB: " << (int)mem.readByte(MEM_PROG_START) << endl;




	
	/*== Main program loop ==*/
	SDL_ShowCursor(0);
	while (!quit) {
		while (SDL_PollEvent(&event)) {
			// Window exit
			if (event.type == SDL_QUIT)
				quit = true;

			// ESC key press
			if (event.type == SDL_KEYUP) {
				if (event.key.keysym.sym = SDLK_ESCAPE)
					quit = true;
			}
		}


		/*== Drawing ==*/
		// Clear the screen
		SDL_SetRenderDrawColor(renderer, offClr.r, offClr.g, offClr.b, 0xFF);
		SDL_RenderClear(renderer);

		// Flip the screen and hold
		SDL_RenderPresent(renderer);
		SDL_Delay(1000 / cpuSpeed);		// In milliseconds
	}

	// Cleanup and exit
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}

