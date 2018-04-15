#ifndef STAGES_H
#define STAGES_H

#include "memory.h"

//на вход - память инструкций и регистров
//сигналы
//регистр после стадии и результат АЛУ

Fetch_reg fetch(Insn_data_memory &mem, uint32_t &PC, uint32_t &PC_DISP, uint8_t &PC_R) {	
	Fetch_reg return_reg(mem.get_insn(PC)); // read instruction from memory

	if (PC_R) PC += PC_DISP / 4; // propagate PC
	else  PC += 1;
	
	return return_reg;
}

Decode_reg decode(Fetch_reg &reg, Regfile &regfile) {
	CU_signals CU = control_unit(reg.get_reg()); //create Control Unit

	uint8_t	rs1 = get_bits(reg.get_reg(), 15, 5); // get data
	uint8_t	rs2 = get_bits(reg.get_reg(), 20, 5);
	uint8_t	rd = get_bits(reg.get_reg(), 7, 5);
	uint16_t imm1 = get_bits(reg.get_reg(), 20, 11);
	uint32_t imm2 = get_bits(reg.get_reg(), 31, 1);

	uint32_t sgn = sign_extend(imm2); // sign extend immediate
	uint32_t rs1_val = regfile.get_register(rs1); // read data from regfile
	uint32_t rs2_val = regfile.get_register(rs2);

	Decode_reg return_reg(CU, rs1, rs2, rs1_val, rs2_val, rd, imm1, sgn);
	return return_reg;
}

Execute_reg execute(Decode_reg &reg, uint32_t &PC_DISP, uint8_t &PC_R, uint32_t &BP_EX, uint32_t &BP_MEM) {
	CU_signals CU = reg.get_CU_reg(); // get signals
	uint8_t AluOp = CU.AluOp;
	uint8_t mux_ex_1 = CU.mux_ex1;
	uint8_t conditional = CU.conditional;

	uint8_t rs1_val = reg.get_rs1_val(); // get data
	uint8_t rs2_val = reg.get_rs2_val();	
	uint16_t imm1 = reg.get_imm1();
	uint32_t imm2 = reg.get_imm2();
	uint32_t rd = reg.get_rd();

	// mux1, mux2

	uint32_t imm_input = imm2 >> 31 | imm1; // imm
	struct Imm imm;
	imm.imm_I = get_bits(imm_input, 20, 12);
	imm.imm_S = get_bits(imm_input, 25, 7) >> 25 | get_bits(imm_input, 7, 5);
	imm.imm_B = get_bits(imm_input, 31, 1) >> 31 | get_bits(imm_input, 7, 1) >> 7 |
				get_bits(imm_input, 25, 6) >> 25 | get_bits(imm_input, 8, 4) >> 8;
	/*imm.imm_U = get_bits(imm_input, 20, 12);
	imm.imm_J = get_bits(imm_input, 31, 1) >> 31 | get_bits(imm_input, 12, 8) >> 12 |
				get_bits(imm_input, 20, 1) >> 20 | get_bits(imm_input, 21, 10);*/

	uint32_t mux5_res = multiplexor5(rs2_val, imm, rd, mux_ex_1); // mux5
	PC_DISP = mux5_res;

	uint32_t ALUresult = alu(AluOp, rs1_val, mux5_res); // ALU

	if (conditional == 1 && rs1_val == rs2_val) {// comparator - conditional
		PC_R = 1;
		std::cout << "it is a branch" << std::endl;
	}

	Execute_reg return_reg(CU, rs2_val, ALUresult, rd);
	return return_reg;
}

Memory_reg memory(Execute_reg &reg, Insn_data_memory &mem, uint32_t &BP_EX) {	
	CU_signals CU = reg.get_CU_reg(); // get signals
	uint8_t mux_mem1 = CU.mux_mem1;
	uint8_t MEM_WE = CU.MEM_WE;

	uint32_t rs2_val = reg.get_rs2_val(); // get data
	uint32_t get_alu_result = reg.get_ALUresult();

	uint32_t rd = mem.get_register(get_alu_result); // read from memory anyway
	if (MEM_WE)
	   	mem.set_register(rs2_val, get_alu_result); // write to memory only on signal
	
	uint32_t result_d;
	if (!mux_mem1) // multiplexor
		result_d = rd;
	else 
		result_d = get_alu_result;

	BP_EX = reg.get_ALUresult(); // set signal
	
	Memory_reg return_reg(CU, result_d, reg.get_rd());
	return return_reg;	
}

void write_back(Memory_reg &reg, Regfile &regfile, uint32_t &BP_MEM) {
	CU_signals CU = reg.get_CU_reg(); // get signals
	uint8_t WB_WE_signal = CU.WB_WE;

	uint32_t WB_D = reg.get_mux_res(); // set signals
	BP_MEM = reg.get_mux_res();
	uint8_t WB_A = reg.get_rd();

	if (WB_WE_signal) {
		regfile.set_register(WB_A, WB_D); // write to regfile only on signal
		std::cout << "[reg]: " << std::bitset<8>(WB_A) << " -> " << std::bitset<32>(regfile.get_register(WB_A)) << std::endl; 
	}
}

#endif