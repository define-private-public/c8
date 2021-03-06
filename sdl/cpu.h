// File:         cpu.h
// Author:       Benjamin N. Summerton
// Description:  The "heart," of the CPU that takes in an OP-Code, then will perform that operation


#ifndef C8_CPU_H
#define C8_CPU_H

#include "sys_internals.h"
#include "display.h"


// Definitions
#define CPU_NUM_GPR 16 						// Number of General purpose registers
#define CPU_DEFAULT_CLOCK_SPEED 60		// 60 clock cyles a second


// The CPU should really be a singleton, but you can make multiple ones
class CPU {
public:
	// Constructors
	CPU(int clockSpeed);
	~CPU();

	// CPU operations
	int executeNextOperation();			// Decode & Execute
	void decrementTimers();
	bool readyToRun();
	void getKeyPress(reg8 key);			// Technically should be in the lower list, but it's a CPU OP

	// Mutators
	void setMemory(Memory *mem);
	void setStack(Stack *stack);
	void setDisplay(Display *display);

	// Accessors
	reg16 getPC();
	reg16 getI();
	reg8 getDT();
	reg8 getST();
	reg8 getV(int id);
	int getClockSpeed();
	reg16 getCurInstruction();
	bool waitingForInput();
	int getInputReg();			// Needed for savestate

	// Varibles
	bool keyDown[16];			// Technically not part of the "CPU," but needed for some instructions
	bool stepModeOn;			// If you want to do step by step executution (a.k.a. debugging)
	int step;					// For step by step execution
	bool debugInfo;				// Turn on Debug information

	// Save state functions
	void load(reg16 pc, reg16 i, reg8 dt, reg8 st, reg8 *v, unsigned char waitingForInput, unsigned char inputReg);


private:
	int _clockSpeed;			// Measured in Hertz
	
	// Special registers
	reg16 _PC;					// Program Counter
	reg16 _I;					// for graphics
	reg8 _DT;					// Delay Timer
	reg8 _ST;					// Sound timer
	
	// Programmer visible
	reg8 _V[CPU_NUM_GPR];		// The V registers, V[F] though should be treated as a flag
	
	// Things we need a reference to, not part of the CPU, but nceccessary for operations
	Stack *_curStack;
	Memory *_curMem;
	Display *_curDisplay;

	// Other stuff
	reg16 _curInst;
	bool _waitingForInput;
	int _inputReg;

	// Internal functions
	int _fetch();				// Sets the next instruction for us
	void _printDebugInfo();		// Prints debugging information to standard output


	// TODO: Things for input
};


#endif	// C8_CPU_H

