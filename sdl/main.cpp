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
		
		unsigned char spr[5];
		for (int i = 8; i < 16; i++) {
			spr[0] = mem.readByte(MEM_FONT_START + (i * 5) + 0);
			spr[1] = mem.readByte(MEM_FONT_START + (i * 5) + 1);
			spr[2] = mem.readByte(MEM_FONT_START + (i * 5) + 2);
			spr[3] = mem.readByte(MEM_FONT_START + (i * 5) + 3);
			spr[4] = mem.readByte(MEM_FONT_START + (i * 5) + 4);
			
			disp.drawSprite((i - 8) * 8 + 1, 1, 5, spr);
		}

		SDL_SetRenderDrawColor(renderer, onClr.r, onClr.g, onClr.b, 0xFF);
		for (int y = 0; y < disp.getHeight(); y++) {
			for (int x = 0; x < disp.getWidth(); x++) {
				if (disp.getPixel(x, y)) {
					pixelRect.x = scale * x, pixelRect.y = scale * y;
					pixelRect.w = pixelRect.h = scale;

					SDL_RenderFillRect(renderer, &pixelRect);
				}
			}
		}



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

