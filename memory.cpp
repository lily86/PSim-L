#include "memory.h"

uint32_t multiplexor4(uint8_t rs1_addr, uint16_t imm1, uint32_t imm2, uint8_t RD, uint8_t mux_ex_signal) {
	switch (mux_ex_signal) {
		case 0: return rs1_addr;
		case 1: return imm1;
		case 2: return imm2;
		case 3: return RD;
	}
}

int8_t multiplexor(int8_t lorD) {
	if (!lorD) return 0;
	return 1;
}

uint32_t get_bits(uint32_t insn, unsigned int pos, unsigned int n) {
    return ((insn & (~(~0 << n) << pos)) >> pos);
}   

int32_t sign_extend(uint32_t insn) {
	std::cout << "instrusction " << std::bitset<32>(insn) << std::endl;
	uint16_t number_register = insn>>20;
	//std::cout <<std::bitset<16>(number_register) <<std::endl;
	uint32_t tmp = 0;	
	uint8_t get_higher_bit = number_register >> 11;
	//std::cout<<std::bitset<8>(get_higher_bit)<<std::endl;
	if (get_higher_bit) tmp = number_register | 0xFFFFF000;
	else tmp = number_register;

	return tmp;
}

int32_t alu(int ALUOp, uint32_t a, uint32_t b) {
	switch(ALUOp) {
		case 0: return a + b;
		case 1: return a - b;
		case 2: return a/b;
		case 3: return a-b;
	}
}

void CU_signals::set_sginals(uint8_t WB_WE_val, uint8_t MEM_WE_val, uint8_t mux_ex1_val,
		uint8_t AluOp_val, uint8_t mux_mem1_val, uint8_t conditional_val) {
	WB_WE = WB_WE_val;
	MEM_WE = MEM_WE_val;
	mux_ex1 = mux_ex1_val;
	AluOp = AluOp_val;
	mux_mem1 = mux_mem1_val;
	conditional = conditional_val;
}

CU_signals control_unit(uint32_t insn) { //uint8_t oppcode, uint8_t funct7)
	uint8_t opcode = get_bits(insn, 0, 7);
	uint8_t funct7 = get_bits(insn, 25, 7);

	CU_signals get_signals;

	//R-type
	if (opcode == 0b0110011) {
		//if funct7 = 01 - substitution , AluOp = 1
		switch (funct7) {
			case 0b0100000 : get_signals.set_sginals(1,0,0,1,1,0);
			case 0b0000000 : get_signals.set_sginals(1,0,0,0,1,0);
		}
	}
	
	if(opcode == 0b0000011) {
		switch (funct7) {
			case 0b0100000 : get_signals.set_sginals(1,0,1,0,0,0);
		}
	}
	
	if(opcode == 0b0100011) {
		switch (funct7) {
			case 0b0100000 : get_signals.set_sginals(0,1,2,0,0,0);
		}
	}
	
	if(opcode == 0b1100011) {
		//if funct7 = 01 - substitution , AluOp = 1
		switch (funct7) {
			case 0b0100000 : get_signals.set_sginals(0,0,0,0,0,1);
		}
	}

	return get_signals;
}
//---------------------------------------------------------------------------------------------------------------

void Fetch_reg::print_reg() {
	std::cout << "fetch_reg: " << std::bitset<32>(reg) << std::endl;	
}

void Decode_reg::set_reg(CU_signals CU_reg_val, uint8_t RS1_v, uint8_t RS2_v, uint32_t RS1_vv, uint32_t RS2_vv, uint8_t RD_v, uint16_t imm1_val, uint32_t imm2_val) {
	CU_reg = CU_reg_val;
	RS1 = RS1_v;
	RS2 = RS2_v;
	RS1_val = RS1_vv;
	RS2_val = RS2_vv;
	imm1 = imm1_val;
	imm2 = imm2_val;
	RD = RD_v;
}

void Decode_reg::print_reg() {
/*	std::cout << "decode_reg: ";
	std::cout << std::bitset<32> (reg) << std::endl;	
*/}

Execute_reg::Execute_reg(CU_signals CU_val, uint8_t rs1_v, uint32_t rs1_vv, uint32_t ALUresult_val, uint8_t RD_val) {
	CU_reg = CU_val;
	rs1 = rs1_v;
	rs1_val = rs1_vv;
	ALUresult = ALUresult_val;
	RD = RD_val;
}

void Execute_reg::print_reg() {
	//std::cout << "execute_reg: " << std::bitset<32>(reg) << std::endl;	
}

Memory_reg::Memory_reg(CU_signals CU_reg_val,uint32_t mux_result_val,uint8_t RD_val) {
	CU_reg = CU_reg_val;
	mux_result = mux_result_val;
	RD = RD_val;
}

void Memory_reg::print_reg() {
	//std::cout << "memory_reg: " << std::bitset<32>(reg) << std::endl;	
}

//---------------------------------------------------------------------------------------------------------------

Insn_data_memory::Insn_data_memory(uint32_t a1, uint32_t a2) {
	m_regs[0] = a1;
	m_regs[1] = a2;
}

void Insn_data_memory::print_instruction(uint32_t PC) {
	std::cout << "oppcode :" << std::bitset<7>(insn[PC]&0b1111111) << std::endl;
	std::cout << "rd :" << std::bitset<5>((insn[PC]>>7)&0b11111) << std::endl;	
	std::cout << "rs1 :" << std::bitset<5>((insn[PC]>>15)&0b11111) << std::endl;
	std::cout << "rs2 :" << std::bitset<5>((insn[PC]>>20)&0b11111) << std::endl;
}

Regfile::Regfile(uint32_t a1, uint32_t a2, uint32_t a3) {
	m_regs[0] = a1;
	m_regs[1] = a2;
	m_regs[20] = a3;
}

void Regfile::print_regfile() {
	for(int i = 0; i < 5; ++i)
		std::cout << m_regs[i] << std::endl;
}