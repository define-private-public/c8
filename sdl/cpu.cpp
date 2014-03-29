// File:         cpu.cpp
// Author:       Benjamin N. Summerton
// Description:  Implementation for `cpu.h`


#include "cpu.h"
#include <cstdlib>
#include <iostream>
using namespace std;


// Constructor
CPU::CPU(int clockSpeed) :
	_clockSpeed(clockSpeed),
	_I(MEM_FONT_START),
	_PC(MEM_PROG_START),
	_waitingForInput(false)
{	
	// Everything else should be NULL or 0
	for (int i = 0; i < 16; i++)
		keyDown[i] = false;
}


// Deconstructor
CPU::~CPU() {
	// Just do jack shit
}

// CPU operations
// Performs an operation for the system
//
// Will return 0 on success (or waiting for input), -1 on bad things
int CPU::executeNextOperation() {
	// Make sure we don't do anything until we are not waiting for input
	if (waitingForInput())
		return 0;

	// Get the next instruction
	if (_fetch())
		return -1;

	// For decoding the instructoin
	int opc = (_curInst & 0xF000) >> 12;
	int nnn = _curInst & 0x0FFF;			// 12 bit mem. addr.
	int nn = _curInst & 0x00FF;				// 8 bit constant
	int n = _curInst & 0x000F;				// 4 bit constant
	int x = (_curInst & 0x0F00) >> 8;		// 4 bit reg. ID
	int y = (_curInst & 0x00F0) >> 4;		// 4 bit reg. ID

	// Other variables
	int copy, factor;
	reg8 count;


	/*== Perform the operations ==*/
	switch (opc) {
		case 0x0:
			switch (nn) {
				case 0xE0:
					// Cear the screen
					_curDisplay->clearDisplay();
					_PC += INST_SIZE;
					break;

				case 0xEE:
					// Return from a subroutine
					_PC = _curStack->pop();
					_PC += INST_SIZE;
					break;

				default:
					return -1;
			}
			break;

		case 0x1:
			// Jump to mem. addr. at `nnn`
			_PC = nnn;
			break;

		case 0x2:
			// Run subroutine at location `nnn`
			_curStack->push(_PC);
			_PC = nnn;
			break;

		case 0x3:
			// Skip the next inst. if the value of V[X] is equal to `nn`
			if (_V[x] == nn)
				_PC += INST_SIZE;

			_PC += INST_SIZE;
			break;

		case 0x4:
			// Skip the next inst. if the value of V[X] is not equal to `nn`
			if (_V[x] != nn)
				_PC += INST_SIZE;

			_PC += INST_SIZE;
			break;

		case 0x5:
			// Skip the next inst. if the value of V[x] is equal to the value of V[y]
			if (_V[x] == _V[y])
				_PC += INST_SIZE;

			_PC += INST_SIZE;
			break;

		case 0x6:
			// Set the value of V[x] equal to `nn`
			_V[x] = nn;
			_PC += INST_SIZE;
			break;

		case 0x7:
			// Add the value `nn` to the value of V[x].
			_V[x] += nn;		// NOTE: This may cause a wraparound
			_PC += INST_SIZE;
			break;

		case 0x8:
			/*== Arithmetic operaitons ==*/
			switch (n) {
				case 0x0:
					// Store the value of V[y] into V[x]
					_V[x] = _V[y];
					_PC += INST_SIZE;
					break;

				case 0x1:
					// OR the value of V[x] with V[y]
					_V[x] |= _V[y];
					_PC += INST_SIZE;
					break;

				case 0x2:
					// AND the value of V[x] with V[y]
					_V[x] &= _V[y];
					_PC += INST_SIZE;
					break;

				case 0x3:
					// XOR the value of V[x] with V[y]
					_V[x] ^= _V[y];
					_PC += INST_SIZE;
					break;

				case 0x4:
					// Add the value of V[y] to V[x], set V[F] to 1 is there is a carry
					if ((_V[x] + _V[y]) > 0xFF)
						_V[0xF] = 1;
					else
						_V[0xF] = 0;

					_V[x] += _V[y];
					_PC += INST_SIZE;
					break;

				case 0x5:
					// Subtract the value of V[y] from V[x], set V[F] to 1 if there isn't a borrow
					if ((_V[x] - _V[y]) < 0)
						_V[0xF] = 0;
					else
						_V[0xF] = 1;
					
					_V[x] -= _V[y];
					_PC += INST_SIZE;
					break;

				case 0x6:
					// Shift the value in V[y] over by one bit to the right, store it in V[x], and set V[F] to
					// the value of the LSB in V[y] before the shift
					_V[0xF] = _V[y] & 0x1;	// Get the LSB
					_V[x] = _V[y] >> 1;
					_PC += INST_SIZE;
					break;

				case 0x7:
					// Set V[x] to the value of V[y] minus V[x]; set V[F] to 1 if there isn't a borrow
					if ((_V[y] - _V[x]) < 0)
						_V[0xF] = 0;
					else
						_V[0xF] = 1;
					
					_V[x] = _V[y] - _V[x];
					_PC += INST_SIZE;
					break;

				case 0xE:
					// Shift the value in V[y] over by one bit to the left, store it in V[x], and set V[F] to
					// the value of the MSB in V[y] before the shift
					_V[0xF] = _V[y] & 0x80;	// Get the MSB
					_V[x] = _V[y] << 1;
					_PC += INST_SIZE;
					break;

			}
			break;

		case 0x9:
			// Skip the next inst. if the value of V[x] is not equal to the value of V[y]
			if (_V[x] != _V[y])
				_PC += INST_SIZE;

			_PC += INST_SIZE;
			break;

		case 0xA:
			// Set I equal to the mem. addr. `nnn`
			_I = nnn;
			_PC += INST_SIZE;
			break;

		case 0xB:
			// Set the program counter to the address `nnn` plus the value of V[0]
			_PC = nnn + _V[0];
			break;

		case 0xC:
			// Generate a "random," number and put it into V[x]; AND mask it with `nn`
			_V[x] = rand() & nn;
			_PC += INST_SIZE;
			break;

		case 0xD:
			// Draw a sprite at location (V[x], V[y]) of 8 by `n` pixels big
			// Get the sprite data starting at mem location I
			
			// Get the sprite data
			unsigned char spriteData[n];
			for (int i = 0; i < n; i++)
				spriteData[i] = _curMem->readByte(_I + i);

			// Draw the sprite and set the V[F] register if there was a collision
			_V[0xF] = _curDisplay->drawSprite(_V[x], _V[y], n, spriteData);
			_PC += INST_SIZE;
			break;

		case 0xE:
			// Skip based upon input
			switch (nn) {
				case 0x9E:
					// Skip the next inst. if the Key for the hex value in V[x] is pressed
					if (keyDown[_V[x]])
						_PC += INST_SIZE;

					_PC += INST_SIZE;
					break;

				case 0xA1:
					// Skip the next inst. if the Key for the hex value in V[x] is released
					if (!keyDown[_V[x]])
						_PC += INST_SIZE;

					_PC += INST_SIZE;
					break;

				default:
					return -1;
			}
			break;

		case 0xF:
			// Just a bunch of random operations
			switch (nn) {	
				case 0x07:
					// Store the value in DT into _V[x]
					_V[x] = _DT;
					_PC += INST_SIZE;
					break;

				case 0x0A:
					// Wait for a keypress and then store the value of the press in V[x]
					// This will set a variable, and the register will be modified in another
					// public function
					_waitingForInput = true;
					_inputReg = x;
					_PC += INST_SIZE;
					break;

				case 0x15:
					// Set the delay timer to the value in V[x]
					_DT = _V[x];
					_PC += INST_SIZE;
					break;

				case 0x18:
					// Set the sound timer to the value in V[x]
					_ST = _V[x];
					_PC += INST_SIZE;
					break;

				case 0x1E:
					// Add the value stored in _V[x] to I
					_I += _V[x];
					_PC += INST_SIZE;
					break;

				case 0x29:
					// Set I to the mem. addr. where the sprite for the character stored in V[x]
					_I = MEM_FONT_START + (_V[x] * MEM_FONT_HEIGHT);
					_PC += INST_SIZE;
					break;

				case 0x33:
					// Store the BCD value of V[x] into mem. addr. I, I + 1, and I + 2
					factor = 100;
					copy = _V[x];
					count;

					// Useful little for loop
					for (reg16 i = 0; i < 3; i++) {
						count = 0;

						while (copy > factor) {
							copy -= factor;
							count++;
						}

						_curMem->writeByte(_I + i, count);
						factor /= 10;
					}

					_PC += INST_SIZE;
					break;

				case 0x55:
					// Store the values in V[0] through V[x] into memory starting at addr. I
					for (reg16 i = 0; i <= x; i++)
						_curMem->writeByte(_I + i, _V[i]);

					// Also increment the I addr.
					_I += (x + 1);
					_PC += INST_SIZE;
					break;

				case 0x65:
					// Load the values in V[0] through V[x] from memory starting at addr. I
					for (reg16 i = 0; i <= x; i++)
						_V[i] = _curMem->readByte(_I + i);

					// Also increment the I addr.
					_I += (x + 1);
					_PC += INST_SIZE;
					break;

				default:
					return -1;
			}
			break;

		default:
			return -1;
	}
	
	return 0;
}


