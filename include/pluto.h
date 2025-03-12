#define _GNU_SOURCE 1
#include<unistd.h>
#include<errno.h>
#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<string.h>
#include<elf.h>
#include<instruction_formats.h>
#include<stdbool.h>

/*R-type*/
static void 
process_r_instruction(uint32_t);
static const char* 
get_r_instruction_name(uint8_t, uint8_t);

/*I-type*/
static void 
process_i_instruction(uint32_t);
static void 
process_reg_i_instruction(uint32_t, uint8_t, uint8_t, uint8_t);
static void 
process_load_instruction(uint32_t, uint8_t, uint8_t, uint8_t);
static void
process_environ_instruction(uint32_t);
static void
process_jalr_instruction(uint32_t, uint8_t, uint8_t);

/*S-type*/
static void 
process_s_instruction(uint32_t);

/*Helper Functions*/
static void 
process_instruction(uint32_t*, enum instruction_format);
static inline uint8_t 
get_opcode(uint32_t);
static const char* 
get_register_name(uint8_t);
static enum instruction_format 
determine_instruction_format(uint32_t);
static void
print_err(const char*);



