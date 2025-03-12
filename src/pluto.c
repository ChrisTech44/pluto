#include <pluto.h>

static void 
print_err(const char * msg) {
    dprintf(STDERR_FILENO, "%s\n", msg);
}

/*Helper function to print the binary of an instructioin*/
void print_binary(uint32_t num) {
    // Iterate through each of the 32 bits
    for (int i = 31; i >= 0; i--) {
        // Check if the bit at position i is set (1) or not (0)
        if (num & (1 << i)) {
            printf("1");
        } else {
            printf("0");
        }
    }
    printf("        ");
}

static const char* 
get_register_name(uint8_t reg) {
    switch (reg) {
        case REG_ZERO: return "zero";
        case REG_RA: return "ra";
        case REG_SP: return "sp";
        case REG_GP: return "gp";
        case REG_TP: return "tp";
        case REG_T0: return "t0";
        case REG_T1: return "t1";
        case REG_T2: return "t2";
        case REG_S0: return "s0";
        case REG_S1: return "s1";
        case REG_A0: return "a0";
        case REG_A1: return "a1";
        case REG_A2: return "a2";
        case REG_A3: return "a3";
        case REG_A4: return "a4";
        case REG_A5: return "a5";
        case REG_A6: return "a6";
        case REG_A7: return "a7";
        case REG_S2: return "s2";
        case REG_S3: return "s3";
        case REG_S4: return "s4";
        case REG_S5: return "s5";
        case REG_S6: return "s6";
        case REG_S7: return "s7";
        case REG_S8: return "s8";
        case REG_S9: return "s9";
        case REG_S10: return "s10";
        case REG_S11: return "s11";
        case REG_T3: return "t3";
        case REG_T4: return "t4";
        case REG_T5: return "t5";
        case REG_T6: return "t6";
        default: return "unknown";  // In case of invalid register
    }
}

static const char*
get_r_instruction_name(uint8_t func3, uint8_t func7) {
    switch (func3) {
        case ADD_FUNC3:
            return func7 ? "sub" : "add";
        case XOR_FUNC3:
            return "xor";
        case OR_FUNC3:
            return "or";
        case AND_FUNC3:
            return "and";
        case SLL_FUNC3:
            return "sll";
        case SRL_FUNC3:
            return func7 ? "sra" : "srl";
        case SLT_FUNC3:
            return "slt";
        case SLTU_FUNC3:
            return "sltu";
        default:
            printf("ERROR: Invalid instruction\n");
            _exit(-1);
            return NULL;
    }

}

/*Methods for handling the various I-type instructions*/
static void 
process_reg_i_instruction(uint32_t instruction, uint8_t func3, uint8_t rs1, uint8_t rd) {
    uint8_t bits_5_11 = ((0xFE0 & instruction) >> 5);

    uint16_t us_immediate = ((0xFFF00000 & instruction) >> 20);
    bool is_signed = false; 
    int32_t s_immediate = 0;


    //Check if signed and extend if necessary
    if (us_immediate & 0x800) {
        is_signed = true;
        s_immediate = us_immediate;
        s_immediate |= 0xFFFFF000;
    }

    char* instruction_name = NULL;
    switch (func3) {
        case ADDI_FUNC3: instruction_name = "addi"; break;
        case XORI_FUNC3: instruction_name = "xori"; break;
        case OR_FUNC3: instruction_name = "or"; break;
        case ANDI_FUNC3: instruction_name = "andi"; break;
        case SLLI_FUNC3: instruction_name = "slli"; break;
        case SLTI_FUNC3: instruction_name = "slti"; break;
        case SLTIU_FUNC3: instruction_name = "sltiu"; break;
        case SRLI_FUNC3:
            if (bits_5_11) {
                instruction_name = "srli";
            } else {
                instruction_name = "srai";
            }
            break;
        default:
            printf("Unknown i instruction found\n");
            _exit(-1);
    }

    printf("%s %s, %s, ", instruction_name, get_register_name(rd), get_register_name(rs1));
    if (!is_signed) {
        printf("%u\n", us_immediate);
    } else {
        printf("%d\n", s_immediate);
    }
}
static void 
process_load_instruction(uint32_t instruction, uint8_t func3, uint8_t rs1, uint8_t rd) {
    uint16_t us_immediate = ((0xFFF00000 & instruction) >> 20);
    bool is_signed = false; 
    int32_t s_immediate = 0;


    //Check if signed and extend if necessary
    if (us_immediate & 0x800) {
        is_signed = true;
        s_immediate = us_immediate;
        s_immediate |= 0xFFFFF000;
    }

    char* instruction_name = NULL;

    switch (func3) {
        case LB_FUNC3:
            instruction_name = "lb";
            break;
        case LBU_FUNC3:
            instruction_name = "lbu";
            break;
        case LW_FUNC3:
            instruction_name = "lw";
            break;
        case LH_FUNC3:
            instruction_name = "lh";
            break;
        case LHU_FUNC3:
            instruction_name = "lhu";
            break;
        default:
            printf("Invalid load found\n");
            _exit(-1);
    }

    if (is_signed) {
        printf("%s %s, %d(%s)\n", instruction_name, get_register_name(rd), s_immediate, get_register_name(rs1));
    } else {
        printf("%s %s, %u(%s)\n", instruction_name, get_register_name(rd), us_immediate, get_register_name(rs1));
    }
}
static void
process_environ_instruction(uint32_t instruction) {
    uint16_t us_immediate = ((0xFFF00000 & instruction) >> 20);

    if (us_immediate) {
        printf("ebreak\n");
    } else {
        printf("ecall\n");
    }
}
static void
process_jalr_instruction(uint32_t instruction, uint8_t rs1, uint8_t rd) {
    uint16_t us_immediate = ((0xFFF00000 & instruction) >> 20);
    bool is_signed = false; 
    int32_t s_immediate = 0;

    //Check if signed and extend if necessary
    if (us_immediate & 0x800) {
        is_signed = true;
        s_immediate = us_immediate;
        s_immediate |= 0xFFFFF000;
    }

    if (is_signed) {
        printf("%s %s, %d(%s)\n", "jalr", get_register_name(rd), s_immediate << 1, get_register_name(rs1));
    } else {
        printf("%s %s, %u(%s)\n", "jalr", get_register_name(rd), us_immediate << 1, get_register_name(rs1));
    }

}
static void
process_i_instruction(uint32_t instruction) {
    uint8_t rd = ((0xF80 & instruction) >> 7);
    uint8_t func3 = ((0x7000 & instruction) >> 12);
    uint8_t rs1 = ((0xF8000 & instruction) >> 15);

    uint8_t opcode = get_opcode(instruction);

    if (opcode == I_OPCODE) {
        process_reg_i_instruction(instruction, func3, rs1, rd);

    } else if (opcode == LOAD_OPCODE) {
        //Load instruction
        process_load_instruction(instruction, func3, rs1, rd);
    } else if (opcode == ENVIRON_OPCODE) {
        //ecall or ebreak
        process_environ_instruction(instruction);
    } else {
        //JALR instruction
        process_jalr_instruction(instruction, rs1, rd);
    }
}

