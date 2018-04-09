#include "memory.h"
#include "stages.h"

int main() {
	//global instruction and data memory
	Insn_data_memory instr_data_mem;
	//global regfile
	Regfile register_file;

	std::vector<uint32_t> insns = { 0b011111100111000010000101000000000,
								    0b01000000100000001000101000011111 };
	instr_data_mem.set_instr(insns);

	uint32_t PC = 0;
	uint32_t PC_DISP = 0;
	uint8_t PC_R = 0;
 
	for (int i = 0; i < insns.size(); i++) {
		//Fetch stage
		//After this stage we get int time_
		Fetch_reg fetch_reg = fetch(instr_data_mem, PC, PC_DISP, PC_R); 
		fetch_reg.print_reg();

		//Decode stage
		Decode_reg decode_reg = decode(fetch_reg, register_file);
		//decode_reg.print_reg();

		Execute_reg execute_reg = execute(decode_reg, PC_DISP, PC_R);
		//execute_reg.print_reg();

		Memory_reg memory_reg = memory(execute_reg, instr_data_mem);
		//memory_reg.print_reg();

		write_back(memory_reg, register_file);
	}

#if 0
insn_memory instr_memory(insn);
instr_memory.print_instruction(PC);

PC = alu(0,PC,1);

Regfile new_regfile(15,20,40);
//получаем значение RS1 
int32_t A1 = new_regfile.get_register(insn, 15);

std::cout<<"A1 is: "<<std::bitset<32>(A1)<<std::endl;

//получаем значение immidiate
int32_t imm = new_regfile.get_register(insn,20);

std::cout<<"Imm: "<<std::bitset<32>(imm)<<std::endl;

int32_t sign_extended_imm = sign_extend(insn);

std::cout<<"sign_extended_imm: "<<std::bitset<32>(sign_extended_imm)<<std::endl;

int32_t alu_result = alu(0,A1,sign_extended_imm);

std::cout<<"ALU result "<<std::bitset<32>(alu_result)<<std::endl;

uint32_t value_ = new_regfile.data_memory(alu_result);

std::cout<<"The data_memory of index[alu_result] "<<std::bitset<32>(value_)<<std::endl;

new_regfile.set_register(insn, value_);

new_regfile.print_regfile();

//std::cout<<std::bitset<32>(sign_extend(insn));
#endif

//insn_memory new_insn(insn);
//new_insn.print_instruction();


#if 0
	int32_t PC = 0;

	Regfile reg_file(4,5,6);
	//reg_file.print_regfile();

	Instruction Instruction1(1, 2, 3, 0b000011, 0b000000111010, 0b010, 0b011);
	Instruction1.print_Instruction();	

	Instruction Instruction2(3, 2, 1, 0b00001, 0b000100111010, 0b000, 0b010);
	

	insn_memory new_memory(Instruction1, Instruction2);
	new_memory.print_memory();

	std::cout<<"returned instruction"<<std::endl;
	new_memory.return_insn(PC).print_Instruction();

	//insn_memory new_object

#endif
	return 0;
}
