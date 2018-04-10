#include "memory.h"

uint32_t multiplexor4(uint32_t rs2_val, uint16_t imm1, uint32_t imm2, uint32_t rd, uint8_t mux_ex) {
	switch (mux_ex) {
		case 0: return rs2_val;
		case 1: return imm1;
		case 2: return imm2;
		case 3: return rd;
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

void CU_signals::set_sginals(uint8_t WB_WE_, uint8_t MEM_WE_, uint8_t mux_ex1_,
							 uint8_t AluOp_, uint8_t mux_mem1_, uint8_t conditional_) {
	WB_WE = WB_WE_;
	MEM_WE = MEM_WE_;
	mux_ex1 = mux_ex1_;
	AluOp = AluOp_;
	mux_mem1 = mux_mem1_;
	conditional = conditional_;
}

void CU_signals::print_CU_signals() {
	std::cout << "CU_signals:" << std::endl;
	std::cout << "  WB_WE:       " << std::bitset<1>(WB_WE) << std::endl;
	std::cout << "  MEM_WE:      " << std::bitset<1>(MEM_WE) << std::endl;
	std::cout << "  AluOp:       " << std::bitset<2>(AluOp) << std::endl;
	std::cout << "  mux_ex1:     " << std::bitset<2>(mux_ex1) << std::endl;
	std::cout << "  mux_mem1:    " << std::bitset<1>(mux_mem1) << std::endl;
	std::cout << "  conditional: " << std::bitset<1>(conditional) << std::endl;
}

CU_signals control_unit(uint32_t insn) {
	uint8_t opcode = get_bits(insn, 0, 7);
	uint8_t funct3 = get_bits(insn, 12, 3);
	uint8_t funct7 = get_bits(insn, 25, 7);

	CU_signals signals;

	if (opcode == 0b0110011) { // R-type
		switch (funct7) { // if funct7 = 01 - substitution , AluOp = 1
			case 0b0100000 : signals.set_sginals(1,0,0,1,1,0); // SUB
							 std::cout << "SUB" << std::endl << std::endl;
							 break;
			case 0b0000000 : signals.set_sginals(1,0,0,0,1,0); // ADD
							 std::cout << "ADD" << std::endl << std::endl;
							 break;
		}
	}
	// HOWTO?
	else if (opcode == 0b0000011) { // I-type
		signals.set_sginals(1,0,1,0,0,0); // LW
		std::cout << "LOAD" << std::endl << std::endl;
	}
	else if (opcode == 0b0100011) { // S-type
		signals.set_sginals(0,1,2,0,0,0); // SW
		std::cout << "STORE" << std::endl << std::endl;
	}
	else if (opcode == 0b1100011) { // B-type
		signals.set_sginals(0,0,0,0,0,1); // BEQ
		std::cout << "BRANCH" << std::endl << std::endl;
	}
	return signals;
}
//---------------------------------------------------------------------------------------------------------------

void Fetch_reg::print_reg() {
	std::cout << "FETCH_REG:     0b" << std::bitset<32>(reg) << std::endl << std::endl;	
}

Decode_reg::Decode_reg(CU_signals CU_reg_, uint8_t rs1_, uint8_t rs2_, uint32_t rs1_val_, uint32_t rs2_val_, uint8_t rd_, uint16_t imm1_, uint32_t imm2_) {
	CU_reg = CU_reg_;
	rs1 = rs1_;
	rs2 = rs2_;
	rs1_val = rs1_val_;
	rs2_val = rs2_val_;
	imm1 = imm1_;
	imm2 = imm2_;
	rd = rd_;
}

void Decode_reg::print_reg() {
	std::cout << "DECODE_REG:" << std::endl;
	std::cout << "RS1:           0b" << std::bitset<8>(rs1) << std::endl;
	std::cout << "RS2:           0b" << std::bitset<8>(rs2) << std::endl;
	std::cout << "RS1_val:       0b" << std::bitset<32>(rs1_val) << std::endl;
	std::cout << "RS2_val:       0b" << std::bitset<8>(rs2_val) << std::endl;
	std::cout << "imm1:          0b" << std::bitset<16>(imm1) << std::endl;
	std::cout << "imm2:          0b" << std::bitset<32>(imm2) << std::endl;
	std::cout << "RD:            0b" << std::bitset<8>(rd) << std::endl << std::endl;
}

Execute_reg::Execute_reg(CU_signals CU_reg_, uint32_t rs2_val_, uint32_t ALUresult_, uint8_t rd_) {
	CU_reg = CU_reg_;
	rs2_val = rs2_val_;
	ALUresult = ALUresult_;
	rd = rd_;
}

void Execute_reg::print_reg() {
	std::cout << "EXECUTE_REG: " << std::endl;
	CU_reg.print_CU_signals();
	std::cout << "RS2_val:       0b" << std::bitset<32>(rs2_val) << std::endl;
	std::cout << "ALUresult:     0b" << std::bitset<32>(ALUresult) << std::endl;
	std::cout << "RD:            0b" << std::bitset<8>(rd) << std::endl << std::endl;
}

Memory_reg::Memory_reg(CU_signals CU_reg_,uint32_t mux_result_,uint8_t rd_) {
	CU_reg = CU_reg_;
	mux_result = mux_result_;
	rd = rd_;
}

void Memory_reg::print_reg() {
	std::cout << "MEMORY_REG: " << std::endl;
	CU_reg.print_CU_signals();
	std::cout << "mux_result:    0b" << std::bitset<32>(mux_result) << std::endl;
	std::cout << "RD:            0b" << std::bitset<8>(rd) << std::endl << std::endl;
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