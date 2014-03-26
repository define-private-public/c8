// File:         sys_internals.cpp
// Author:       Benjamin N. Summerton
// Description:  Implementation for the Sys Internal classes


#include "sys_internals.h"



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
	for (int i = 0; i < _stackSize; i++)
		_stack[i] = other._stack[i];
}


// Assignment operator
Stack &Stack::operator=(const Stack &other) {
	if (this == &other)
		return *this;
	
	_stackSize = other._stackSize;
	_sp = other._sp;

	// Allocate for the stack and copy the other one
	_stack = new reg16[_stackSize];
	for (int i = 0; i < _stackSize; i++)
		_stack[i] = other._stack[i];

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
// Special registers
reg16 I;		// for graphics
reg16 PC;		// Program Counter
reg8 DT;		// Delay Timer
reg8 ST;		// Sound timer

// Programmer visible
reg8 V[NUM_GPR];	

