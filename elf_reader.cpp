#include "elf_reader.h"
#include <iostream>

namespace elf
{
void Elf_reader::Init(const char *filename)
{
	
    if (e_ || fd_)
        Clear();


    if (elf_version(EV_CURRENT) == EV_NONE)
    	errx(EXIT_FAILURE , " ELF library initialization failed : % s " , elf_errmsg ( -1));

    fd_ = open(filename, O_RDONLY, 0);
    if (fd_ < 0)
    	err(EXIT_FAILURE, " open %s failed" , filename);

    e_ = elf_begin(fd_, ELF_C_READ, nullptr);
    
    if (!e_)
        errx(EXIT_FAILURE , " elf_begin () failed : % s . " , elf_errmsg ( -1));

    if (elf_kind(e_) != ELF_K_ELF)
		errx(EXIT_FAILURE , " elf_kind () failed : % s . " , elf_errmsg ( -1));	

    GElf_Ehdr ehdr;
    
    if (!gelf_getehdr(e_, &ehdr))
        errx(EXIT_FAILURE , " getehdr () failed : % s . " , elf_errmsg ( -1));
   
    //if (ehdr.e_machine != EM_RISCV)
    	std::cout<<ehdr.e_machine<<std::endl;
     //   print_and_exit("Elf: %s is not a RISCV machine\n", filename);




 	printf("%u\n",ehdr.e_ident[EI_CLASS]);

    if (ehdr.e_ident[EI_CLASS] != ELFCLASS32)
        errx( EXIT_FAILURE , " getident () failed : % s . " , elf_errmsg ( -1));	
   		//printf ( " %3s e_ident [0..%1d ] %7s " , " " , EI_ABIVERSION , " " );




    if (elf_getphdrnum(e_, &phdrnum_))
    	errx( EXIT_FAILURE , " getphdrnum () failed : % s . " , elf_errmsg ( -1));
    
    entry_ = ehdr.e_entry;
}

bool Elf_reader::Load(std::vector<uint32_t> &cmds, uint32_t &va, uint32_t &offset)
{
    for (; cur_phdr_ < phdrnum_; ++cur_phdr_)
    {
        GElf_Phdr *gres = gelf_getphdr(e_, cur_phdr_, &phdr_);
        if (gres != &phdr_)
           errx( EXIT_FAILURE , " getphdr () failed : % s . " , elf_errmsg ( -1));
        
        if (phdr_.p_type != PT_LOAD)
            continue;
		
        long int off = phdr_.p_offset;
        if (off != lseek(fd_, off, SEEK_SET))
            errx(EXIT_FAILURE ,"Elf: lseek failed\n", elf_errmsg(-1));

        std::vector<uint32_t>buf(phdr_.p_memsz);

        int read_sz = read(fd_, &(buf[0]), phdr_.p_filesz);
        
        if (read_sz < 0 || phdr_.p_filesz != static_cast<uint32_t>(read_sz))
            errx(EXIT_FAILURE ,"Elf: segment reading failed\n",elf_errmsg(-1));

        ++cur_phdr_;
        std::swap(cmds, buf);
        va = phdr_.p_vaddr;
        offset = entry_;   //- va;   // phdr_.p_vaddr;
        return true;
    }
    return false;
}

void Elf_reader::Clear()
{
    elf_end(e_);
    e_ = nullptr;
    close(fd_);
    fd_ = 0;
}

Elf_reader::~Elf_reader() {}
}   // namespace elf
