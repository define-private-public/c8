// File:         save_state.cpp
// Author:       Benjamin N. Summerton
// Description:  Implementation for the savestate functions


#include "save_state.h"
#include <cstdio>


// Writes a savestate to a file,  returns 0 on success, other stuff on failure
int writeSaveState(const char *filename, CPU *cpu, Stack *stack, Memory *mem, Display *disp) {
	FILE *saveFile;

	// First open the file and check that we can write to it.
	saveFile = fopen(filename, "wb");
	if (!saveFile)
		return 1;

	// Pull out the CPU information first
	reg16 pc = cpu->getPC();
	reg16 i = cpu->getI();
	reg8 dt = cpu->getDT();
	reg8 st = cpu->getST();
	unsigned char waitingForInput = cpu->waitingForInput() ? 1 : 0;
	unsigned char inputReg = cpu->getInputReg();
	reg8 v[CPU_NUM_GPR];
	for (int j = 0; j < CPU_NUM_GPR; j++)
		v[j] = cpu->getV(j);

	// The write it
	fwrite(&pc, sizeof(pc), 1, saveFile);
	fwrite(&i, sizeof(i), 1, saveFile);
	fwrite(&dt, sizeof(dt), 1, saveFile);
	fwrite(&st, sizeof(st), 1, saveFile);
	fwrite(&waitingForInput, sizeof(waitingForInput), 1, saveFile);
	fwrite(&inputReg, sizeof(inputReg), 1, saveFile);
	fwrite(v, sizeof(v[0]), CPU_NUM_GPR, saveFile);


	// Get the stack data
	unsigned char ss = stack->getStackSize();
	unsigned char sp = stack->getStackPointer();
	reg16 *s = stack->dump();	// Dynamic

	// Write stack data
	fwrite(&ss, sizeof(ss), 1, saveFile);
	fwrite(&sp, sizeof(sp), 1, saveFile);
	fwrite(s, sizeof(s[0]), ss, saveFile);


	// Memory
	unsigned char memory[MEM_4KB];
	mem->dump(memory);
	fwrite(memory, sizeof(memory[0]), mem->getMemorySize(), saveFile);


	// Display data
	unsigned char width = disp->getWidth();
	unsigned char height = disp->getHeight();
	unsigned char *pixels = disp->dump(); 

	// Save the rest of the display data
	fwrite(&width, sizeof(width), 1, saveFile);
	fwrite(&height, sizeof(height), 1, saveFile);
	fwrite(pixels, sizeof(pixels[0]), width * height, saveFile);


	// Cleanup
	fclose(saveFile);
	delete[] s;
	delete[] pixels;
	return 0;
}


int loadSaveState(const char *filename, CPU *cpu, Stack *stack, Memory *mem, Display *disp) {

	return 0;
}


