/* define opcodes*/
#define R_OPCODE 0b0110011
#define I_OPCODE 0b0010011
#define LOAD_OPCODE 0b0000011
#define ENVIRON_OPCODE 0b1110011
#define S_OPCODE 0b0100011
#define B_OPCODE 0b1100011
#define LUI_OPCODE 0b0110111
#define AUIPC_OPCODE 0b0010111
#define JAL_OPCODE 0b1101111
#define JALR_OPCODE 0b1100111


/*Define the RISC-V symbolic names*/
// Zero Register
#define REG_ZERO     0  // X0 (Always zero)
// Return Address
#define REG_RA       1  // X1 (Return address register)
// Stack Pointer
#define REG_SP       2  // X2 (Stack pointer register)
// Global Pointer
#define REG_GP       3  // X3 (Global pointer register)
// Thread Pointer
#define REG_TP       4  // X4 (Thread pointer register)

// Temporary Registers
#define REG_T0       5  // X5 (Temporary register 0)
#define REG_T1       6  // X6 (Temporary register 1)
#define REG_T2       7  // X7 (Temporary register 2)

// Saved Registers
#define REG_S0       8  // X8 (Saved register 0)
#define REG_S1       9  // X9 (Saved register 1)

// Argument Registers
#define REG_A0       10 // X10 (Argument register 0)
#define REG_A1       11 // X11 (Argument register 1)
#define REG_A2       12 // X12 (Argument register 2)
#define REG_A3       13 // X13 (Argument register 3)
#define REG_A4       14 // X14 (Argument register 4)
#define REG_A5       15 // X15 (Argument register 5)
#define REG_A6       16 // X16 (Argument register 6)
#define REG_A7       17 // X17 (Argument register 7)

// Saved Registers (continued)
#define REG_S2       18 // X18 (Saved register 2)
#define REG_S3       19 // X19 (Saved register 3)
#define REG_S4       20 // X20 (Saved register 4)
#define REG_S5       21 // X21 (Saved register 5)
#define REG_S6       22 // X22 (Saved register 6)
#define REG_S7       23 // X23 (Saved register 7)
#define REG_S8       24 // X24 (Saved register 8)
#define REG_S9       25 // X25 (Saved register 9)
#define REG_S10      26 // X26 (Saved register 10)
#define REG_S11      27 // X27 (Saved register 11)

// Temporary Registers (continued)
#define REG_T3       28 // X28 (Temporary register 3)
#define REG_T4       29 // X29 (Temporary register 4)
#define REG_T5       30 // X30 (Temporary register 5)
#define REG_T6       31 // X31 (Temporary register 6)

/*
* RV32I 40 Instructions--------------
*/
/*func3 and func7 codes for R-type instructions*/
#define ADD_FUNC3 0x0 //Same as SUB
#define XOR_FUNC3 0x4
#define OR_FUNC3 0x6
#define AND_FUNC3 0x7
#define SLL_FUNC3 0x1
#define SRL_FUNC3 0x5 //Same as SRA
#define SLT_FUNC3 0x2
#define SLTU_FUNC3 0x3

#define ADD_FUNC7 0x00
#define SUB_FUNC7 0x20
#define SRL_FUNC7 0x00
#define SRA_FUNC7 0x20

/*define func3 codes for I-type instructions*/
//Standard
#define ADDI_FUNC3 0x0
#define XORI_FUNC3 0x4
#define ORI_FUNC3 0x6
#define ANDI_FUNC3 0x7
#define SLLI_FUNC3 0x1
#define SRLI_FUNC3 0x5
#define SLTI_FUNC3 0x2
#define SLTIU_FUNC3 0x3
//Load
#define LB_FUNC3 0x0
#define LH_FUNC3 0x1
#define LW_FUNC3 0x2
#define LBU_FUNC3 0x4
#define LHU_FUNC3 0x5
//Special
#define JALR_FUNC3 0x0
#define ECALL_FUNC3 0x0

/*Define func3 codes for S-type instructions*/
#define SB_FUNC3 0x0
#define SH_FUNC3 0x1
#define SW_FUNC3 0x2

/*Define func3 codes for B-type instructions*/
#define BEQ_FUNC3 0x0
#define BNE_FUNC3 0x1
#define BLT_FUNC3 0x4
#define BGE_FUNC3 0x5
#define BLTU_FUNC3 0x6
#define BGEU_FUNC3 0x7


/*Enumeration for isntruction formats*/
typedef enum instruction32_format {
    R, 
    I, 
    S, 
    B, 
    U, 
    J
} Format32;


/*
*16 Bit Compressed Instructions ----------------
*/
/*Enumeration for isntruction formats*/
typedef enum instruction16_format {
    CR, 
    CI, 
    CSS, 
    CIW, 
    CL, 
    CS,
    CB,
    CJ
} Format16;

