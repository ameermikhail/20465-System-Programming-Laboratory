#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include "utilityFile.h"
#include "assembler.h"
#include "macroFile.h"

int check_array_Obj(char * arrayObj){
   /* Function to check if the given string represents an array object */
   if(strchr(arrayObj,'[')!=NULL && strchr(arrayObj,']')!=NULL) return 1; /* Return 1 if both '[' and ']' characters are found */
   return 0; /* Otherwise, return 0 */
}

int fetch_const_data(char* symbol,symbol_table * ptr_sym_table){
   /* Function to fetch constant data from the symbol table */
   while(ptr_sym_table!=NULL){
      if(strcmp(symbol,ptr_sym_table->symbolname)==0) return ptr_sym_table->Address_; /* Return the address if the symbol matches */
      ptr_sym_table=ptr_sym_table->pNextSymbol; /* Move to the next symbol table entry */
   }
   return -1; /* Return -1 if symbol not found */
}

int check_In_symbolTable(char * Token,symbol_table * pointer){
   /* Function to check if a token is present in the symbol table */
   while(pointer!=NULL){
      if(strcmp(Token,pointer->symbolname)==0){
         return 1; /* Return 1 if the token matches a symbol in the table */
      }
      pointer=pointer->pNextSymbol; /* Move to the next symbol table entry */
   }   
   return 0; /* Return 0 if token not found */
}

int address_symbol(char * symbol,symbol_table * pointer){
   /* Function to get the address of a symbol from the symbol table */
   while(pointer!=NULL){
      if(strcmp(symbol,pointer->symbolname)==0){
         return pointer->Address_; /* Return the address if the symbol matches */
      }
      pointer=pointer->pNextSymbol; /* Move to the next symbol table entry */
   }
   return 0; /* Return 0 if symbol not found */
}

int check_In_entryTable(char* symbol,symbol_ent * pointer){
   /* Function to check if a symbol is present in the entry table */
   while(pointer!=NULL){
      if(strcmp(symbol,pointer->symbolName)==0) return 1; /* Return 1 if the symbol matches an entry */
      pointer=pointer->nextSymbol; /* Move to the next entry table entry */
   }
   return 0; /* Return 0 if symbol not found */
}

int check_addressType(char * data){
   /* Function to check the address type of data */
   if(strchr(data,'r')!=NULL) return 3; /* Return 3 if 'r' is found, indicating register */
   if(strchr(data,'[')!=NULL) return 2; /* Return 2 if '[' is found, indicating memory address */
   if(strchr(data,'#')!=NULL) return 0; /* Return 0 if '#' is found, indicating immediate value */
   return 1; /* Return 1 if none of the above conditions are met, indicating a label */
}

int check_In_externTable(char* symbol,symbol_ext * pointer){
   /* Function to check if a symbol is present in the external table */
   while(pointer!=NULL){
      if(strcmp(pointer->symbolName,symbol)==0) return 1; /* Return 1 if the symbol matches an external entry */
      pointer=pointer->nextSymbol; /* Move to the next external table entry */
   }
   return 0; /* Return 0 if symbol not found */
}

int check_command(char * data){
   /* Function to check if the given data represents a command */
   int j;
   j=0;
   while(j<16){
      if(strcmp(data,commands[j].pCmd)==0) return commands[j].cmd_number; /* Return the command number if data matches a command */
      j++;
   }
   return 0; /* Return 0 if command not found */
}

int check_constant(char * check){
   /* Function to check if the given data represents a constant */
   if(check[0]=='#') return 1; /* Return 1 if data starts with '#' */
   return 0; /* Return 0 otherwise */
}

int check_register(char * reg){
   /* Function to check if the given string represents a register */
   if(reg[0]=='r') return 1; /* Return 1 if data starts with 'r' */
   return 0; /* Return 0 otherwise */
}


