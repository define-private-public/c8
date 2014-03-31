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
#include "save_state.h"

// Defitions
#define BEEP_PATH "../beep.wav"

/*== Key Map:
*
*      COSMAC:          Keyboard:
*
*   1 | 2 | 3 | C      1 | 2 | 3 | 4
*  ---------------    ---------------
*   4 | 5 | 6 | D      Q | W | E | R
*  ---------------    ---------------
*   7 | 8 | 9 | E      A | S | D | F
*  ---------------    ---------------
*   A | 0 | B | F      Z | X | C | V
*
==*/


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
	SDL_Color onClr = {0xE0, 0xEE, 0xEE}, offClr = {0x00, 0x00, 0x9C};
	SDL_AudioSpec beepSpec;
	bool saveStatesOn = false;
	cpu.stepModeOn = false;
	
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

	// Turn on save states, third arguement is the save state file
	if (argc >= 3) 
		saveStatesOn = true;

	
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
		/*== Input Loop ==*/
		while (SDL_PollEvent(&event)) {
			// Window exit
			if (event.type == SDL_QUIT)
				quit = true;

			if (event.type == SDL_KEYUP) {
				// ESC key press
				if (event.key.keysym.sym == SDLK_ESCAPE)
					quit = true;

				// not for the block one, just general, key up?
				switch (event.key.keysym.sym) {
					case SDLK_1:
						cpu.keyDown[0x1] = false;
						break;

					case SDLK_2:
						cpu.keyDown[0x2] = false;
						break;

					case SDLK_3:
						cpu.keyDown[0x3] = false;
						break;

					case SDLK_4:
						cpu.keyDown[0xC] = false;
						break;

					case SDLK_q: 
						cpu.keyDown[0x4] = false;
						break;

					case SDLK_w:
						cpu.keyDown[0x5] = false;
						break;

					case SDLK_e:
						cpu.keyDown[0x6] = false;
						break;

					case SDLK_r:
						cpu.keyDown[0xD] = false;
						break;

					case SDLK_a:
						cpu.keyDown[0x7] = false;
						break;

					case SDLK_s:
						cpu.keyDown[0x8] = false;
						break;

					case SDLK_d:
						cpu.keyDown[0x9] = false;
						break;

					case SDLK_f:
						cpu.keyDown[0xE] = false;
						break;

					case SDLK_z:
						cpu.keyDown[0xA] = false;
						break;

					case SDLK_x:
						cpu.keyDown[0x0] = false;
						break;

					case SDLK_c:
						cpu.keyDown[0xB] = false;
						break;

					case SDLK_v:
						cpu.keyDown[0xF] = false;
						break;
				}
			}

			if (event.type == SDL_KEYDOWN) {
				// In the case we have a hold because of input get something
				if (cpu.waitingForInput()) {
					switch (event.key.keysym.sym) {
						case SDLK_1: 
							cpu.getKeyPress(0x1);
							break;

						case SDLK_2:
							cpu.getKeyPress(0x2);
							break;

						case SDLK_3:
							cpu.getKeyPress(0x3);
							break;

						case SDLK_4:
							cpu.getKeyPress(0xC);
							break;

						case SDLK_q: 
							cpu.getKeyPress(0x4);
							break;

						case SDLK_w:
							cpu.getKeyPress(0x5);
							break;

						case SDLK_e:
							cpu.getKeyPress(0x6);
							break;

						case SDLK_r:
							cpu.getKeyPress(0xD); 
							break;

						case SDLK_a:
							cpu.getKeyPress(0x7);
							break;

						case SDLK_s:
							cpu.getKeyPress(0x8);
							break;

						case SDLK_d:
							cpu.getKeyPress(0x9);
							break;

						case SDLK_f:
							cpu.getKeyPress(0xE);
							break;

						case SDLK_z:
							cpu.getKeyPress(0xA);
							break;

						case SDLK_x:
							cpu.getKeyPress(0x0);
							break;

						case SDLK_c:
							cpu.getKeyPress(0xB);
							break;

						case SDLK_v:
							cpu.getKeyPress(0xF); 
							break;
					}
				}

				// not for the block one, just general, key down?
				switch (event.key.keysym.sym) {
					case SDLK_p:
						// Save state save
						if (saveStatesOn) {
							if (writeSaveState(argv[2], &cpu, &stack, &mem, &disp) == 0)
								cout << "Save state written to `" << argv[2] << "`." << endl;
							else
								cout << "Error writing save state to `" << argv[2] << "`." << endl;
						}

						break;

					case SDLK_o:
						// load state save
						if (saveStatesOn) {
							if (loadSaveState(argv[2], &cpu, &stack, &mem, &disp) == 0)
								cout << "Loaded save state `" << argv[2] << "`." << endl;
							else
								cout << "Error loading save state `" << argv[2] << "`." << endl;
						}

						break;

					case SDLK_m:
						// Perform a STEP
						cpu.step = true;
						break;

					case SDLK_1:
						cpu.keyDown[0x1] = true;
						break;

					case SDLK_2:
						cpu.keyDown[0x2] = true;
						break;

					case SDLK_3:
						cpu.keyDown[0x3] = true;
						break;

					case SDLK_4:
						cpu.keyDown[0xC] = true;
						break;

					case SDLK_q: 
						cpu.keyDown[0x4] = true;
						break;

					case SDLK_w:
						cpu.keyDown[0x5] = true;
						break;

					case SDLK_e:
						cpu.keyDown[0x6] = true;
						break;

					case SDLK_r:
						cpu.keyDown[0xD] = true;
						break;

					case SDLK_a:
						cpu.keyDown[0x7] = true;
						break;

					case SDLK_s:
						cpu.keyDown[0x8] = true;
						break;

					case SDLK_d:
						cpu.keyDown[0x9] = true;
						break;

					case SDLK_f:
						cpu.keyDown[0xE] = true;
						break;

					case SDLK_z:
						cpu.keyDown[0xA] = true;
						break;

					case SDLK_x:
						cpu.keyDown[0x0] = true;
						break;

					case SDLK_c:
						cpu.keyDown[0xB] = true;
						break;

					case SDLK_v:
						cpu.keyDown[0xF] = true;
						break;
				}
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

