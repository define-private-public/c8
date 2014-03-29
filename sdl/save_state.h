// File:         save_state.h
// Author:       Benjamin N. Summerton
// Description:  Function prototypes for read and writing from a save-state

#ifndef C8_SAVE_STATE_H
#define C8_SAVE_STATE_H 


#include "sys_internals.h"
#include "display.h"
#include "cpu.h"


// Functions
int writeSaveState(const char *filename, CPU *cpu, Stack *stack, Memory *mem, Display *disp);
int loadSaveState(const char *filename, CPU *cpu, Stack *stack, Memory *mem, Display *disp);


#endif 	// C8_SAVE_STATE_H

