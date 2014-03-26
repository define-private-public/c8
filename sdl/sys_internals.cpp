// File:         sys_internals.cpp
// Author:       Benjamin N. Summerton
// Description:  Implementation for the Sys Internal classes


#include "sys_internals.h"
#include <cstring>



/*== the Memory class ==*/

// Sets up the memory file for us
Memory::Memory(int memSize) :
	_memSize(memSize),
	_programSize(0)
{
	// Allocate some fresh memory
	_mem = new unsigned char[_memSize];
}


// Deconstructor
Memory::~Memory() {
	delete[] _mem;
}


Memory::Memory(const Memory &other) :
	_memSize(other._memSize),
	_programSize(other.programSize)
{
	// Load up some fresh memory and copy the other stuff over
	_mem = new unsigned char[_memSize];
	memcpy(_mem, other._mem, _memSize);
}


Memory &Memory::operator=(const Memory &other) { }


reg8 Memory::readByte() { }


int Memory::writeByte(reg8 data) { }


int Memory::loadCHIP8Program(char *filename) { }


int Memory::load(unsigned char *data, int size) { }


int Memory::dump(unsigned char *dest) { }


int Memory::getProgramSize() { }


int Memory::getMemorySize() { }


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
	// allocate for the stack and copy the other one.
	_stack = new reg16[_stackSize];
	memcpy(_stack, other._stack, _stackSize);
}


// Assignment operator
Stack &Stack::operator=(const Stack &other) {
	if (this == &other)
		return *this;
	
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

