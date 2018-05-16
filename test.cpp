#include <bitset>
#include <iostream>
#include <fstream>
#include <vector>
//BEQ x3, x5, -1

uint32_t get_bits(uint32_t insn, unsigned int pos, unsigned int n) {
    return ((insn & (~(~0 << n) << pos)) >> pos);
} 

uint32_t sign_extend(uint32_t insn)
{
	if (get_bits(insn, 31, 1)) return 0xFFFFFF<<11;
	return 0;
}

int main()

{
	//uint32_t test_inst = 0b 1111111 00101 00101 000 11101 1100011; // beq x5, x5, -2
	//uint32_t test_inst = 0b11111110010100101000111011100011; // beq x5, x5, -2
	uint32_t a = 0b00000000000000000000000000000010;
	//uint32_t a = 0b11111111111111111111111111111110;
	std::cout<<static_cast<int32_t>(a)<<std::endl;

	//uint32_t test_inst = {0b 0000000 00011 00101 000 11101 1100011}; //beq x3, x5, 2
	uint32_t test_inst = {0b00000000001100101000000101100011};
	uint8_t rs2 = get_bits(test_inst,31,1);
	std::cout<<std::bitset<32>(rs2)<<std::endl;
	uint16_t imm1 = 0b11111100101;
	uint32_t imm2 = sign_extend(test_inst);
	uint8_t rd = 0b11101;

	uint32_t imm_input = imm1 | imm2;
	uint32_t imm_input_S = imm2 | ((get_bits(imm1,5,6) << 5 ) | rd);
	uint32_t imm_input_B = imm2 | ( (get_bits(rd,1,4) | get_bits(imm1,5,5) << 4 | get_bits(imm1,10,1) << 9 | get_bits(imm2,31,1) << 10 ));
//works!


	std::cout<<std::bitset<32>(imm1)<<std::endl;
	std::cout<<std::bitset<32>(imm2)<<std::endl;
	std::cout<<std::bitset<32>(imm_input)<<std::endl;
	std::cout<<std::bitset<32>(imm_input_S)<<std::endl;
	
	std::cout<<"B "<<std::bitset<32>(imm_input_B)<<std::endl;
	std::cout<<std::bitset<32>((get_bits(rd,1,4)))<<std::endl;
	std::cout<<std::bitset<32>((get_bits(imm1,5,5) << 4))<<std::endl;
	std::cout<<std::bitset<32>((get_bits(imm1,11,1) << 9))<<std::endl;
	std::cout<<std::bitset<32>((get_bits(imm1,12,1) << 10))<<std::endl;
	
	std::cout<<std::bitset<32>((get_bits(rd,1,4)) | (get_bits(imm1,5,5) << 4) )<<std::endl;

	//std::vector<uint32_t> insns = {0b 1000000 00011 00101 000 00110 1100011};
}
