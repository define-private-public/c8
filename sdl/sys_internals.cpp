// File:         sys_internals.cpp
// Author:       Benjamin N. Summerton
// Description:  Implementation for the Sys Internal classes


#include "sys_internals.h"
#include <cstring>
#include <cstdio>



/*== the Memory class ==*/

// Sets up the memory file for us
Memory::Memory(int memSize) :
	_memSize(memSize),
	_programSize(0)
{
	// Allocate some fresh memory
	_mem = new unsigned char[_memSize];
	_loadFont();	// Load up the default CHIP-8 Font
}


// Deconstructor
Memory::~Memory() {
	delete[] _mem;
}


Memory::Memory(const Memory &other) :
	_memSize(other._memSize),
	_programSize(other._programSize)
{
	// Don't have any extra lingering memory
	if (_mem)
		delete[] _mem;

	// Load up some fresh memory and copy the other stuff over
	_mem = new unsigned char[_memSize];
	memcpy(_mem, other._mem, _memSize);
}


// Assignment operator overload
Memory &Memory::operator=(const Memory &other) {
	if (this == &other)
		return *this;
	
	// Don't have any extra lingering memory
	if (_mem)
		delete[] _mem;

	// Copy everything over
	_memSize = other._memSize;
	_programSize = other._programSize;
	_mem = new unsigned char[_memSize];
	memcpy(_mem, other._mem, _memSize);

	return *this;
}


// Read a single byte from memroy at a specific loation
//
// Will return a 0 if the address exetnds upon current memory size
// Else it will return the byte at that spot in memory
reg8 Memory::readByte(reg16 addr) {
	if (addr > _memSize)
		return 0;
	
	return _mem[addr];
}


// Write a byte to memory
//
// Returns -1 if the location is bad
// Returns 0 if the byte was successfully written
int Memory::writeByte(reg16 addr, reg8 data) {
	if (addr > _memSize)
		return -1;

	// Write the byte then succed
	_mem[addr] = data;
	return 0;
}


// Loads up a CHIP-8 program and places it into the memory file starting at locaiton 0x200
//
// Returns -1 if the file cannot be opened
// Returns 0 on success
int Memory::loadCHIP8Program(char *filename) {
	// Vars we need
	int i, c;
	FILE *c8File = NULL;

	// Open the file first
	c8File = fopen(filename, "r");
	if (!c8File)
		return -1;
	
	// Read in each byte
	c = fgetc(c8File);
	i = MEM_PROG_START;
	while ((c != EOF) && (i < _memSize)) {
		_mem[i++] = (unsigned char)c;
		c = fgetc(c8File);
	}

	// Set the program size
	_programSize = i - MEM_PROG_START;

	// close the file and exit with success
	fclose(c8File);
	return 0;
}


// Overwrite all of memory with a single dump from a block of data
//
// NOTE: Do not use this function unless you've read the code below, so you can know the
//       ramifications of something so powerful.        (...)       (It's a joke.)
//
// Returns 0 on success
int Memory::load(unsigned char *data, int size) {
	// First clean out the memory
	memset(_mem, 0, _memSize);
	memcpy(_mem, data, size);

	return 0;
}


// Dump all of memory into `dest`.
//
// Returns the number of bytes copied into `dest`
int Memory::dump(unsigned char *dest) {
	memcpy(dest, _mem, _memSize);
	return _memSize;
}


// Returns the current program size in bytes
int Memory::getProgramSize() {
	return _programSize;
}


// Returns the current size of allocated memory
int Memory::getMemorySize() {
	return _memSize;
}


