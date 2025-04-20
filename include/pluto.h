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

#include "elf-local.h"
#include "symbols.h"


/*
* RV32I 40 Instructions--------------
*/

/*Helper methods*/
static void 
process_instruction32(uint32_t*, enum instruction32_format, uintptr_t);
static inline uint8_t 
get_opcode32(uint32_t);


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

/*B-type*/
static void
process_b_instruction(uint32_t, uintptr_t);

/*U-type*/
static void
process_u_instruction(uint32_t);

/*J-type*/
static void
process_j_instruction(uint32_t, uintptr_t);


/*
* Compressed Instructions--------------
*/
// static void
// process_compressed_instruction(uint32_t);
// static void 
// process_instruction16(uint16_t*, enum instruction32_format);


/*
*Helper Functions
*/
static const char* 
get_register_name(uint8_t);
static Format32 
determine_instruction_format32(uint32_t);
static void
print_err(const char*);


