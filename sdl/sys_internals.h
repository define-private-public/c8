// File:         sys_internals.h
// Author:       Benjamin N. Summerton
// Description:  This file contains information and important data about CHIP-8 system internals.
//               e.g. Registers, Memeory, the Stack.  While these could be split into separate
//               files, that's a bit too much of over-engineering


#ifndef C8_SYS_INTERNALS_H
#define C8_SYS_INTERNALS_H

#define NUM_GPR 16 		// Number of General purpose registers
#define STACK_SIZE 16	// Defualt stack size for more systems


// Sometypedefs, though possibly unecessary, kind of useful
typedef unsigned char reg8;
typedef unsigned short reg16;


class Memory {
	// TODO: Add stuf here	

private:
	int _programSize;
};


// For recursion and stuff
class Stack {
public:
	Stack(int stackSize);
	~Stack();

	Stack(const Stack &other);

	Stack &operator=(const Stack &other);

	// Stack operations
	int push(reg16 addr);
	reg16 pop();

	// Accessors
	bool atMax();
	bool empty();
	int getStackSize();
	int getStackPointer();

private:
	int _sp;			// Stack pointer (points to top of stack)
	int _stackSize;
	reg16 *_stack;
};


/*== Global variables: ==*/
// CPU speed
extern int cpuSpeed;	// Measured in Hertz

// Special registers
extern reg16 I;			// for graphics
extern reg16 PC;		// Program Counter
extern reg8 DT;			// Delay Timer
extern reg8 ST;			// Sound timer

// Programmer visible
extern reg8 V[NUM_GPR];	// The V registers, V[F] though should be treated as a flag





#endif	// C8_SYS_INTERNALS_H

