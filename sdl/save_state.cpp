// File:         save_state.cpp
// Author:       Benjamin N. Summerton
// Description:  Implementation for the savestate functions


#include "save_state.h"
#include <cstdio>
#include <cstring>


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


// Load a save state file
int loadSaveState(const char *filename, CPU *cpu, Stack *stack, Memory *mem, Display *disp) {
	FILE *saveFile;

	// First open the file and check that we can write to it.
	saveFile = fopen(filename, "rb");
	if (!saveFile)
		return 1;

	// Pull out the CPU information first
	reg16 pc;
	reg16 i;
	reg8 dt;
	reg8 st;
	unsigned char waitingForInput;
	unsigned char inputReg;
	reg8 v[CPU_NUM_GPR];
	fread(&pc, sizeof(pc), 1, saveFile);
	fread(&i, sizeof(i), 1, saveFile);
	fread(&dt, sizeof(dt), 1, saveFile);
	fread(&st, sizeof(st), 1, saveFile);
	fread(&waitingForInput, sizeof(waitingForInput), 1, saveFile);
	fread(&inputReg, sizeof(inputReg), 1, saveFile);
	fread(v, sizeof(v[0]), CPU_NUM_GPR, saveFile);

	cpu->load(pc, i, dt, st, v, waitingForInput, inputReg);


	// Get the stack data
	unsigned char ss;
	unsigned char sp;
	fread(&ss, sizeof(ss), 1, saveFile);
	fread(&sp, sizeof(sp), 1, saveFile);

	reg16 s[ss];
	fread(s, sizeof(s[0]), ss, saveFile);

	stack->load(s, sp, ss);


	// Memory
	unsigned char memory[MEM_4KB];
	fread(memory, sizeof(memory[0]), MEM_4KB, saveFile);
	mem->load(memory, MEM_4KB);


	// Display data
	unsigned char width;
	unsigned char height;
	fread(&width, sizeof(width), 1, saveFile);
	fread(&height, sizeof(height), 1, saveFile);

	unsigned char pixels[width * height]; 
	fread(pixels, sizeof(pixels[0]), width * height, saveFile);

	disp->load(width, height, pixels);


	// Cleanup
	fclose(saveFile);

	return 0;
}


