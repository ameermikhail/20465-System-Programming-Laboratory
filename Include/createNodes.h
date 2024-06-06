#ifndef _CREATENODES_H_
#define _CREATENODES_H_

#include "assembler.h"
#include "utilityFile.h"

/* Function to create a new node in the machine code list */
struct bitNode * new_bit_Code_Node(int Address_,char* encoding,char* symbol);

/* Function to create a new node in the extern table */
struct extern_table * new_ext_table_node(char * symbolName,int Address_);

/* Function to create a new node in the entry table */
struct entry_table * new_ent_table_node(char * symbolName,int Address_);

/* Function to create a new node for an external symbol */
symbol_ext * newexternSymbolNode(char * symbolName);

/* Function to create a new node for an entry symbol */
symbol_ent* newentrySymbolNode(char * symbolName);

/* Function to create a new node in the macro table */
Node_macro_Table *new_Table_Node(char* arr);

/* Function to create a new node in the macro definition list */
Node_macro_def *newNode(char* brr);

/* Function to create a new node in the symbol table */
symbol_table * newSymbolTable(char *symbolName,int value);

#endif /* _CREATENODES_H_ */