// Decrement the delay and sound timers
void CPU::decrementTimers() {
	if (waitingForInput())
		return;

	// Sound
	if (_ST > 0)
		_ST--;
	
	// Delay
	if (_DT > 0)
		_DT--;
}


// See if we can start going
bool CPU::readyToRun() {
	return ((_curMem != NULL) && (_curStack != NULL) && (_curDisplay != NULL));
}


// Use this function to set an IO value for a register
// Also turns of the `_waitingForInput` flag
//
// `key` should only be a value between 0x0 and 0xF (inclusive)
void CPU::getKeyPress(reg8 key) {
	// Safety check
	if (!waitingForInput())
		return;

	_V[_inputReg] = key;
	_waitingForInput = false;
}


/*== Mutators ==*/
// Set the memory device we will use
void CPU::setMemory(Memory *mem) {
	_curMem = mem;
}


// Set the stack device we will need to use
void CPU::setStack(Stack *stack) {
	_curStack = stack;
}


// Set a connection to the display device we need to modify
void CPU::setDisplay(Display *display) {
	_curDisplay = display;
}


// Mem. addr. in the I register
reg16 CPU::getI() {
	return _I;
}


// Mem. Addr. of next instrution to execute
reg16 CPU::getPC() {
	return _PC;
}


// Current value of the delay timer
reg8 CPU::getDT() {
	return _DT;
}


// Current value of the sound timer
reg8 CPU::getST() {
	return _ST;
}


// Gets a value from one of the V registers,  will return a 0 if out of range
reg8 CPU::getV(int id) {
	if ((id < 0) || (id > 0xF))
		return 0;
	
	return _V[id];
}


// Gets the current set clock speed
int CPU::getClockSpeed() {
	return _clockSpeed;
}


// Gets the instruction that we just executed
reg16 CPU::getCurInstruction() {
	return _curInst;
}


// Gets which register we are waiting to put input into
int CPU::getInputReg() {
	return _inputReg;
}


// Sees if we are waiting for a keypad press
bool CPU::waitingForInput() {
	return _waitingForInput;
}


// Fetches from memory the the next instruction to decode and execute.
// Returns -1 if we are not ready to run, else it will return 0
int CPU::_fetch() {
	if (!readyToRun())
		return -1;
	
	// Get the instruction from the mem. addr. store in the PC
	_curInst = _curMem->readByte(_PC) << 8;
	_curInst |= _curMem->readByte(_PC + 1);

	return 0;
}


