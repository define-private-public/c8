// File:         perform_operation.cpp
// Author:       Benjamin N. Summerton
// Description:  Implementation for `perform_operation.h`


#include "perform_operation.h"
#include <cstdlib>


// Performs an operation for the system
//
// Will return 0 on success, -1 on bad things
int performOperation(reg16 inst) {
	// For decoding the instructoin
	int opc = (inst & 0xF000) >> 12;
	int nnn = inst & 0x0FFF;			// 12 bit mem. addr.
	int nn = inst & 0x00FF;				// 8 bit constant
	int n = inst & 0x000F;				// 4 bit constant
	int x = (inst & 0x0F00) >> 8;		// 4 bit reg. ID
	int y = (inst & 0x00F0) >> 4;		// 4 bit reg. ID

	// Other variables
	int copy, factor;
	reg8 count;


	/*== Perform the operations ==*/
	switch (opc) {
		case 0x0:
			switch (nn) {
				case 0xE0:
					// Cear the screen
					curDisplay.clearDisplay();
					PC += INST_SIZE;
					break;

				case 0xEE:
					// Return from a subroutine
					PC = stack.pop();
					PC += INST_SIZE;
					break;

				default:
					return -1;
			}
			break;

		case 0x1:
			// Jump to mem. addr. at `nnn`
			PC = nnn;
			break;

		case 0x2:
			// Run subroutine at location `nnn`
			stack.push(PC);
			PC = nnn;
			break;

		case 0x3:
			// Skip the next inst. if the value of V[X] is equal to `nn`
			if (V[x] == nn)
				PC += INST_SIZE;

			PC += INST_SIZE;
			break;

		case 0x4:
			// Skip the next inst. if the value of V[X] is not equal to `nn`
			if (V[x] != nn)
				PC += INST_SIZE;

			PC += INST_SIZE;
			break;

		case 0x5:
			// Skip the next inst. if the value of V[x] is equal to the value of V[y]
			if (V[x] == V[y])
				PC += INST_SIZE;

			PC += INST_SIZE;
			break;

		case 0x6:
			// Set the value of V[x] equal to `nn`
			V[x] = nn;
			PC += INST_SIZE;
			break;

		case 0x7:
			// Add the value `nn` to the value of V[x].
			V[x] += nn;		// NOTE: This may cause a wraparound
			PC += INST_SIZE;
			break;

		case 0x8:
			// TODO: Comeback later, arithmetic
			break;

		case 0x9:
			// Skip the next inst. if the value of V[x] is not equal to the value of V[y]
			if (V[x] != V[y])
				PC += INST_SIZE;

			PC += INST_SIZE;
			break;

		case 0xA:
			// Set I equal to the mem. addr. `nnn`
			I = nnn;
			PC += INST_SIZE;
			break;

		case 0xB:
			// Set the program counter to the address `nnn` plus the value of V[0]
			PC = nnn + V[0];
			break;

		case 0xC:
			// Generate a "random," number and put it into V[x]; AND mask it with `nn`
			V[x] = rand() & nn;
			PC += INST_SIZE;
			break;

		case 0xD:
			// Draw a sprite at location (V[x], V[y]) of 8 by `n` pixels big
			// Get the sprite data starting at mem location I
			
			// Get the sprite data
			unsigned char spriteData[n];
			for (int i = 0; i < n; i++)
				spriteData[i] = mem.readByte(I + i);

			// Draw the sprite and set the V[F] register if there was a collision
			V[0xF] = curDisplay.drawSprite(V[x], V[y], n, spriteData);
			PC += INST_SIZE;
			break;

		case 0xE:
			// TODO: Input, comeback
			break;

		case 0xF:
			// Just a bunch of random operations
			switch (nn) {	
				case 0x07:
					// Store the value in DT into V[x]
					V[x] = DT;
					PC += INST_SIZE;
					break;

				case 0x0A:
					// TODO: Input comback
					break;

				case 0x15:
					// Set the delay timer to the value in V[x]
					DT = V[x];
					PC += INST_SIZE;
					break;

				case 0x18:
					// Set the sound timer to the value in V[x]
					ST = V[x];
					PC += INST_SIZE;
					break;

				case 0x1E:
					// Add the value stored in V[x] to I
					I += V[x];
					PC += INST_SIZE;
					break;

				case 0x29:
					// Set I to the mem. addr. where the sprite for the character stored in V[x]
					I = MEM_FONT_START + (V[x] * MEM_FONT_HEIGHT);
					PC += INST_SIZE;
					break;

				case 0x33:
					// Store the BCD value of V[x] into mem. addr. I, I + 1, and I + 2
					factor = 100;
					copy = V[x];
					count;

					// Useful little for loop
					for (reg16 i = 0; i < 3; i++) {
						count = 0;

						while (copy > factor) {
							copy -= factor;
							count++;
						}

						mem.writeByte(I + i, count);
						factor /= 10;
					}

					PC += INST_SIZE;
					break;

				case 0x55:
					// Store the values in V[0] through V[x] into memory starting at addr. I
					for (reg16 i = 0; i <= x; i++)
						mem.writeByte(I + i, V[i]);

					// Also increment the I addr.
					I += (x + 1);
					PC += INST_SIZE;
					break;

				case 0x65:
					// Load the values in V[0] through V[x] from memory starting at addr. I
					for (reg16 i = 0; i <= x; i++)
						V[I] = mem.readByte(I + i);

					// Also increment the I addr.
					I += (x + 1);
					PC += INST_SIZE;
					break;
			}
			break;

		default:
			return -1;
	}
	
	return 0;
}
