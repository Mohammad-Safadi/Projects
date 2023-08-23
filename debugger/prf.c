#include <stdio.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <signal.h>
#include <syscall.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/reg.h>
#include <sys/user.h>
#include <unistd.h>
#include <errno.h>
#include "elf64.h"
#include <string.h>

typedef struct string string;
pid_t run_target(const char* programname)
{
    pid_t pid;

    pid = fork();

    if (pid > 0) {
        return pid;

    } else if (pid == 0) {
        if (ptrace(PTRACE_TRACEME, 0, NULL, NULL) < 0) {
            perror("ptrace");
            exit(1);
        }
        execl(programname, programname, NULL);

    } else {
        perror("fork");
        exit(1);
    }
}

void run_breakpoint_debugger(pid_t child_pid, unsigned long addr)
{
    int wait_status;
    struct user_regs_struct regs;

    /* Wait for child to stop on its first instruction */
    wait(&wait_status);
    unsigned long long first_data_in_func = ptrace(PTRACE_PEEKTEXT, child_pid, (void*)addr, NULL);
    unsigned long long first_data_trap = (first_data_in_func & 0xFFFFFFFFFFFFFF00) | 0xCC;
    unsigned long long first_func_call_rbp = 0;
    while(!WIFEXITED(wait_status)){
        ptrace(PTRACE_POKETEXT,child_pid, (void*)addr, (void*)first_data_trap);
        ptrace(PTRACE_CONT, child_pid, 0, 0);
        wait(&wait_status);
        ptrace(PTRACE_POKETEXT,child_pid,(void*)addr,(void*)first_data_in_func);
        ptrace(PTRACE_GETREGS, child_pid, 0, &regs);
        regs.rip-=1;
        if((regs.rip) != addr){
            continue;
        }
        ptrace(PTRACE_SETREGS, child_pid, 0, &regs);
        if(!first_func_call_rbp){
            first_func_call_rbp=regs.rbp;
        }
        unsigned long long return_addr = ptrace(PTRACE_PEEKTEXT, child_pid, (void *) regs.rsp, NULL);
        unsigned long long first_data_in_return = ptrace(PTRACE_PEEKTEXT, child_pid, (void *) return_addr, NULL);
        unsigned long long return_data_trap = (first_data_in_return & 0xFFFFFFFFFFFFFF00) | 0xCC;
        ptrace(PTRACE_POKETEXT, child_pid, (void *) return_addr, (void *) return_data_trap);
        int in_func=1;
        while(in_func){
            ptrace(PTRACE_SYSCALL, child_pid, NULL, NULL);
            wait(&wait_status);
            ptrace(PTRACE_GETREGS, child_pid, 0, &regs);
            if ((regs.rip-1) == return_addr){
                ptrace(PTRACE_POKETEXT, child_pid, (void *) return_addr, (void *) first_data_in_return);
                ptrace(PTRACE_GETREGS, child_pid, 0, &regs);
                regs.rip-=1;
                ptrace(PTRACE_SETREGS, child_pid, 0, &regs);
                if(regs.rbp == first_func_call_rbp){
                    first_func_call_rbp = 0;
                    in_func=0;
                    break;
                } else {
                    ptrace(PTRACE_SINGLESTEP, child_pid, NULL, NULL);
                    wait(&wait_status);
                    ptrace(PTRACE_POKETEXT, child_pid, (void *) return_addr, (void *) first_data_trap);
                }

            }
            ptrace(PTRACE_SYSCALL, child_pid, NULL, NULL);
            wait(&wait_status);
            ptrace(PTRACE_GETREGS, child_pid, 0, &regs);
            if ((long long )regs.rax<0){
                printf("PRF:: syscall in %llx returned with %lld\n",regs.rip-0x2 , regs.rax);
            }
        }
    }
}






int main(int argc, char** argv) {
    FILE * pointer_in_file=fopen(argv[2], "rb");
    if (!pointer_in_file){
        exit(1) ;
    }
    Elf64_Ehdr elf_header;
    fread(&elf_header,sizeof (elf_header), 1, pointer_in_file);
    Elf64_Off start_of_section_header=elf_header.e_shoff;
    fseek(pointer_in_file, start_of_section_header, SEEK_SET);
    Elf64_Half entries_num_section_header=elf_header.e_shnum;
    Elf64_Shdr *sections=malloc(sizeof (Elf64_Shdr)*entries_num_section_header);
    if (sections == NULL){
        fclose(pointer_in_file);
        exit(1);
    }
    for (int i=0; i<entries_num_section_header; i++){
        fread(&sections[i], sizeof(Elf64_Shdr), 1, pointer_in_file);
    }

    Elf64_Off sym_tab_off=0;
    Elf64_Xword sym_table_size;
    Elf64_Xword sym_table_one_entry_size;
    Elf64_Word sym_table_link;
    for (int i = 0; i <entries_num_section_header ; ++i) {
        if (sections[i].sh_type==2){
            sym_tab_off=sections[i].sh_offset;
            sym_table_size=sections[i].sh_size;
            sym_table_one_entry_size=sections[i].sh_entsize;
            sym_table_link=sections[i].sh_link;
        }
    }
    if (!sym_tab_off  || !sym_table_size){
        free(sections);
        fclose(pointer_in_file);
        exit(1) ;
    }
    unsigned int sym_tab_entries= sym_table_size / sym_table_one_entry_size;
    Elf64_Sym * sym_table=malloc(sizeof (Elf64_Sym)*sym_tab_entries);
    if (!sym_table){
        free(sections);
        fclose(pointer_in_file);
        exit(1);
    }
    fseek(pointer_in_file, sym_tab_off, SEEK_SET);
    for (int i = 0; i < sym_tab_entries; ++i) {
        fread(&sym_table[i], sym_table_one_entry_size, 1, pointer_in_file);
    }
    char tmp_char;
    char* symbol_name;
    /*if (!symbol_name){
        free(sym_table);
        free(sections);
        fclose(pointer_in_file);
        exit(1);
    }*/
    int is_func_not_global=1;
    int is_func_not_exist=1;
    Elf64_Addr func_addr=0;
    Elf64_Shdr str_tab_section=sections[sym_table_link];
    for (int i=0; i<sym_tab_entries;i++){
        if (sym_table[i].st_name){
            char c;
            fseek(pointer_in_file, (sym_table[i].st_name),SEEK_SET);
            c=(char)fread(&c, 1, 1, pointer_in_file);
            int j=0;
            while (c){

                j++;
                c=(char)fread(&c, 1, 1, pointer_in_file);
            }
            symbol_name=malloc(sizeof (char)*(j+1));
            fseek(pointer_in_file,(sym_table[i].st_name)+str_tab_section.sh_offset,SEEK_SET );
            fread(symbol_name, j+1, 1, pointer_in_file);

            if (!strcmp(argv[1], symbol_name)){
                is_func_not_exist=0;
                if (sym_table[i].st_shndx!=1){
                    is_func_not_exist=1;
                }
                func_addr=sym_table[i].st_value;
                if (ELF64_ST_BIND(sym_table[i].st_info) != 0) {
                    is_func_not_global=0;
                }
                break;
            }
            free(symbol_name);
        }
    }
    free(sym_table);
    free(symbol_name);
    free(sections);
    fclose(pointer_in_file);

    if (is_func_not_exist){
        printf("PRF:: not found!\n");
        return 0;
    }
    if (is_func_not_global){
        printf("PRF:: local found!\n");
        return 0;
    }


    pid_t child_pid;
    child_pid = run_target(argv[2]);
    run_breakpoint_debugger(child_pid, func_addr);
    return 0;



}
