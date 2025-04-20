#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <elf.h>
#include <string.h>
#include <stdbool.h> 
#include <assert.h>
#include "list.h"
#include "elf-local.h" 

typedef struct pluto_symbol {
    Element elem;
    char* name;
    uintptr_t address;
    size_t size;
} Symbol; 

void load_symbols(FILE*);
Symbol* get_symbol(uintptr_t);
Symbol* get_next_symbol(Symbol*);