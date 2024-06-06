#ifndef _UTILITYFILE_H_
#define _UTILITYFILE_H_

#include "assembler.h"
#include<string.h>
#include<stdlib.h>
#include<stdio.h>

/* Function to create symbol table */
symbol_table * createSymTable(symbol_ext ** ptr_EXT, symbol_ent ** ptr_ENT,FILE *file_am_ptr,int *count_line,int *e_flag);

/* Function to check if a token is present in the symbol table */
int check_In_symbolTable(char * Token,symbol_table * sym);

/* Function to fetch constant data from symbol table */
int fetch_const_data(char* symbol,symbol_table * ptr_symbol);

/* Function to check if a command is valid */
int check_command(char * cmd);

/* Function to check the address type */
int check_addressType(char * check_address);

/* Function to fetch address of a symbol from symbol table */
int address_symbol(char * symbol,symbol_table * sym);

/* Function to check if a symbol is present in the external symbol table */
int check_In_externTable(char* symbol,symbol_ext * sym);

/* Function to check if a symbol is present in the entry symbol table */
int check_In_entryTable(char* symbol,symbol_ent * sym);

/* Function to check if a token represents a constant */
int check_constant(char * check);

/* Function to check if a token represents an array object */
int check_array_Obj(char * Obj_arr);

/* Function to check if a token represents a register */
int check_register(char * regis);

#endif /* _UTILITYFILE_H_ */
