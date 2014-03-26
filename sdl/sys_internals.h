// File:         sys_internals.h
// Author:       Benjamin N. Summerton
// Description:  This file contains information and important data about CHIP-8 system internals.
//               e.g. Registers, Memeory, the Stack.  While these could be split into separate
//               files, that's a bit too much of over-engineering


#ifndef C8_SYS_INTERNALS_H
#define C8_SYS_INTERNALS_H

#define NUM_GPR 16 				// Number of General purpose registers
#define STACK_SIZE 16			// Defualt stack size for more systems
#define MEM_4KB 0x1000			// Defualt memory size
#define MEM_FONT_START 0x050	// Start of the built in CHIP-8 Font
#define MEM_PROG_START 0x200	// Default program starting point


// Sometypedefs, though possibly unecessary, kind of useful
typedef unsigned char reg8;
typedef unsigned short reg16;


// The memory file
class Memory {
public:
	// Constructors
	Memory(int memSize);
	~Memory();
	Memory(const Memory &other);

	// Overloads
	Memory &operator=(const Memory &other);

	// Memory operations
	reg8 readByte(reg16 addr);
	int writeByte(reg16 addr, reg8 data);
	int loadCHIP8Program(char *filename);

	// NOTE: Don't use the methods unless you know what you're doing, thing of them as a back door
	int load(unsigned char *data, int size);		// Overwrite all of memory with a specific block
	int dump(unsigned char *dest);					// Perform a dump on memory

	// Accessors
	int getProgramSize();
	int getMemorySize();

	// TODO: Add in CHIP-8 Interpreter & Font?
	// TODO: Add in Dump to file?

private:
	int _programSize;		// Size of the loaded CHIP-8 program
	int _memSize;			// Total size of the memory
	unsigned char *_mem;	// The actual memory being referenced

	// I know there might some inconsistancies with using `unisgned char` instead of `reg8` here,
	// but I thought this is a bit more descriptive;  reg8 is meant for registers
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

