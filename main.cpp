#include "memory.h"
#include "stages.h"

int main() {
	std::vector<uint32_t> insns = { 0b00000000000000010010000010000011, // LW x1,x2,0
									0b00000000010100100000000110110011, // ADD x3,x4,x5
								    0b00000000100100111000001101100011, // BEQ x7,x9,3
									0b01000000101000110000001010110011, // SUB x5,x6,x10
									0b00000000000010000110000100000011,
									0b00000000010010101100001000100001};
	// global instruction and data memory
	Insn_data_memory instr_data_mem;
	instr_data_mem.set_insn(insns);
	instr_data_mem.print_memory();

	// global regfile
	uint32_t regs[32] = {0, 2, 0, 64, 10, 8, 14, 76, 100, 76, 24, 4, 2, 6, 2, 5, 53, 63, 27, 47, 64, 94, 27, 26, 39, 3, 3, 0, 84, 24, 85, 3};
	Regfile regfile(regs);
	regfile.print_regfile();

	// global signals
	uint32_t PC = 0;
	uint32_t PC_DISP = 0;
	uint8_t PC_R = 0;
	uint32_t BP_EX = 0;
	uint32_t BP_MEM = 0;

	Fetch_reg fetch_reg;
	Decode_reg decode_reg;
	Execute_reg execute_reg;
	Memory_reg memory_reg;

	int N = 10; // number of cycles
 
	for (int i = 0; i < N; i++) {
		printf("--------------------------------------------------------\n");
		std::cout << "PC = " << PC << std::endl;
		std::cout << "cycle = " << i << std::endl << std::endl;

		//Fetch stage
		//After this stage we get int time_
		Fetch_reg fetch_tmp = fetch(instr_data_mem, PC, PC_DISP, PC_R); 
		fetch_tmp.print_reg();

		//Decode stage
		Decode_reg decode_tmp = decode(fetch_reg, regfile);
		decode_tmp.print_reg();

		Execute_reg execute_tmp = execute(decode_reg, PC_DISP, PC_R, BP_EX, BP_MEM);
		execute_tmp.print_reg();

		Memory_reg memory_tmp = memory(execute_reg, instr_data_mem, BP_EX);
		memory_tmp.print_reg();

		write_back(memory_reg, regfile, BP_MEM);

		fetch_reg = fetch_tmp;
		decode_reg = decode_tmp;
		execute_reg = execute_tmp;
		memory_reg = memory_tmp;
	}

	return 0;
}