symbol_table * createSymTable(symbol_ext ** ptr_EXT, symbol_ent ** ptr_ENT,FILE *file_am_ptr,int * count_line, int * e_flag){
   /* Function to create symbol table */
   symbol_ent * ptr_ent_curr=NULL;
   symbol_ext * ptr_ext_curr=NULL;
   symbol_ent * ent_ptr=NULL;
   symbol_ext * ext_ptr=NULL;
   symbol_table * symbol_table_ptr_curr=NULL;
   symbol_table * symbol_table_ptr=NULL;
   char Lines[500];
   int Pointer;
   Pointer=99;
   (*count_line)=0;
   /* Read each line from the file */
   while(fgets(Lines,sizeof(Lines),file_am_ptr)){
      char * Token;
      char newLines[100 + 1];
      strcpy(newLines, Lines);
      (*count_line)++;
      Token=strtok(newLines,"\t ");
      /* Ignore lines starting with ';' */
      if(strchr(Token,';')!=NULL) continue;
      if(strcmp(Token,".extern")==0){
         char name_sym[20];
         int j;
         int length=0;
         Token=strtok(NULL,"\t ");
         if(Token==NULL){
            printf("error in line %d: defination is incorrect\n",(*count_line));
            (*e_flag)=1;
          }
         while(Token[length]!='\n'){
            length++;
         }
         j=0;
         while(j<length){
            name_sym[j]=Token[j];
            j++;
         }
         name_sym[length]='\0';
         if(ext_ptr==NULL){
            ext_ptr=newexternSymbolNode(name_sym);
            ptr_ext_curr=ext_ptr;
          }
          else{
            ptr_ext_curr->nextSymbol=newexternSymbolNode(name_sym);
            ptr_ext_curr=ptr_ext_curr->nextSymbol;
          }
          Token=strtok(NULL,"\t ");
          if(Token!=NULL && Token[0]!='\n'){
            printf("error in line %d: defination is incorrect\n",(*count_line));
            (*e_flag)=1;
          }
         }
         else if(strcmp(Token,".entry")==0){
         int length=0;
         char name_sym[20];
         int j;
         Token=strtok(NULL,"\t ");
         if(Token==NULL){
            printf("error in line %d: defination is incorrect\n",(*count_line));
            (*e_flag)=1;
          }
         while(Token[length]!='\n'){
            length++;
         }
         j=0;
         while(j<length){
            name_sym[j]=Token[j];
            j++;
         }
         name_sym[length]='\0';
         if(ent_ptr==NULL){
            ent_ptr=newentrySymbolNode(name_sym);
            ptr_ent_curr=ent_ptr;
          }
          else{
            ptr_ent_curr->nextSymbol=newentrySymbolNode(name_sym);
            ptr_ent_curr=ptr_ent_curr->nextSymbol;
          }
          Token=strtok(NULL,"\t ");
          if(Token!=NULL && Token[0]!='\n'){
            printf("error in line %d: defination is incorrect\n",(*count_line));
            (*e_flag)=1;
          }
         }
      else if(strcmp(Token,".define")==0){
          int j;
          int pos;
          int val=0;
          int length=0;
          char name_sym[20];
         
          Token=strtok(NULL,"\t ");
          strcpy(name_sym,Token);
          strtok(NULL,"\t ");
          Token=strtok(NULL,"\t ");
          while(Token[length]!='\n' && Token[length]!='\0'){
            length++;
          }
          pos=1;
          j=length-1;
          while(j>=0){
            val=val+(Token[j]-'0')*pos;
               pos*=10;
               j--;
          }
          
          if(symbol_table_ptr==NULL){
            symbol_table * temp_ptr= newSymbolTable(name_sym,val);
            if(check_In_symbolTable(name_sym,symbol_table_ptr)){
              printf("error in line %d: statement already declared\n",(*count_line));
            }
            symbol_table_ptr=temp_ptr;
            symbol_table_ptr_curr=temp_ptr;
          }
          else{
            if(check_In_symbolTable(name_sym,symbol_table_ptr)){
              printf("error in line %d: statement already declared\n",(*count_line));
            }
            symbol_table_ptr_curr->pNextSymbol=newSymbolTable(name_sym,val);
            symbol_table_ptr_curr=symbol_table_ptr_curr->pNextSymbol;
          }
          Token=strtok(NULL,"\t ");
          if(Token!=NULL && Token[0]!='\n'){
            printf("error in line %d: defination is incorrect\n",(*count_line));
            (*e_flag)=1;
          }
          } 
        else if(strchr(Token,':')!=NULL){
           char name_sym[20];
           int length=0;
           int j;
           while(Token[length]!=':'){
            length++;
           }
           j=0;
           while(j<length){
               name_sym[j]=Token[j];
               j++;
           }
           name_sym[length]='\0';
         if(check_In_symbolTable(name_sym,symbol_table_ptr)){
              printf("error in line %d: is already defined above\n",(*count_line));
              (*e_flag)=1;
            }
          if(symbol_table_ptr==NULL){
            symbol_table_ptr=newSymbolTable(name_sym,Pointer+1);
            symbol_table_ptr_curr=symbol_table_ptr;
          }
          else{
            symbol_table_ptr_curr->pNextSymbol=newSymbolTable(name_sym,Pointer+1);
            symbol_table_ptr_curr=symbol_table_ptr_curr->pNextSymbol;
          }
          Token=strtok(NULL,"\t ");
          while(Token!=NULL){
              if(strchr(Token,'[')!=NULL || strchr(Token,']')!=NULL){
               if(!(strchr(Token,'[')!=NULL && strchr(Token,']')!=NULL)){
                  printf("error in line %d: syntax error\n",(*count_line));
                  (*e_flag)=1;
                }
               Pointer+=2;
              }
              else if(Token[0]=='r'){
                  if(Token[1]>='8' || (Token[2]>='0' && Token[2]<='9')){
                    printf("error in line %d: register is incorrect\n",(*count_line));
                      (*e_flag)=1;
                  }
                  Token=strtok(NULL,"\t ");
                  if(Token==NULL){
                     Pointer++;
                     break;
                  }
                  if(Token[0]=='r'){
                    if(Token[1]>='8' || (Token[2]>='0' && Token[2]<='9')){
                      printf("error in line %d: register is incorrect\n",(*count_line));
                      (*e_flag)=1;
                    }
                    Pointer++;
                  }
                  else{
                     if(strchr(Token,'[')!=NULL || strchr(Token,']')!=NULL){
                        if(!(strchr(Token,'[')!=NULL && strchr(Token,']')!=NULL)){
                          printf("error in line %d: syntax error\n",(*count_line));
                          (*e_flag)=1;
                        }
                        Pointer+=3;
                     }
                     else{
                        Pointer+=2;
                     }
                  }
              }
              else if(strstr(Token,".string")!=NULL){
                 int length=0;
                 Token=strtok(NULL," \t\n");
                 while(Token[length]!='\0' && Token[length]!='\n'){
                     length++;
                 }
               Pointer+=length-1;
              }
              else if(strstr(Token,".data")!=NULL){
                  Token=strtok(NULL," \t");
                  while(Token!=NULL){
                       Pointer++;
                       Token=strtok(NULL," \t\n");
                  }
              }
              else if(Token[0]!='\n') Pointer++;
              Token=strtok(NULL,"\t ");
          }
       }
       else{
          while(Token!=NULL){
              if(strchr(Token,'[')!=NULL || strchr(Token,']')!=NULL){
               if(!(strchr(Token,'[')!=NULL && strchr(Token,']')!=NULL)){
                  printf("error in line %d: syntax error\n",(*count_line));
                  (*e_flag)=1;
                }
               Pointer+=2;
              }
              else if(Token[0]=='r'){
                  if(Token[1]>='8' || (Token[2]>='0' && Token[2]<='9')){
                    printf("error in line %d: register is incorrect\n",(*count_line));
                      (*e_flag)=1;
                  }
                  Token=strtok(NULL,"\t ");
                  if(Token==NULL){
                     Pointer++;
                     break;
                  }
                  if(Token[0]=='r'){
                    if(Token[1]>='8' || (Token[2]>='0' && Token[2]<='9')){
                      printf("error in line %d: register is incorrect\n",(*count_line));
                      (*e_flag)=1;
                    }
                    Pointer++;
                  }
                  else{
                     if(strchr(Token,'[')!=NULL || strchr(Token,']')!=NULL){
                        if(!(strchr(Token,'[')!=NULL && strchr(Token,']')!=NULL)){
                          printf("error in line %d: syntax error\n",(*count_line));
                          (*e_flag)=1;
                        }
                        Pointer+=3;
                     }
                     else{
                        Pointer+=2;
                     }
                  }
              }
              else if(strstr(Token,".string")!=NULL){
                 int length=0;
                 Token=strtok(NULL," \t\n");
                 while(Token[length]!='\0'){
                     length++;
                 }
               Pointer+=length-1;
              }
              else if(strstr(Token,".data")!=NULL){
                  Token=strtok(NULL," \t");
                  while(Token!=NULL){
                       Pointer++;
                       Token=strtok(NULL," \t\n");
                  }
              }
              else if(Token[0]!='\n') Pointer++;
              Token=strtok(NULL,"\t ");
          }
       }
   }
    rewind(file_am_ptr);
    (*ptr_EXT)=ext_ptr;
    (*ptr_ENT)=ent_ptr;
    return symbol_table_ptr;
}








