// File:         buzzer.h
// Author:       Benjamin N. Summerton
// Description:  Interface for controlling a little "beep" that makes a buzzer noise


#ifndef C8_BUZZER_H
#define C8_BUZZER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

// Buzzer Class
// NOTE: This class should be a singleton
class Buzzer {
public:
	// Constructors
	Buzzer(char *filename);
	~Buzzer();

	void play();

private:
	static bool _soundLoaded;
	static Mix_Chunk *_wav;
};

#endif 	// C8_BUZZER_H


