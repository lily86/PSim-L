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
	if (get_bits(insn, 31, 1)) return 0x1FFFFF;
	return 0;

/*	std::cout << "instrusction " << std::bitset<32>(insn) << std::endl;
	uint16_t number_register = insn>>20;
	//std::cout <<std::bitset<16>(number_register) <<std::endl;
	uint32_t tmp = 0;	
	uint8_t get_higher_bit = number_register >> 11;
	//std::cout<<std::bitset<8>(get_higher_bit)<<std::endl;
	if (get_higher_bit) tmp = number_register | 0xFFFFF000;
	else tmp = number_register;
	return tmp; */
}

// concrete?
int32_t alu(int ALUOp, uint32_t a, uint32_t b) {
	switch(ALUOp) {
		case 0: return a + b;
		case 1: return a - b;
		case 2: return a / b;
		case 3: return a - b;
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

void CU_signals::print_CU_signals() {
	std::cout << "CU_signals:" << std::endl;
	std::cout << "  WB_WE:       0b" << std::bitset<8>(WB_WE) << std::endl;
	std::cout << "  MEM_WE:      0b" << std::bitset<8>(MEM_WE) << std::endl;
	std::cout << "  MEM_WE:      0b" << std::bitset<8>(MEM_WE) << std::endl;
	std::cout << "  AluOp:       0b" << std::bitset<8>(AluOp) << std::endl;
	std::cout << "  mux_mem1:    0b" << std::bitset<8>(mux_mem1) << std::endl;
	std::cout << "  conditional: 0b" << std::bitset<8>(conditional) << std::endl;
}

CU_signals control_unit(uint32_t insn) {
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
	std::cout << "FETCH_REG:     0b" << std::bitset<32>(reg) << std::endl << std::endl;	
}

Decode_reg::Decode_reg(CU_signals CU_reg_val, uint8_t RS1_v, uint8_t RS2_v, uint32_t RS1_vv, uint32_t RS2_vv, uint8_t RD_v, uint16_t imm1_val, uint32_t imm2_val) {
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
	std::cout << "DECODE_REG:" << std::endl;
	std::cout << "RS1:           0b" << std::bitset<8>(RS1) << std::endl;
	std::cout << "RS2:           0b" << std::bitset<8>(RS2) << std::endl;
	std::cout << "RS1_val:       0b" << std::bitset<32>(RS1_val) << std::endl;
	std::cout << "RS2_val:       0b" << std::bitset<8>(RS2_val) << std::endl;
	std::cout << "imm1:          0b" << std::bitset<16>(imm1) << std::endl;
	std::cout << "imm2:          0b" << std::bitset<32>(imm2) << std::endl;
	std::cout << "RD:            0b" << std::bitset<8>(RD) << std::endl << std::endl;
}

Execute_reg::Execute_reg(CU_signals CU_val, uint8_t RS1_v, uint32_t RS1_vv, uint32_t ALUresult_val, uint8_t RD_val) {
	CU_reg = CU_val;
	RS1 = RS1_v;
	RS1_val = RS1_vv;
	ALUresult = ALUresult_val;
	RD = RD_val;
}

void Execute_reg::print_reg() {
	std::cout << "EXECUTE_REG: " << std::endl;
	CU_reg.print_CU_signals();
	std::cout << "RS1:           0b" << std::bitset<8>(RS1) << std::endl;
	std::cout << "RS1_val:       0b" << std::bitset<32>(RS1_val) << std::endl;
	std::cout << "ALUresult:     0b" << std::bitset<32>(ALUresult) << std::endl;
	std::cout << "RD:            0b" << std::bitset<8>(RD) << std::endl << std::endl;
}

Memory_reg::Memory_reg(CU_signals CU_reg_val,uint32_t mux_result_val,uint8_t RD_val) {
	CU_reg = CU_reg_val;
	mux_result = mux_result_val;
	RD = RD_val;
}

void Memory_reg::print_reg() {
	std::cout << "MEMORY_REG: " << std::endl;
	CU_reg.print_CU_signals();
	std::cout << "mux_result:    0b" << std::bitset<32>(mux_result) << std::endl;
	std::cout << "RD:            0b" << std::bitset<8>(RD) << std::endl << std::endl;
}
//---------------------------------------------------------------------------------------------------------------

void Insn_data_memory::print_memory() {
	std::cout << "INSN_DATA_MEMORY:" << std::endl;
	for (uint32_t PC = 0; PC < insn.size(); PC++) 
		std::cout << "0b" << std::bitset<32>(insn[PC]) << std::endl;
	std::cout << std::endl;
}

Regfile::Regfile(uint32_t regs[32]) {
	m_regs[0] = 0; 
	for (int i = 1; i < 32; i++) 
		m_regs[i] = regs[i];
}

void Regfile::print_regfile() {
	std::cout << "REGFILE:" << std::endl;
	for (int i = 0; i < 32; ++i)
		std::cout << "x" << i << ": 0b" << std::bitset<32>(m_regs[i]) << std::endl;
	std::cout << std::endl;
}