static void 
process_r_instruction(uint32_t instruction) {
    uint8_t rd = ((0xF80 & instruction) >> 7);
    uint8_t func3 = ((0x7000 & instruction) >> 12);
    uint8_t rs1 = ((0xF8000 & instruction) >> 15);
    uint8_t rs2 = ((0x1F00000 & instruction) >> 20);
    uint8_t func7 = ((0xFE000000 & instruction) >>25);

    printf("%s %s, %s, %s\n", get_r_instruction_name(func3, func7), get_register_name(rd), get_register_name(rs1), get_register_name(rs2));
}

static void
process_s_instruction(uint32_t instruction) {
    char* instruction_name = NULL;
    uint8_t func3 = ((0x7000 & instruction) >> 12);
    uint8_t rs1 = ((0xF8000 & instruction) >> 15);
    uint8_t rs2 = ((0x1F00000 & instruction) >> 20);


    switch (func3) {
        case SB_FUNC3: instruction_name = "sb"; break;
        case SH_FUNC3: instruction_name = "sh"; break;
        case SW_FUNC3: instruction_name = "sw"; break;
        default: printf("Unknown S-type instruction\n"); _exit(-1);
    }

    uint16_t lower_5 = (0xF80 & instruction) >> 7;
    uint16_t upper_7 = (0xFE000000 & instruction) >> 20;
    uint32_t u_immediate  = upper_7 | lower_5;
    bool is_signed = false; 
    int32_t s_immediate = 0;

    //Check if signed and extend if necessary
    if (u_immediate & 0x800) {
        is_signed = true;
        s_immediate = u_immediate;
        s_immediate |= 0xFFFFF000;
    }

    if (is_signed) {
        printf("%s %s, %d(%s)\n", instruction_name, get_register_name(rs2), s_immediate, get_register_name(rs1));
    } else {
        printf("%s %s, %u(%s)\n", instruction_name, get_register_name(rs2), u_immediate, get_register_name(rs1));
    }


}

