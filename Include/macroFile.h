#ifndef _MACROFILE_H_
#define _MACROFILE_H_

#include "macroFile.h"
#include<stdio.h>
#include "utilityFile.h"
#include<stdlib.h>
#include "assembler.h"

/* Function to check if a macro is present in the macro table */
Node_macro_def *MacroCheck(char * macro_arr, Node_macro_Table *Macro_table_ptr);

/* Function to extract macros from a file */
Node_macro_Table *extracting_macros(FILE *file);

#endif /* _MACROFILE_H_ */