// Sets the default font into memory
void Memory::_loadFont() {
	const int fontHeight = 5;
	
	// 0
	_mem[MEM_FONT_START + 0] = 0xF0;
	_mem[MEM_FONT_START + 1] = 0x90;
	_mem[MEM_FONT_START + 2] = 0x90;
	_mem[MEM_FONT_START + 3] = 0x90;
	_mem[MEM_FONT_START + 4] = 0xF0;

	// 1
	_mem[MEM_FONT_START + (1 * fontHeight) + 0] = 0x20;
	_mem[MEM_FONT_START + (1 * fontHeight) + 1] = 0x60;
	_mem[MEM_FONT_START + (1 * fontHeight) + 2] = 0x20;
	_mem[MEM_FONT_START + (1 * fontHeight) + 3] = 0x20;
	_mem[MEM_FONT_START + (1 * fontHeight) + 4] = 0x70;

	// 2
	_mem[MEM_FONT_START + (2 * fontHeight) + 0] = 0xF0;
	_mem[MEM_FONT_START + (2 * fontHeight) + 1] = 0x10;
	_mem[MEM_FONT_START + (2 * fontHeight) + 2] = 0xF0;
	_mem[MEM_FONT_START + (2 * fontHeight) + 3] = 0x80;
	_mem[MEM_FONT_START + (2 * fontHeight) + 4] = 0xF0;

	// 3
	_mem[MEM_FONT_START + (3 * fontHeight) + 0] = 0xF0;
	_mem[MEM_FONT_START + (3 * fontHeight) + 1] = 0x10;
	_mem[MEM_FONT_START + (3 * fontHeight) + 2] = 0xF0;
	_mem[MEM_FONT_START + (3 * fontHeight) + 3] = 0x10;
	_mem[MEM_FONT_START + (3 * fontHeight) + 4] = 0xF0;

	// 4
	_mem[MEM_FONT_START + (4 * fontHeight) + 0] = 0x90;
	_mem[MEM_FONT_START + (4 * fontHeight) + 1] = 0x90;
	_mem[MEM_FONT_START + (4 * fontHeight) + 2] = 0xF0;
	_mem[MEM_FONT_START + (4 * fontHeight) + 3] = 0x10;
	_mem[MEM_FONT_START + (4 * fontHeight) + 4] = 0x10;

	// 5
	_mem[MEM_FONT_START + (5 * fontHeight) + 0] = 0xF0;
	_mem[MEM_FONT_START + (5 * fontHeight) + 1] = 0x80;
	_mem[MEM_FONT_START + (5 * fontHeight) + 2] = 0xF0;
	_mem[MEM_FONT_START + (5 * fontHeight) + 3] = 0x10;
	_mem[MEM_FONT_START + (5 * fontHeight) + 4] = 0xF0;

	// 6
	_mem[MEM_FONT_START + (6 * fontHeight) + 0] = 0xF0;
	_mem[MEM_FONT_START + (6 * fontHeight) + 1] = 0x80;
	_mem[MEM_FONT_START + (6 * fontHeight) + 2] = 0xF0;
	_mem[MEM_FONT_START + (6 * fontHeight) + 3] = 0x90;
	_mem[MEM_FONT_START + (6 * fontHeight) + 4] = 0xF0;

	// 7
	_mem[MEM_FONT_START + (7 * fontHeight) + 0] = 0xF0;
	_mem[MEM_FONT_START + (7 * fontHeight) + 1] = 0x10;
	_mem[MEM_FONT_START + (7 * fontHeight) + 2] = 0x20;
	_mem[MEM_FONT_START + (7 * fontHeight) + 3] = 0x40;
	_mem[MEM_FONT_START + (7 * fontHeight) + 4] = 0x40;

	// 8
	_mem[MEM_FONT_START + (8 * fontHeight) + 0] = 0xF0;
	_mem[MEM_FONT_START + (8 * fontHeight) + 1] = 0x90;
	_mem[MEM_FONT_START + (8 * fontHeight) + 2] = 0xF0;
	_mem[MEM_FONT_START + (8 * fontHeight) + 3] = 0x90;
	_mem[MEM_FONT_START + (8 * fontHeight) + 4] = 0xF0;

	// 9
	_mem[MEM_FONT_START + (9 * fontHeight) + 0] = 0xF0;
	_mem[MEM_FONT_START + (9 * fontHeight) + 1] = 0x90;
	_mem[MEM_FONT_START + (9 * fontHeight) + 2] = 0xF0;
	_mem[MEM_FONT_START + (9 * fontHeight) + 3] = 0x10;
	_mem[MEM_FONT_START + (9 * fontHeight) + 4] = 0xF0;

	// A
	_mem[MEM_FONT_START + (0xA * fontHeight) + 0] = 0xF0;
	_mem[MEM_FONT_START + (0xA * fontHeight) + 1] = 0x90;
	_mem[MEM_FONT_START + (0xA * fontHeight) + 2] = 0xF0;
	_mem[MEM_FONT_START + (0xA * fontHeight) + 3] = 0x90;
	_mem[MEM_FONT_START + (0xA * fontHeight) + 4] = 0x90;

	// B
	_mem[MEM_FONT_START + (0xB * fontHeight) + 0] = 0xE0;
	_mem[MEM_FONT_START + (0xB * fontHeight) + 1] = 0x90;
	_mem[MEM_FONT_START + (0xB * fontHeight) + 2] = 0xE0;
	_mem[MEM_FONT_START + (0xB * fontHeight) + 3] = 0x90;
	_mem[MEM_FONT_START + (0xB * fontHeight) + 4] = 0xE0;

	// C
	_mem[MEM_FONT_START + (0xC * fontHeight) + 0] = 0xF0;
	_mem[MEM_FONT_START + (0xC * fontHeight) + 1] = 0x80;
	_mem[MEM_FONT_START + (0xC * fontHeight) + 2] = 0x80;
	_mem[MEM_FONT_START + (0xC * fontHeight) + 3] = 0x80;
	_mem[MEM_FONT_START + (0xC * fontHeight) + 4] = 0xF0;

	// D
	_mem[MEM_FONT_START + (0xD * fontHeight) + 0] = 0xE0;
	_mem[MEM_FONT_START + (0xD * fontHeight) + 1] = 0x90;
	_mem[MEM_FONT_START + (0xD * fontHeight) + 2] = 0x90;
	_mem[MEM_FONT_START + (0xD * fontHeight) + 3] = 0x90;
	_mem[MEM_FONT_START + (0xD * fontHeight) + 4] = 0xE0;

	// E
	_mem[MEM_FONT_START + (0xE * fontHeight) + 0] = 0xF0;
	_mem[MEM_FONT_START + (0xE * fontHeight) + 1] = 0x80;
	_mem[MEM_FONT_START + (0xE * fontHeight) + 2] = 0xF0;
	_mem[MEM_FONT_START + (0xE * fontHeight) + 3] = 0x80;
	_mem[MEM_FONT_START + (0xE * fontHeight) + 4] = 0xF0;

	// F
	_mem[MEM_FONT_START + (0xF * fontHeight) + 0] = 0xF0;
	_mem[MEM_FONT_START + (0xF * fontHeight) + 1] = 0x80;
	_mem[MEM_FONT_START + (0xF * fontHeight) + 2] = 0xF0;
	_mem[MEM_FONT_START + (0xF * fontHeight) + 3] = 0x80;
	_mem[MEM_FONT_START + (0xF * fontHeight) + 4] = 0x80;
}




