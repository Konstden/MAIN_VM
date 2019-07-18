#include"HW.h"
#include<stdio.h>
#include<stdlib.h>

// sign-extending for immediate values
uint16_t sign_extend(uint16_t x, int bit_count) {
	if ((x >> (bit_count - 1)) & 1) // i.e. if sign bit
		x |= (0xFFFF << bit_count);
 
	return x; 
}

// update flages any time value written to a register 
void update_flags(uint16_t reg) {
	if (registers[reg] == 0)
		registers[R_COND] = FL_ZRO;
	else if (registers[reg] >> 15) // 1 in most_significant bit indicate negative
		registers[R_COND] = FL_NEG;
	else
		registers[R_COND] = FL_POS;

}

uint16_t mem_read(uint16_t addr) {
	return memory[addr];
}

void mem_write(uint16_t mem_addr, uint16_t value) {
	memory[mem_addr] = value;
}

int main(int argc, char * argv[])
{


	enum { PC_START = 0x3000 };
	registers[R_PC] = PC_START;

	int running = 1;
	while (running)
	{
		/*Fethcing instruction's opcode*/
		uint16_t instruction = mem_read(registers[R_PC]++);
		uint16_t op = instruction >> 12;

		switch (op) {
		case OP_ADD:
			uint16_t r0 = (instruction >> 9) & 0x7;
			uint16_t r1 = (instruction >> 6) & 0x7;
			uint16_t immediate_flag = (instruction >> 5) & 0x1;

			if (immediate_flag) {
				uint16_t immediate5bits = sign_extend(instruction & 0x001F, 5);
				registers[r0] = registers[r1] + immediate5bits;
			}
			else {
				uint16_t r2 = instruction & 0x7;
				registers[r0] = registers[r1] + registers[r2];
			}
			update_flags(r0);
		break;
		
		case OP_AND:
			uint16_t r0 = (instruction >> 9) & 0x7;
			uint16_t immediate_flag = (instruction >> 6) & 0x1;
			if (immediate_flag) {
				uint16_t immediate5bit = sign_extend(instruction & 0x1F, 5);
				registers[r0] = registers[r1] & immediate5bit;
			}
			else {
				uint16_t r2 = instruction & 0x7;
				registers[r0] = registers[r1] & registers[r2];
			}
			update_flags(r0);
		break;
		
		case OP_NOT:
			uint16_t r0 = (instruction >> 9) & 0x7;
			uint16_t r1 = (instruction >> 6) & 0x7;

			registers[r0] = ~registers[r1];
			update_flags(r0);
		break;

		case OP_BR:
			uint16_t PC_offset = sign_extend(instruction & 0x1FF, 9);
			uint16_t condition_flag = (instruction >> 9) & 0x7;
			if (condition_flag & registers[R_COND]) {
				registers[R_PC] += PC_offset;
			}
		break;
		case OP_JMP:
			uint16_t r1 = (instruction >> 6) & 0x7;
			registers[R_PC] = registers[r1];
		break;

		case OP_JSR:
			uint16_t r1 = (instruction >> 6) & 0x7;
			uint16_t long_PC_offset = sign_extend(instruction & 0x7FF, 11);
			uint16_t long_PC_flag = (instruction >> 11) & 0x1;
			
			registers[R_R7] = registers[R_PC];
			if (long_PC_flag)
				registers[R_PC] = long_PC_offset;
			else
				registers[R_PC] = registers[r1];

		break;

		case OP_LD:
			uint16_t r0 = (instruction >> 9) & 0x7;
			uint16_t PC_offset = sign_extend(instruction & 0x1FF, 9);

			registers[r0] = mem_read(registers[R_PC] + PC_offset);
			update_flags(r0);
		break;

		case OP_LDI:
			uint16_t r0 = (instruction >> 9) & 0x7;
			uint16_t PC_offset = sign_extend(instruction & 0x1FF, 9);
			
			registers[r0] = mem_read(registers[R_PC] + PC_offset);
			update_flags(r0);
		break;

		case OP_LDR:
			uint16_t r0 = (instruction >> 9) & 0x7;
			uint16_t r1 = (instruction >> 6) & 0x7;
			uint16_t merely_offset = sign_extend(instruction & 0x3F, 6);
			
			registers[r0] = mem_read(registers[r1] + merely_offset);
			update_flags(r0);
		break;

		case OP_LEA:
			uint16_t r0 = (instruction >> 9) & 0x7;
			uint16_t PC_offset = sign_extend(instruction & 0x1FF, 9);
			registers[r0] = mem_read(registers[R_PC] + PC_offset);
			update_flags(r0);
		break;

		case OP_ST:
			uint16_t r0 = (instruction >> 9) & 0x7;
			uint16_t PC_offset = sign_extend(instruction & 0x1FF, 9);

			mem_write(registers[R_PC] + PC_offset, registers[r0]);
		break;

		case OP_STI:
			uint16_t r0 = (instruction >> 9) & 0x7;
			uint16_t PC_offset = sign_extend(instruction & 0x1FF, 9);
			mem_write(mem_read(registers[R_PC] + PC_offset), registers[r0]);
		break;

		case OP_STR:
			uint16_t r0 = (instruction >> 6) & 0x7;
			uint16_t r1 = (instruction >> 9) & 0x7;
			uint16_t PC_offset = sign_extend(instruction & 0x3F, 6);
			
			mem_write(registers[r0] + PC_offset, registers[r1]);
		break;

		case OP_TRAP:
		case OP_RES:
		case OP_RTI:
		default:
			printf("Unspecified opcode: 0x%X", op);
		break;
		}
	}
}