#ifndef MEMORY_H
#define MEMORY_H

#include <bitset>
#include <iostream>
#include <fstream>
#include <vector>

struct Imm {
	uint32_t imm_I;
	uint32_t imm_S;
	uint32_t imm_B;
	uint32_t imm_U;
	uint32_t imm_J;
};

uint32_t multiplexor5(uint32_t rs2_val, Imm imm, uint32_t rd, uint8_t mux_ex); // зачем RD ???
int8_t multiplexor(int8_t lorD);
uint32_t get_bits(uint32_t insn, unsigned int pos, unsigned int n);
int32_t sign_extend(uint32_t insn);
int32_t alu(int ALUOp, uint32_t a, uint32_t b);

struct CU_signals {
	uint8_t WB_WE = 0;
	uint8_t MEM_WE = 0;
	uint8_t mux_ex = 0;
	uint8_t AluOp = 0;
	uint8_t mux_mem = 0;
	uint8_t conditional = 0;
	uint8_t stop = 0;

	uint8_t mux_pc = 0;

	CU_signals() {}

	void set_signals(uint8_t WB_WE_, uint8_t MEM_WE_, uint8_t mux_ex_,
					 uint8_t AluOp_, uint8_t mux_mem_, uint8_t conditional_, uint8_t stop_, uint8_t mux_pc_);

	void print_CU_signals();
};

struct CU_signals control_unit(uint32_t insn);
//-------------------------------------------------------------------------------------------------------
// TEMPORARY REGISTERS:

class Fetch_reg {
	uint32_t reg;
	uint32_t local_PC;

public:
	Fetch_reg() : reg(0), local_PC(0) {}
	Fetch_reg(uint32_t reg_, uint32_t local_PC_) { reg = reg_, local_PC = local_PC_; }
	
	uint32_t get_reg() { return reg; }
	uint32_t get_local_PC() { return local_PC; }

	void print_reg();
	void print_local_PC();
};

class Decode_reg {
	CU_signals CU_reg;
	uint8_t rs1;
	uint8_t rs2;
	uint32_t rs1_val;
	uint32_t rs2_val;
	uint16_t imm1; //from 20 to 30
	uint32_t imm2; //31
	uint8_t rd;

	uint32_t local_PC;
	uint8_t funct3;

public:
	Decode_reg() : rs1(0), rs2(0), rs1_val(0), rs2_val(0), imm1(0), imm2(0), rd(0), local_PC(0), funct3(0) {}
	Decode_reg(CU_signals CU_reg_, uint8_t rs1_, uint8_t rs2_, uint32_t rs1_val_, 
			   uint32_t rs2_val_, uint8_t rd_, uint16_t imm1_, uint32_t imm2_, uint32_t local_PC_, uint8_t funct3);

	CU_signals get_CU_reg() { return CU_reg; }	
	uint8_t get_rs1() { return rs1; }	
	uint8_t get_rs2() { return rs2; }
	uint32_t get_rs1_val() { return rs1_val; }	
	uint32_t get_rs2_val() { return rs2_val; }
	uint8_t get_rd() { return rd; }
	uint16_t get_imm1() { return imm1; }
	uint32_t get_imm2() { return imm2; }
	uint32_t get_local_PC() { return local_PC; }
	uint8_t get_funct3() { return funct3; }

	void print_reg();
};

class Execute_reg {
	CU_signals CU_reg;
	uint32_t rs2_val;
	uint32_t ALUresult;
	uint8_t rd;

public:
	Execute_reg() : rs2_val(0), ALUresult(0), rd(0) {}
	Execute_reg(CU_signals CU_, uint32_t rs2_, uint32_t ALUresult_, uint8_t rd_);

	CU_signals get_CU_reg() { return CU_reg; }
	uint32_t get_rs2_val() { return rs2_val; }
	uint32_t get_ALUresult() { return ALUresult; }
	uint8_t get_rd() { return rd; }

	void print_reg();
};

class Memory_reg {
	CU_signals CU_reg;
	uint32_t mux_result;
	uint8_t rd;

public:
	Memory_reg() : mux_result(0), rd(0) {}
	Memory_reg(CU_signals CU_reg, uint32_t mux_result, uint8_t rd);

	CU_signals get_CU_reg() { return CU_reg; }
	uint32_t get_mux_res() { return mux_result; }
	uint8_t get_rd() { return rd; }

	void print_reg();
};
//-------------------------------------------------------------------------------------------------------
// MEMORY:

//класс instn_Memory - хранит массив из 32 битных инструкций
class Insn_data_memory {
	std::vector<uint32_t> insn;

public:
	Insn_data_memory() {}

	void set_insn(std::vector<uint32_t> insns) { insn = insns; }
	uint32_t get_insn(int PC) { return insn[PC]; }
	void set_register(uint32_t reg, uint32_t A) { insn[A] = reg; }
	uint32_t get_register(uint32_t reg) { return insn[reg]; }

	void print_memory();
};

class Regfile {
	uint32_t m_regs[32];

public:
	Regfile() {}
  	Regfile(uint32_t regs[32]);
	
	uint32_t get_register(uint32_t number_register) { return m_regs[number_register]; }
	void set_register(uint8_t number_register, uint32_t word) { m_regs[number_register] = word; }

	void print_regfile();
};

#endif
