#ifndef MEMORY_H
#define MEMORY_H

#include <iostream>
#include <vector>
#include <bitset>

uint32_t multiplexor4(uint8_t rs1_addr, uint16_t imm1, uint32_t imm2, uint8_t RD, uint8_t mux_ex_signal);
int8_t multiplexor(int8_t lorD);
uint32_t get_bits(uint32_t insn, unsigned int pos, unsigned int n);
int32_t sign_extend(uint32_t insn);
int32_t alu(int ALUOp, uint32_t a, uint32_t b);

struct CU_signals {
	uint8_t WB_WE = 0;
	uint8_t MEM_WE = 0;
	uint8_t mux_ex1 = 0;
	uint8_t AluOp = 0;
	uint8_t mux_mem1 = 0;
	uint8_t conditional = 0;

	CU_signals() {}

	void set_sginals(uint8_t WB_WE_val, uint8_t MEM_WE_val, uint8_t mux_ex1_val,
		uint8_t AluOp_val, uint8_t mux_mem1_val, uint8_t conditional_val);

	void print_CU_signals();
};

struct CU_signals control_unit(uint32_t insn);

//-------------------------------------------------------------------------------------------------------
// TEMPORARY REGISTERS:

class Fetch_reg {
	uint32_t reg;

public:
	void set_reg(uint32_t val) { reg = val; }
	uint32_t get_reg() { return reg; }

	void print_reg();
};

class Decode_reg {
	CU_signals CU_reg;
	uint8_t RS1;
	uint8_t RS2;
	uint32_t RS1_val;
	uint32_t RS2_val;
	uint16_t imm1; //from 20 to 30
	uint32_t imm2; //31
	uint8_t RD;

public:
	Decode_reg(CU_signals CU_reg_val, uint8_t RS1_v, uint8_t RS2_v, uint32_t RS1_vv, uint32_t RS2_vv, uint8_t RD_v, uint16_t imm1_val, uint32_t imm2_val);

	CU_signals get_CU_reg() { return CU_reg; }	
	uint8_t RS1_reg() { return RS1; }	
	uint8_t RS2_reg() { return RS2; }
	uint32_t RS1_val_reg() { return RS1_val; }	
	uint32_t RS2_val_reg() { return RS2_val; }
	uint8_t RD_reg() { return RD; }
	uint16_t Imm1_reg() { return imm1; }
	uint32_t Imm2_reg() { return imm2; }

	void print_reg();
};

class Execute_reg {
	CU_signals CU_reg;
	uint8_t RS1;
	uint32_t RS1_val;
	uint32_t ALUresult;
	uint8_t RD;

public:
	Execute_reg() {}
	Execute_reg(CU_signals CU_val, uint8_t RS1_v, uint32_t RS1_vv, uint32_t ALUresult_val, uint8_t RD_val);

	CU_signals get_CU_reg() { return CU_reg; }
	uint8_t rs1_reg() { return RS1;	}
	uint32_t rs1_val_reg() { return RS1_val; }
	uint32_t ALUresult_reg() { return ALUresult; }
	uint8_t RD_reg() { return RD; }

	void print_reg();
};

class Memory_reg {
	CU_signals CU_reg;
	uint32_t mux_result;
	uint8_t RD;

public:
	Memory_reg(CU_signals CU_reg_val, uint32_t mux_result_val, uint8_t RD_val);

	CU_signals get_CU_reg() { return CU_reg; }
	uint32_t mux_res() { return mux_result; }
	uint8_t RD_reg() { return RD; }

	void print_reg();
};
//-------------------------------------------------------------------------------------------------------
// MEMORY:

//класс instn_Memory - хранит массив из 32 битных инструкций
class Insn_data_memory {
	std::vector<uint32_t> insn;
	uint32_t m_regs[1000];

public:
	Insn_data_memory() {}

	void set_instr(std::vector<uint32_t> insns) { insn = insns; }
	uint32_t get_instruction(int PC) { return insn[PC]; }
	void set_register(uint32_t reg, uint32_t A) { m_regs[A] = reg; }
	uint32_t get_register(uint32_t reg) { return m_regs[reg]; }

	void print_memory();
};

class Regfile {
	uint32_t m_regs[32];

public:
	Regfile() {}
  	Regfile(uint32_t regs[32]);
	
	int32_t get_register(uint32_t number_register) { return m_regs[number_register]; }
	void set_register(uint8_t number_register, uint32_t word) { m_regs[number_register] = word; }

	void print_regfile();
};

#endif