/*== the Stack class ==*/

// Constructor
Stack::Stack(int stackSize) :
	_stackSize(stackSize),
	_sp(0)
{
	_stack = new reg16[_stackSize];
}


// Deconstructor
Stack::~Stack() {
	delete[] _stack;
}


// Copy constructor
Stack::Stack(const Stack &other) :
	_stackSize(other._stackSize),
	_sp(other._sp)
{
	// Cleanup
	if (_stack)
		delete[] _stack;

	// allocate for the stack and copy the other one.
	_stack = new reg16[_stackSize];
	memcpy(_stack, other._stack, _stackSize);
}


// Assignment operator
Stack &Stack::operator=(const Stack &other) {
	if (this == &other)
		return *this;

	// Cleanup
	if (_stack)
		delete[] _stack;

	// Set variables
	_stackSize = other._stackSize;
	_sp = other._sp;

	// Allocate for the stack and copy the other one
	_stack = new reg16[_stackSize];
	memcpy(_stack, other._stack, _stackSize);

	return *this;
}


// Pushes new addresses onto the stack
//
// Returns -1 if the stack is already full
// Returns 0 if the address was succesfully added
int Stack::push(reg16 addr) {
	if (atMax())
		return -1;
	
	// Still have space left on the stack, add `addr` to it
	_stack[_sp++] = addr;
	return 0;
}


// Pops an address off of the stack
//
// Returns 0 if the stack is empty
// Returns the latest SP entry otherwise
reg16 Stack::pop() {
	if (empty())
		return 0;

	// Return the latest address
	return _stack[--_sp];
}


// Have we filled up our stack?
bool Stack::atMax() {
	return (_sp == _stackSize);
}


// Check to see if nothing is on our stack
bool Stack::empty() {
	return (_sp == 0);
}


// Returns the size that we set the stack to
int Stack::getStackSize() {
	return _stackSize;
}


// Returns the location of where the stack pointer currently is pointing to
int Stack::getStackPointer() {
	return _sp;
}


/*== Global variables: ==*/
// CPU speed
int cpuSpeed = 60;	// Measured in Hertz

// Special registers
reg16 I;			// for graphics
reg16 PC;			// Program Counter
reg8 DT;			// Delay Timer
reg8 ST;			// Sound timer

// Programmer visible
reg8 V[NUM_GPR];	

