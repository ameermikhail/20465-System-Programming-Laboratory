#include "macroFile.h"
#include<stdlib.h>
#include<string.h>
#include<stdio.h>

Node_macro_def *MacroCheck(char macro_arr[], Node_macro_Table *Macro_table_ptr)
{
   /* Function to check for a macro definition in the macro table */
   while (Macro_table_ptr!=NULL)
   {
      /* If the current macro name matches the provided macro array */
      if (strcmp(Macro_table_ptr->macros_name,macro_arr) == 0)
      {
         /* Return the pointer to the macro definition */
         return Macro_table_ptr->ptr_to_def;
      }
      /* Move to the next macro table entry */
      Macro_table_ptr = Macro_table_ptr->nextMacros;
   }
   /* Return NULL if macro not found */
   return NULL;
}

Node_macro_Table *extracting_macros(FILE *_file_)
{
   /* Function to extract macros from a file */
   char Line_number[99+1];
   int check = 0;
   Node_macro_def *Macro_def_ptr = NULL;
   Node_macro_def *Macro_def_ptr_curr = NULL;
   Node_macro_Table *Macro_table_ptr = NULL;
   Node_macro_Table *Macro_table_ptr_curr = NULL;
   while (fgets(Line_number, sizeof(Line_number), _file_))
   {  
      if (check == 1)
      {
         char arr2[99+1];
         char *word_two;
         strcpy(arr2, Line_number);
         word_two = strtok(Line_number, " \t\n");
         while (word_two != NULL)
         {
            if (strcmp(word_two, "endmcr") == 0)
            {
               /* End of macro definition */
               check = 0;
               /* Assign the macro definition to the current macro table entry */
               Macro_table_ptr_curr->ptr_to_def = Macro_def_ptr;
               /* Reset pointers for the next macro */
               Macro_def_ptr_curr = NULL;
               Macro_def_ptr = NULL;
               break;
            }
            word_two = strtok(NULL, " \t\n");
         }
         if (check == 1)
         {
            if (Macro_def_ptr == NULL)
            {
               /* Create a new node for the macro definition */
               Macro_def_ptr = newNode(arr2);
               Macro_def_ptr_curr = Macro_def_ptr;
            }
            else
            {
               /* Add a new node for the macro definition */
               Macro_def_ptr_curr->next = newNode(arr2);
               Macro_def_ptr_curr = Macro_def_ptr_curr->next;
            }
         }
      }
      else if (check == 0)
      {  char *word_one;
         word_one = strtok(Line_number, " \t\n");
         while (word_one != NULL)
         {
            if (strcmp(word_one, "mcr") == 0)
            {  char macros_name[100];
               /* Start of macro definition */
               check = 1;
               word_one = strtok(NULL, " \t\n");
               strcpy(macros_name, word_one);
               if (Macro_table_ptr == NULL)
               {
                  /* Create a new node for the macro table */
                  Macro_table_ptr = new_Table_Node(macros_name);
                  Macro_table_ptr_curr = Macro_table_ptr;
               }
               else
               {
                  /* Add a new node for the macro table */
                  Macro_table_ptr_curr->nextMacros = new_Table_Node(macros_name);
                  Macro_table_ptr_curr = Macro_table_ptr_curr->nextMacros;
               }
               break;
            }

            word_one = strtok(NULL, " \t\n");
         }
      }
      
   }
   /* Return the pointer to the macro table */
   return Macro_table_ptr;
}
