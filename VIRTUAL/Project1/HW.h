#include<stdint.h>
 
	uint16_t memory[UINT16_MAX]; // memory = 2^16 * 2 bytes

	// registers 10 8 - GPR-s, 1- PC, 1- COND(for inf about previous calculations) 
	enum {
		R_R0 = 0,
		R_R1,
		R_R2,
		R_R3,
		R_R4,
		R_R5,
		R_R6,
		R_R7,
		R_PC,
		R_COND,
		R_COUNT
	};

	// opcodes 
	enum {
		OP_BR = 0,		// conditional branch
		OP_ADD,			// add
		OP_LD,			// load
		OP_ST,			// store
		OP_JSR,			// Jump reg
		OP_AND,			// logical bitwise AND
		OP_LDR,			// load reg (base reg addressing mode)
		OP_STR,			// store reg
		OP_RTI,			// unused 
		OP_NOT,			// bitwise not
		OP_LDI,			// load indirect ( register point to memory location M[REG])
		OP_STI,			// store indirect
		OP_JMP,			// uncoditional branch ( jump )
		OP_RES,			// Reserved (unused )
		OP_LEA,			// load effective address
		OP_TRAP,		// execute TRAP 
	};

	enum {
		FL_POS = 1 << 0, // P
		FL_ZRO = 1 << 1, // Z
		FL_NEG = 1 << 2	 // N
	};
	uint16_t registers[R_COUNT];

	uint16_t sign_extend(uint16_t x, int bit_count);
 
 
