#ifndef STAGES_H
#define STAGES_H

#include "memory.h"

//на вход - память инструкций и регистров
//сигналы
//регистр после стадии и результат АЛУ

Fetch_reg fetch(Insn_data_memory &mem, uint32_t &PC, uint32_t &PC_DISP, uint8_t &PC_R) {	
	Fetch_reg return_reg;
	if (PC_R) PC = PC + PC_DISP / 4;
	else PC = PC + 1;

	return_reg.set_reg(mem.get_instruction(PC));
	return return_reg;
}

Decode_reg decode(Fetch_reg &reg, Regfile &obj)  {
	//create Control Unit
	struct CU_signals CU;
	//initialize control unit depend from instruction
	CU = control_unit(reg.get_reg());
	Decode_reg return_reg;

	uint8_t	rs1 = get_bits(reg.get_reg(), 15, 5);
	uint8_t	rs2 = get_bits(reg.get_reg(), 20, 5);
	uint8_t	rd = get_bits(reg.get_reg(), 7, 5);
	uint16_t imm1 = get_bits(reg.get_reg(), 20, 11);
	uint32_t imm2 = get_bits(reg.get_reg(), 31, 1);

	uint32_t sgn = sign_extend(imm2);
	uint32_t rs1_val = obj.get_register(rs1);
	uint32_t rs2_val = obj.get_register(rs2);

	return_reg.set_reg(CU, rs1, rs2, rs1_val, rs2_val, rd, imm1, sgn);
	return return_reg;
}

Execute_reg execute(Decode_reg &reg, uint32_t &PC_DISP, uint8_t &PC_R) {
	Execute_reg return_reg;

	CU_signals get_CU = reg.get_CU_reg();

	uint8_t AluOp_sign = get_CU.AluOp;
	uint8_t mux_ex_sign = get_CU.mux_ex1;
	uint8_t conditional_sign = get_CU.conditional;

	uint8_t rs1_address = reg.RS1_reg();
	uint8_t rs1_value = reg.RS1_val_reg();
	uint8_t rs2_value = reg.RS2_val_reg();	
	uint16_t imm1_value = reg.Imm1_reg();
	uint32_t imm2_value = reg.Imm2_reg();
	uint32_t RD_reg = reg.RD_reg();

/*	if (conditional && ) { 
		PC_R = ;
		PC_DISP = ;
	}*/

	if (0) {}

	//????????	

	//????????

	return return_reg;
}

Memory_reg memory(Execute_reg &reg, Insn_data_memory &mem) {	
	CU_signals get_CU = reg.get_CU_reg();
	uint32_t get_alu_result = reg.ALUresult_reg();
	uint32_t RS1_val = reg.rs1_val_reg();
	uint8_t mux_mem1 = get_CU.mux_mem1;

	uint8_t MEM_WE = get_CU.MEM_WE;
	uint32_t RD = mem.get_register(get_alu_result);
	if (MEM_WE)
	   	mem.set_register(RS1_val, get_alu_result);	
	
	uint32_t RESULT_D;	
	if (!mux_mem1) RESULT_D = RD;
	else RESULT_D = get_alu_result;
	
	Memory_reg return_reg(get_CU, RESULT_D, reg.RD_reg());
	return return_reg;	
}

void write_back(Memory_reg &reg, Regfile &regfile) {
	CU_signals get_CU = reg.get_CU_reg();
	uint8_t WB_WE_signal = get_CU.WB_WE;
	uint32_t WB_D = reg.mux_res();
	uint8_t WB_A = reg.RD_reg();

	if (WB_WE_signal) regfile.set_register(WB_A, WB_D);
}

#endif