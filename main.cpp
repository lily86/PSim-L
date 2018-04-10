#include "memory.h"
#include "stages.h"

int main() {
	std::vector<uint32_t> insns = { 0b00010010000010000011, // LW x1,x2,0
									0b011111100111000010000101000000000,
								    0b01000000100000001000101000011111,
									0b1010101010010000110000100000011,
									0b1010101010010000110000100000011,
									0b0000000010010101100001000100001};
	//global instruction and data memory
	Insn_data_memory instr_data_mem;
	instr_data_mem.set_instr(insns);
	instr_data_mem.print_memory();

	//global regfile
	uint32_t regs[32] = {0, 2, 13, 64, 10, 8, 14, 76, 100, 7, 24, 4, 2, 6, 2, 5, 53, 63, 27, 47, 64, 94, 27, 26, 39, 3, 3, 0, 84, 24, 85, 3};
	Regfile regfile(regs);
	regfile.print_regfile();

	uint32_t PC = 0;
	uint32_t PC_DISP = 0;
	uint8_t PC_R = 0;
	uint32_t BP_EX = 0;
	uint32_t BP_MEM = 0;
 
	for (int i = 0; i < insns.size(); i++) {
		printf("--------------------------------------------------------\n");
		std::cout << "PC = " << PC << std::endl << std::endl;
		//Fetch stage
		//After this stage we get int time_
		Fetch_reg fetch_reg = fetch(instr_data_mem, PC, PC_DISP, PC_R); 
		fetch_reg.print_reg();

		//Decode stage
		Decode_reg decode_reg = decode(fetch_reg, regfile);
		decode_reg.print_reg();

		Execute_reg execute_reg = execute(decode_reg, PC_DISP, PC_R);
		execute_reg.print_reg();

		Memory_reg memory_reg = memory(execute_reg, instr_data_mem, BP_EX);
		memory_reg.print_reg();

		write_back(memory_reg, regfile, BP_MEM);
	}

	return 0;
}
