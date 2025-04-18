#include "symbols.h"

static List symbols; /*List of symbols*/

static uint8_t* symbol_names; /*Buffer containing raw strtab data*/

/*
* Loads the strtab section for symbol processing
* file = the file whose strtab to load
*/
static void load_symbol_names(FILE* file) {
    Fhdr header; /*File header*/
    uint64_t length; /*Length read*/
    symbol_names = readelfsection(file, ".strtab", &length, &header);
}

/*
* Add the symbol to the list based off address
* symbol = the symbol to insert
*/
static void ordered_insert(Symbol* symbol) {
    Element* curr = &symbols.head;
    Element* tail = &symbols.tail;
    
    while (curr->next != tail && symbol->address > get_entry(curr->next, Symbol, elem)->address) {
        curr = curr->next;
    }

    symbol->elem.prev = curr;
    symbol->elem.next = curr->next;
    curr->next->prev = &symbol->elem;
    curr->next = &symbol->elem;
}

/*
* Print the symbols
*/
static void __attribute__((unused)) print_symbols() {
    Element* curr = symbols.head.next;
    Element* tail = &symbols.tail;

    while (curr != tail) {
        Symbol* sym = get_entry(curr, Symbol, elem);
        printf("%s at address %lu with size %ld ending at %ld\n", sym->name, sym->address, sym->size, sym->address + sym->size);
        curr = curr->next;
    }

}

/*
* Iterate through the list and check symbols
*/
static void update_sizes() {
    assert(symbols.head.next != &symbols.tail && "No symbols loaded");
    Element* curr = symbols.head.next;
    Element* tail = &symbols.tail;

    while (curr->next != tail && curr != tail) {
        Symbol* sym = get_entry(curr, Symbol, elem);
        Symbol* next = get_entry(curr->next, Symbol, elem);
        if (sym->size == 0) {
            sym->size = next->address - sym->address;
        }
        curr = curr->next;
    }
}

/*
* load the text symbols from a given ELF file
* file = pointer to the ELF file
*/
void load_symbols(FILE* file) {

    //Load the symbol names
    load_symbol_names(file);

    //Create symbol list
    list_init(&symbols);

    //Load the symbol table
    Fhdr header; /*File header*/
    uint8_t* buffer; /*Buffer holding read data*/
    uint64_t length; /*Length read*/

    buffer = readelfsection(file, ".symtab", &length, &header);

    //Iterate over symtab and parses symbols with strings, addrs, offsets
    Elf32_Sym* symbol_buffer = (Elf32_Sym*) buffer;
    int num_symbols = length / sizeof(Elf32_Sym);
    for(int i = 0; i < num_symbols; i++) {
        Elf32_Sym current = symbol_buffer[i];

        //Verify it is a text symbol and has a valid name
        unsigned char symbol_type = ELF32_ST_TYPE(current.st_info);
        if (symbol_type != STT_FUNC || current.st_shndx == SHN_UNDEF) {
            continue;
        }

        char* name = (char*) symbol_names + current.st_name;
        if (*(name) == '\0' || !name) {
            continue;
        }

        Symbol* symbol = malloc(sizeof(Symbol));
        symbol->address = current.st_value;
        symbol->name = strdup(name);
        symbol->size = current.st_size;
        ordered_insert(symbol);
    }

    //Free buffer
    free(buffer);
    free(symbol_names);
    //Update the sizes so they reflect address differences
    update_sizes();
}

/*
* Get the next text symbol based off address
* sym = pointer to the previous symbol or NULL if first call
* return = NULL if last symbol or next Symbol
*/
Symbol* get_next_symbol(Symbol* sym) {

    if (sym == NULL) {
        Symbol* ret = get_entry(symbols.head.next, Symbol, elem);
        return ret;
    }

    Element* next = sym->elem.next;

    if (next == &symbols.tail) {

        return NULL;
    }

    Symbol* ret = get_entry(next, Symbol, elem);
    if (ret->address == sym->address) {
        return get_next_symbol(ret);
    }

    return ret;
}