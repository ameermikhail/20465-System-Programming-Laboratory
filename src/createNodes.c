#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "createNodes.h"
#include "utilityFile.h"
#include "assembler.h"


/* Function to create a new bit code node */
struct bitNode * new_bit_Code_Node(int Address_,char* encoding,char* symbol){
   struct bitNode * node=(struct bitNode *)malloc(sizeof(struct bitNode)); /* Allocate memory for the new node */
   int i;
   node->Address_=Address_; /* Assign the provided address to the node */
   for(i=0;i<8;i++){
      node->encoding[i]=encoding[i]; /* Copy the encoding array to the node */
   }
   node->encoding[7]='\0'; /* Ensure the encoding string is null-terminated */
   for(i=0;i<10;i++){
      node->symbol[i]=symbol[i]; /* Copy the symbol array to the node */
   }
   node->nextNode=NULL; /* Set the next node pointer to NULL */
   return node; /* Return the newly created node */
}

/* Function to create a new external table node */
struct extern_table * new_ext_table_node(char* symbolName,int Address_){
  struct extern_table * node=(struct extern_table *)malloc(sizeof(struct extern_table)); /* Allocate memory for the new node */
  int i;
  for(i=0;i<10;i++){
   node->symbolName[i]=symbolName[i]; /* Copy the symbol name to the node */
  }
  node->Address_=Address_; /* Assign the provided address to the node */
  node->nextSymbolTable=NULL; /* Set the next symbol table pointer to NULL */
  return node; /* Return the newly created node */
}

/* Function to create a new entry table node */
struct entry_table * new_ent_table_node(char* symbolName,int Address_){
  struct entry_table * node=(struct entry_table *)malloc(sizeof(struct entry_table)); /* Allocate memory for the new node */
  int i;
  for(i=0;i<10;i++){
   node->symbolName[i]=symbolName[i]; /* Copy the symbol name to the node */
  }
  node->Address_=Address_; /* Assign the provided address to the node */
  node->nextSymbolTable=NULL; /* Set the next symbol table pointer to NULL */
  return node; /* Return the newly created node */
}

/* Function to create a new external symbol node */
symbol_ext * newexternSymbolNode(char* symbolName){
   symbol_ext * node=(symbol_ext *)malloc(sizeof(symbol_ext)); /* Allocate memory for the new node */
   int i;
   for(i=0;i<10;i++){
      node->symbolName[i]=symbolName[i]; /* Copy the symbol name to the node */
   }
   node->nextSymbol=NULL; /* Set the next symbol pointer to NULL */
   return node; /* Return the newly created node */
}

/* Function to create a new entry symbol node */
symbol_ent* newentrySymbolNode(char* symbolName){
     symbol_ent * node=(symbol_ent *)malloc(sizeof(symbol_ent)); /* Allocate memory for the new node */
     int i;
     for(i=0;i<10;i++){
        node->symbolName[i]=symbolName[i]; /* Copy the symbol name to the node */
     }
     node->nextSymbol=NULL; /* Set the next symbol pointer to NULL */
     return node; /* Return the newly created node */
}

/* Function to create a new macro table node */
Node_macro_Table *new_Table_Node(char* arr)
{
   Node_macro_Table *node = (Node_macro_Table *)malloc(sizeof(Node_macro_Table)); /* Allocate memory for the new node */
   int i;
   for (i = 0; i < 500; i++)
   {
      node->macros_name[i] = arr[i]; /* Copy the array to the node */
   }
   node->nextMacros = NULL; /* Set the next macros pointer to NULL */
   node->ptr_to_def = NULL; /* Set the pointer to definition to NULL */
   return node; /* Return the newly created node */
}

/* Function to create a new macro definition node */
Node_macro_def *newNode(char* brr)
{
   Node_macro_def *node = (Node_macro_def *)malloc(sizeof(Node_macro_def)); /* Allocate memory for the new node */
   int i;
   for (i = 0; i < 500; i++)
   {
      node->macros_arr[i] = brr[i]; /* Copy the array to the node */
   }
   node->next = NULL; /* Set the next pointer to NULL */
   return node; /* Return the newly created node */
}

/* Function to create a new symbol table node */
symbol_table * newSymbolTable(char* symbolName,int value){
    symbol_table * node=(symbol_table *)malloc(sizeof(symbol_table)); /* Allocate memory for the new node */
    int i;
    for(i=0;i<10;i++){
      node->symbolname[i]=symbolName[i]; /* Copy the symbol name to the node */
    }
    node->Address_=value; /* Assign the provided value to the node */
    node->pNextSymbol=NULL; /* Set the next symbol pointer to NULL */
    return node; /* Return the newly created node */
}
