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
	Buzzer(const char *filename);
	~Buzzer();

	void play();
	void stop();

private:
	Mix_Chunk *_wav;
	int _channel;
};

#endif 	// C8_BUZZER_H


