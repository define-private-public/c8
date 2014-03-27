// File:         main.cpp
// Author:       Benjamin N. Summerton 
// Description:  Main source file for c8's SDL2 emulator


// C++ includes
#include <iostream>
using namespace std;

// SDL2 includes
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

// c8 includes
#include "sys_internals.h"
#include "display.h"
#include "buzzer.h"
#include "cpu.h"


// Defitions
#define BEEP_PATH "../beep.wav"


// for the screen
Display disp(64, 32);
const int scale = 4;
const int screenWidth = disp.getWidth() * scale;
const int screenHeight = disp.getHeight() * scale;

// for the audio
int audioRate = 44100;
Uint16 audioFormat = AUDIO_S16;		// 16 Bit Stero
int audioChannels = 1;
int audioBuffers = 4096;

// Others
Memory mem(MEM_4KB);
Stack stack(DEFAULT_STACK_SIZE);
CPU cpu(CPU_DEFAULT_CLOCK_SPEED);



// Main program function
int main(int argc, char *argv[]) {
	// SDL Variables
	bool quit = false;
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Event event;
	SDL_Color onClr = {0x00, 0xD0, 0x00}, offClr = {0x00, 0x20, 0x00};
	SDL_AudioSpec beepSpec;
	
	// For graphics
	SDL_Rect pixelRects[disp.getWidth() * disp.getHeight()];
	int pixelsOn = 0;


	// Print out a use message
	if (argc < 2) {
		cout << "Need to specify a ROM file" << endl;
		cout << endl;
		cout << "Usage:" << endl;
		cout << argv[0] << " <CHIP-8 ROM>" << endl;
		exit(0);
	}

	
	/*== Initialize the SDL stuff ==*/
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

	// Setup the Mixer
	if (Mix_OpenAudio(audioRate, audioFormat, audioChannels, audioBuffers)) {
		cerr << "Could not setup the audio mixer." << endl;
		exit(1);
	}

	
	/*== Other Setup Junk ==*/
	Buzzer buzzer(BEEP_PATH);	// Needs to be setup after audio device has been opened

	// Setup the pixel rectangle dimensions
	for (int i = 0; i < (disp.getWidth() * disp.getHeight()); i++)
		pixelRects[i].w = pixelRects[i].h = scale;


	// Some last minute CPU things
	cpu.setMemory(&mem);
	cpu.setStack(&stack);
	cpu.setDisplay(&disp);

	// Load up the file specified
	mem.loadCHIP8Program(argv[1]);

	if (!cpu.readyToRun()) {
		cerr << "The CPU object reported that it wasn't ready to run." << endl;
		cerr << "Memory, Stack, or Display objects were not linked up properly." << endl;
		exit(1);
	}

	
	/*== Main program loop ==*/
	SDL_ShowCursor(0);
	while (!quit) {
		while (SDL_PollEvent(&event)) {
			// Window exit
			if (event.type == SDL_QUIT)
				quit = true;

			if (event.type == SDL_KEYUP) {
				// ESC key press
				if (event.key.keysym.sym == SDLK_ESCAPE)
					quit = true;
			}
		}


		/*== Timers ==*/
		// Buzzer
		if (cpu.getST() > 0)
			buzzer.play();
		else
			buzzer.stop();

		cpu.decrementTimers();


		/*== Fetch, Decode, Execute ==*/
		cpu.executeNextOperation();


		/*== Drawing ==*/
		// Queue up the pixels that need to be drawn
		pixelsOn = 0;
		for (int y = 0; y < disp.getHeight(); y++) {
			for (int x = 0; x < disp.getWidth(); x++) {
				if (disp.getPixel(x, y)) {
					pixelRects[pixelsOn].x = scale * x, pixelRects[pixelsOn].y = scale * y;
					pixelsOn++;
				}
			}
		}

		// Clear the screen then draw the pixels
		SDL_SetRenderDrawColor(renderer, offClr.r, offClr.g, offClr.b, 0xFF);
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, onClr.r, onClr.g, onClr.b, 0xFF);
		SDL_RenderFillRects(renderer, pixelRects, pixelsOn);

		// Flip the screen and hold
		SDL_RenderPresent(renderer);
		SDL_Delay(1000 / cpu.getClockSpeed());		// In milliseconds
	}

	// Cleanup and exit
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	Mix_CloseAudio();
	SDL_Quit();
	return 0;
}

