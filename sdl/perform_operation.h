// File:         perform_operation.h
// Author:       Benjamin N. Summerton
// Description:  The "heart," of the CPU that takes in an OP-Code, then will perform that operation


#include "sys_internals.h"


// Probably the most important function of this software package.  It is what decodes an instruction
// then performs an action on the which systems
//
// Normally will return 0 on a success, but a -1 if something went bad
int performOperation(reg16 inst);