static void
process_b_instruction(uint32_t instruction) {
    char* instruction_name = NULL;
    uint8_t func3 = ((0x7000 & instruction) >> 12);
    uint8_t rs1 = ((0xF8000 & instruction) >> 15);
    uint8_t rs2 = ((0x1F00000 & instruction) >> 20);

    switch (func3) {
        case BEQ_FUNC3: instruction_name = "beq"; break;
        case BNE_FUNC3: instruction_name = "bne"; break;
        case BLT_FUNC3: instruction_name = "blt"; break;
        case BGE_FUNC3: instruction_name = "bge"; break;
        case BLTU_FUNC3: instruction_name = "bltu"; break;
        case BGEU_FUNC3: instruction_name = "bgeu"; break;
        default:
            printf("Unknown branch instruction found\n");
            _exit(-1);
    }

    uint16_t b12 = ((0x80000000 & instruction) >> 19);
    uint16_t b10_to_5 = ((0x7E000000 & instruction) >> 20);
    uint16_t b4_to_1 = ((0xF00 & instruction) >> 7);
    uint16_t b11 = ((0x080 & instruction) << 4);

    uint16_t u_immediate = (b4_to_1 | b10_to_5 | b11 | b12);
    bool is_signed = false; 
    int32_t s_immediate = 0;

    //Check if signed and extend if necessary
    if (u_immediate & 0x800) {
        is_signed = true;
        s_immediate = u_immediate;
        s_immediate |= 0xFFFFF000;
    }

    if (is_signed) {
        printf("%s %s, %s, %d\n", instruction_name, get_register_name(rs1), get_register_name(rs2), s_immediate);
    } else {
        printf("%s %s, %s, %u\n", instruction_name, get_register_name(rs1), get_register_name(rs2), u_immediate);
    }
}
static void
process_u_instruction(uint32_t instruction)  {
    int8_t opcode = get_opcode(instruction);
    uint8_t rd = ((0xF80 & instruction) >> 7);
    char* instruction_name;

    if (opcode == LUI_OPCODE) {
        instruction_name = "lui";
    } else if (opcode == AUIPC_OPCODE){
        instruction_name = "auipc";
    } else {
        printf("Unknown U-type instruction found\n");
        _exit(-1);
    }


    uint32_t u_immediate = (0xFFFFF000 & instruction);
    if (u_immediate & 0x80000000) {
        int32_t s_immediate = u_immediate; 
        printf("%s %s, %d\n", instruction_name, get_register_name(rd), s_immediate >> 12);
    } else {
        printf("%s %s, %d\n", instruction_name, get_register_name(rd), u_immediate >> 12);
    }
}
static void
process_j_instruction(uint32_t instruction) {
    char* instruction_name = "jal";
    uint8_t rd = ((0xF80 & instruction) >> 7);
    uint32_t b20 = ((0x80000000 & instruction) >> 11);
    uint32_t b10_to_1 = ((0x7FE00000 & instruction) >> 20);
    uint32_t b11 = ((0x00100000 & instruction) >> 9);
    uint32_t b19_to_12 = ((0x000FF000 & instruction));

    if (b20) {
        int32_t s_immediate = b20 | b19_to_12 | b11 | b10_to_1;
        printf("%s %s, %d\n", instruction_name, get_register_name(rd), s_immediate);
    } else {
        b19_to_12 = b20 | b19_to_12 | b11 | b10_to_1;
        printf("%s %s, %u\n", instruction_name, get_register_name(rd), b19_to_12);

    }
}


static void 
process_instruction(uint32_t* instruction_ptr, enum instruction_format format) {
    printf("%p:     ", instruction_ptr);
    printf("%08x     ", *instruction_ptr);

    if (format == R) {
        process_r_instruction(*instruction_ptr);
    } else if (format == I) {
        process_i_instruction(*instruction_ptr);
    } else if (format == S) {
        process_s_instruction(*instruction_ptr);
    } else if (format == B) {
        process_b_instruction(*instruction_ptr);
    } else if (format == U) {
        process_u_instruction(*instruction_ptr);
    } else if (format == J) {
        process_j_instruction(*instruction_ptr);
    }
}

static inline uint8_t get_opcode(uint32_t instruction) {
    return 0x7F & instruction;
}

static enum instruction_format
determine_instruction_format(uint32_t instruction) {
    uint8_t opcode = get_opcode(instruction);
    switch (opcode) {
        case R_OPCODE:
            return R;
        case I_OPCODE:
            return I;
        case LOAD_OPCODE:
            return I;
        case JALR_OPCODE:
            return I;
        case ENVIRON_OPCODE:
            return I;
        case S_OPCODE:
            return S;
        case B_OPCODE:
            return B;
        case JAL_OPCODE:
            return J;
        case AUIPC_OPCODE:
            return U;
        case LUI_OPCODE:
            return U;
        default:
            printf("Unknown opcode found\n");
            return UNKNOWN;
    }
}   

int 
main(int argc, char** argv) {    

    if (argc != 2) {
        print_err("Usage ./pluto <path/to/file>\n");
        return -1;
    }

    Fhdr header; /*File header*/
    FILE* file; /*File to read information from*/
    uint8_t* buffer; /*Buffer holding read data*/
    uint64_t length; /*Length read*/
    
    file = fopen(argv[1], "r"); /*Open file*/
    
    if (file == NULL) {         /*Check if success*/ 
        printf("%s\n", strerror(errno)); 
        return -1;
    }

    buffer = readelfsection(file, ".text", &length, &header);
    uint32_t* word_buffer = (uint32_t *) buffer;
    uint64_t instruction_pos = 0;
    while (instruction_pos < length) {

        enum instruction_format i_type = determine_instruction_format(*word_buffer);
        process_instruction(word_buffer, i_type);
        word_buffer++;
        instruction_pos += 4;
    }
    
    free(buffer);
    
    int ret_val = fclose(file);
    if (ret_val != 0) {
        printf("%s\n", strerror(errno)); 
        return -1;
    }

    return 0;
}
