// File:         perform_operation.cpp
// Author:       Benjamin N. Summerton
// Description:  Implementation for `perform_operation.h`


#include "perform_operation.h"


// Performs an operation for the system
int performOperation(reg16 inst) {
	// For decoding the instructoin
	int opc = (inst & 0xF000) >> 12;
	int nnn = inst & 0x0FFF;			// 12 bit mem. addr.
	int nn = inst & 0x00FF;				// 8 bit constant
	int n = inst & 0x000F;				// 4 bit constant
	int x = (inst & 0x0F00) >> 8;		// 4 bit reg. ID
	int y = (inst & 0x00F0) >> 4;		// 4 bit reg. ID
	
	return 0;
}
