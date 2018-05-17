#ifndef STAGES_H
#define STAGES_H

#include "memory.h"

//на вход - память инструкций и регистров
//сигналы
//регистр после стадии и результат АЛУ

Fetch_reg fetch(Insn_data_memory &mem, uint32_t &PC, uint32_t &PC_DISP, uint8_t &PC_R, uint32_t &local_PC, uint8_t branch) {	
	Fetch_reg return_reg(mem.get_insn(PC), PC); // read instruction from memory

	//if (PC_R) PC += PC_DISP / 4; // propagate PC
	std::cout<<"PC_R is: "<<std::bitset<8>(PC_R)<<std::endl;
	if (PC_R)
	{
		std::cout<<"PC_R: "<<std::bitset<8>(PC_R)<<std::endl;
		PC = local_PC + static_cast<int32_t>(PC_DISP);
		std::cout<<"local_PC : "<<local_PC<<std::endl;
		std::cout<<"PC_DISP : "<<static_cast<int32_t>(PC_DISP)<<std::endl;			
		std::cout<<"PC: "<<std::bitset<32>(PC)<<std::endl;	
		branch = 1;
	}

	else  
	{
		PC += 1;
		branch = 0;
	}
	return return_reg;
}

Decode_reg decode(Fetch_reg &reg, Regfile &regfile) {
	CU_signals CU = control_unit(reg.get_reg()); //create Control Unit

	uint32_t local_PC = reg.get_local_PC();
	std::cout<<"reg.get_reg: "<<std::bitset<32>(reg.get_reg())<<std::endl;
	uint8_t	rs1 = get_bits(reg.get_reg(), 15, 5); // get data
	uint8_t	rs2 = get_bits(reg.get_reg(), 20, 5);
	uint8_t	rd = get_bits(reg.get_reg(), 7, 5);
	uint16_t imm1 = get_bits(reg.get_reg(), 20, 11);
	uint32_t imm2 = get_bits(reg.get_reg(), 31, 1);
	uint8_t funct3 = get_bits(reg.get_reg(), 12, 3);

	uint32_t sgn = sign_extend(reg.get_reg()); // sign extend immediate
	uint32_t rs1_val = regfile.get_register(rs1); // read data from regfile
	uint32_t rs2_val = regfile.get_register(rs2);

	//take here not imm2 , but sign_extended(imm2)
	Decode_reg return_reg(CU, rs1, rs2, rs1_val, rs2_val, rd, imm1, sgn, local_PC, funct3);
	return return_reg;
}

Execute_reg execute(Decode_reg &reg, uint32_t &PC_DISP, uint8_t &PC_R, uint32_t &BP_EX, uint32_t &BP_MEM, uint8_t branch) {
	CU_signals CU = reg.get_CU_reg(); // get signals
	
	uint8_t AluOp = CU.AluOp;
	uint8_t mux_ex = CU.mux_ex;
	uint8_t conditional = CU.conditional;
	uint8_t mux_pc = CU.mux_pc;
	uint8_t check_J = CU.check_J;

	uint32_t rs1_val = reg.get_rs1_val(); // get data // !!!!!!!!!!!!!!! 32
	uint32_t rs2_val = reg.get_rs2_val(); // !!!!!!!!!!!!!!!!!!!!!!!!!!! 32
	uint16_t imm1 = reg.get_imm1(); //from 20 to 30 bit
	uint32_t imm2 = reg.get_imm2(); // 31 bit is here sign extended!!( uint32_t )
	uint8_t rd = reg.get_rd(); // from 7 to 11 bit

	uint8_t rs1 = reg.get_rs1();
	uint8_t funct3 = reg.get_funct3();
	uint32_t local_PC = reg.get_local_PC();

	std::cout<<"imm1 is: "<<std::bitset<32>(imm1)<<std::endl;
	std::cout<<"imm2 is: "<<std::bitset<32>(imm2)<<std::endl;

	struct Imm imm;
	imm.imm_I = imm1 | imm2;
	imm.imm_S = imm2 | ((get_bits(imm1,5,6) << 5 ) | rd);
	imm.imm_B = imm2 | ((get_bits(rd,1,4) | get_bits(imm1,5,5) << 4 | get_bits(imm1,10,1) << 9 | get_bits(imm2,31,1) << 10 ));
	imm.imm_U = imm1 << 8 | rs1 << 3 | funct3;
	imm.imm_J = funct3 | rs1 << 3 | get_bits(imm1,10,1) << 8 | get_bits(imm1,0,9) << 9 | get_bits(imm2,31,1) << 19;
	std::cout<<"immidiate B is: "<<std::bitset<32>(imm.imm_B)<<std::endl;

	// extra multiplexor and signal for choosing between rs1_val or PC !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	if (mux_pc)
		rs1 = local_PC; 

	uint32_t mux5_res = multiplexor5(rs2_val, imm, rd, mux_ex); // mux5
	PC_DISP = mux5_res;
	
	std::cout<<"PC_DISP: "<<std::bitset<32>(PC_DISP)<<std::endl;
	//PC_DISP just return imm.imm_B
	uint32_t ALUresult = alu(AluOp, rs1_val, mux5_res); // ALU

	if (conditional == 1 && rs1_val == rs2_val) { // comparator - conditional
		PC_R = 1;
		branch = 1;
		std::cout << "it is a branch" << std::endl;
	} else if (check_J == 1) {
		PC_R = 1;
		branch = 1;
		std::cout << "it is a JAL" << std::endl;
	}
	else PC_R = 0;

	Execute_reg return_reg(CU, rs2_val, ALUresult, rd);
	return return_reg;
}

Memory_reg memory(Execute_reg &reg, Insn_data_memory &mem, uint32_t &BP_EX, uint8_t branch) {	
	CU_signals CU = reg.get_CU_reg(); // get signals
	uint8_t mux_mem = CU.mux_mem;
	uint8_t MEM_WE = CU.MEM_WE;

	uint32_t rs2_val = reg.get_rs2_val(); // get data
	uint32_t get_alu_result = reg.get_ALUresult();

	uint32_t rd = mem.get_register(get_alu_result); // read from memory anyway
	if (MEM_WE & (!branch) )
	   	mem.set_register(rs2_val, get_alu_result); // write to memory only on signal
	
	uint32_t result_d;
	if (!mux_mem) // multiplexor
		result_d = rd;
	else 
		result_d = get_alu_result;

	BP_EX = reg.get_ALUresult(); // set signal
	
	Memory_reg return_reg(CU, result_d, reg.get_rd());
	return return_reg;	
}

void write_back(Memory_reg &reg, Regfile &regfile, uint32_t &BP_MEM, uint8_t &branch) {
	CU_signals CU = reg.get_CU_reg(); // get signals
	uint8_t WB_WE_signal = CU.WB_WE;
	uint8_t STOP_signal = CU.stop;

	if (STOP_signal == 1) {
		std::cout << "STOP PIPELINE!" << std::endl << std::endl;
		exit(-1);
	}

	uint32_t WB_D = reg.get_mux_res(); // set signals
	BP_MEM = reg.get_mux_res();
	uint8_t WB_A = reg.get_rd();

	if (WB_WE_signal & (!branch)){
		regfile.set_register(WB_A, WB_D); // write to regfile only on signal
		std::cout << "[reg]: " << std::bitset<8>(WB_A) << " -> " << std::bitset<32>(regfile.get_register(WB_A)) << std::endl; 
	} 
}

#endif
