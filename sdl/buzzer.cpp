// File:         buzzer.cpp
// Author:       Benjamin N. Summerton
// Description:  Implementation code for the Buzzer module


#include "buzzer.h"

// Constructor
Buzzer::Buzzer(const char *filename) :
	_channel(-1)
{
	_wav = Mix_LoadWAV(filename);
}


// Deconstructor
Buzzer::~Buzzer() {
	if (_wav) {
		Mix_FreeChunk(_wav);
		_wav = NULL;
	}
}


// Star the buzzer
void Buzzer::play() {
	// Startup the noise if it isn't playing
	if ((_channel < 0) && (_wav != NULL))
		_channel = Mix_PlayChannel(-1, _wav, -1);	// Loop on any channel
}


// Stop the buzzing noise from playing
void Buzzer::stop() {
	if ((_channel >= 0) && (_wav != NULL)) {
		Mix_HaltChannel(_channel);
		_channel = -1;
	}
}


