all:
	g++ -std=gnu++11 elf_reader.h elf_reader.cpp memory.cpp main.cpp -o psim -lelf -lm -w
