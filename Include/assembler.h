#ifndef _ASSEMBLER_H_
#define _ASSEMBLER_H_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

/* Structure for an entry in the entry table */
struct entry_table * new_ent_table_node(char* symbolName,int Address_);

/* Structure for an entry in the extern table */
struct extern_table * new_ext_table_node(char* symbolName,int Address_);

/* Structure for a node in the macro definition list */
typedef struct macro_def_Node_
{
   char macros_arr[650]; /* Array to store macro definition */
   struct macro_def_Node_ *next; /* Pointer to next node */
}Node_macro_def;

/* Function to create a new node in the macro definition list */
Node_macro_def *newNode(char* brr);

/* Structure for a node in the macro table */
typedef struct macro_Table_Node_
{
   char macros_name[650]; /* Array to store macro name */
   struct macro_Table_Node_ *nextMacros; /* Pointer to next node in macro table */
   Node_macro_def *ptr_to_def; /* Pointer to the definition of the macro */
}Node_macro_Table;

/* Function to create a new node in the macro table */
Node_macro_Table *new_Table_Node(char* arr);

/* Structure for the symbol table */
typedef struct symbolTable_ {
   char symbolname[20]; /* Name of the symbol */
   int Address_; /* Address of the symbol */
   struct symbolTable_ * pNextSymbol; /* Pointer to next symbol in the table */
}symbol_table;

/* Function to get the address of a symbol from the symbol table */
int address_symbol(char * symbol,symbol_table * file_ptr);

/* Function to create a new node in the symbol table */
symbol_table * newSymbolTable(char* symbolName,int value);

/* Structure for an external symbol */
typedef struct symbol_ext_{
   char symbolName[20]; /* Name of the symbol */
   struct symbol_ext_ * nextSymbol; /* Pointer to the next external symbol */
}symbol_ext;

/* Function to create a new node for an external symbol */
symbol_ext * newexternSymbolNode(char* symbolName);

/* Structure for a command */
typedef struct Command_{
   char * pCmd; /* Pointer to the command */
   int number_of_operands; /* Number of operands for the command */
   int cmd_number; /* Number representing the command */
}Command;

extern Command commands[]; /* Array of command structures */

/* Structure for an entry symbol */
typedef struct symbol_ent_{
   char symbolName[20]; /* Name of the symbol */
   struct symbol_ent_ * nextSymbol; /* Pointer to the next entry symbol */
}symbol_ent;

/* Function to create a new node for an entry symbol */
symbol_ent* newentrySymbolNode(char* symbolName);

/* Structure for a node containing machine code bits */
struct bitNode{
   int Address_; /* Address of the machine code */
   char encoding[8]; /* Array to store the machine code */
   char symbol[20]; /* Symbol associated with the machine code */
   struct bitNode * nextNode; /* Pointer to the next node */
};

/* Structure for an entry in the entry table */
struct entry_table{
   char symbolName[20]; /* Name of the symbol */
   int Address_; /* Address of the symbol */
   struct entry_table * nextSymbolTable; /* Pointer to the next entry in the table */
};

/* Structure for an entry in the extern table */
struct extern_table{
   char symbolName[20]; /* Name of the symbol */
   int Address_; /* Address of the symbol */
   struct extern_table * nextSymbolTable; /* Pointer to the next entry in the table */
};

/* Function to create a new node in the machine code list */
struct bitNode * createMachineCode(FILE * file_am_ptr, symbol_table * ptr_sym_table,int * instruction_bit,int *data_bit,symbol_ext * ext_ptr,symbol_ent * ent_ptr,struct extern_table ** table_org_ext, struct entry_table ** table_org_ent);

/* Function to create a new node in the machine code list */
struct bitNode * new_bit_Code_Node(int Address_,char* encoding,char* symbol);

/* Function to create the output file */
void File_creation(char * name_file,struct extern_table * ext_ptr,struct entry_table * ent_ptr,struct bitNode * ptr_ob,int instruction_bit, int data_bit);

#endif /* _ASSEMBLER_H_ */
