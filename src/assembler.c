/* Include necessary header files */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "assembler.h"
#include "macroFile.h"
#include "utilityFile.h"
#include "createNodes.h"

/* Global variables */
int count_line = 0;
int e_flag = 0;

/* Function to free memory allocated for entry symbols */
void freeSymbolEntMemory(symbol_ent * entptr){
   if(entptr == NULL) return;
   if(entptr->nextSymbol != NULL){
      freeSymbolEntMemory(entptr->nextSymbol);
   }
   free(entptr);
}

/* Function to free memory allocated for external symbols */
void freeSymbolExtMemory(symbol_ext * extptr){
   if(extptr == NULL) return;
   if(extptr->nextSymbol != NULL){
      freeSymbolExtMemory(extptr->nextSymbol);
   }
   free(extptr);
}

/* Function to free memory allocated for macro definitions */
void freeMacroDefMemory(Node_macro_def *ptrToDefination){
       if(ptrToDefination == NULL) return;
       if(ptrToDefination->next != NULL){
         freeMacroDefMemory(ptrToDefination->next);
       }
       free(ptrToDefination);
}

/* Function to free memory allocated for macro tables */
void freeMacrosTableMemory(Node_macro_Table *Macro_table_ptr){
   if(Macro_table_ptr == NULL) return;
   if(Macro_table_ptr->nextMacros != NULL){
      freeMacrosTableMemory(Macro_table_ptr->nextMacros);
   }
   if(Macro_table_ptr->ptr_to_def != NULL){
      freeMacroDefMemory(Macro_table_ptr->ptr_to_def);
   }
   free(Macro_table_ptr);
}

/* Function to free memory allocated for bit code nodes */
void freebitCodeNodeMemory(struct bitNode * ptr_bitCode_table){
   if(ptr_bitCode_table == NULL) return;
   if(ptr_bitCode_table->nextNode != NULL){
      freebitCodeNodeMemory(ptr_bitCode_table->nextNode);
   }
   free(ptr_bitCode_table);
}

/* Function to free memory allocated for external symbol tables */
void freeExtTableMemory(struct extern_table * ptr_ext_Table){
    if(ptr_ext_Table == NULL) return ;
    if(ptr_ext_Table->nextSymbolTable != NULL){
      freeExtTableMemory(ptr_ext_Table->nextSymbolTable);
    }
    free(ptr_ext_Table);
}

/* Function to free memory allocated for entry symbol tables */
void freeEntTableMemory(struct entry_table * ptr_ent_Table){
   if(ptr_ent_Table == NULL) return;
    if(ptr_ent_Table->nextSymbolTable != NULL){
      freeEntTableMemory(ptr_ent_Table->nextSymbolTable);
    }
    free(ptr_ent_Table);
}

/* Function to free memory allocated for symbol tables */
void freeTableSymbolMemory(symbol_table * start){
   if(start == NULL) return;
    if(start->pNextSymbol != NULL){
       freeTableSymbolMemory(start->pNextSymbol);
    }
    free(start);    
}

/* Function to create .am file */
void createAmFile(FILE *ptrRead, char* pFileName , Node_macro_Table *Macro_table_ptr, FILE ** file_am_ptr)
{
   char Line_number[499 + 1];
   int check = 0;
   while (fgets(Line_number, sizeof(Line_number), ptrRead))
   {
      if (check == 0)
      {
         int tokenCount = 0;
         char data[499 + 1];
         char *Token;
         strcpy(data, Line_number);
         Token = strtok(Line_number, " \t\n");
         if(data[0] == '\0' || data[0] == '\n') continue;
         while (Token != NULL && Token[0] != '\0' && Token[0] != '\n')
         {
            tokenCount++;
            if (strcmp(Token, "mcr") == 0)
            {
               check = 1;
               break;
            }
            Token = strtok(NULL, " \t\n");
         }

         if (check == 0)
         {
            if (tokenCount == 1)
            {  char *macro_arr;
               Node_macro_def *ptrtoDefination;
               macro_arr = strtok(data, " \t\n");
               ptrtoDefination = MacroCheck(macro_arr, Macro_table_ptr);
               if (ptrtoDefination != NULL)
               {
                  while (ptrtoDefination != NULL)
                  {
                     fprintf((*file_am_ptr),"%s",ptrtoDefination->macros_arr);
                     ptrtoDefination = ptrtoDefination->next;
                  }
               }
            }
            else
            {  
               fprintf((*file_am_ptr),"%s",data);
            }
         }
      }
      else
      {  char *Token;
         Token = strtok(Line_number, " \t\n");
         while (Token != NULL)
         {
            if (strcmp(Token, "endmcr") == 0)
            {
               check = 0;
               break;
            }
            Token = strtok(NULL, " \t\n");
         }
      }
   }
   rewind(*file_am_ptr);
}

/* Function: File_creation */
/* This function creates output files (.ob, .ext, .ent) based on the provided arguments. */
void File_creation(char * name_file,struct extern_table * ext_ptr,struct entry_table * ent_ptr,struct bitNode * ptr_ob,int instruction_bit, int data_bit){
     
    char filename_ob[20];
    char filename_ent[20];
    char filename_ext[20];
    FILE * file_ptr_ob;
    FILE * file_ptr_ext;
    FILE * file_ptr_ent;
    /* Create filenames for .ob, .ext, .ent files based on the input filename. */
    sprintf(filename_ext,"%s.ext",name_file);
    sprintf(filename_ent,"%s.ent",name_file);
    sprintf(filename_ob,"%s.ob",name_file);

    /* Open .ext, .ent, and .ob files for writing. */
    file_ptr_ext=fopen(filename_ext,"w");
    file_ptr_ent=fopen(filename_ent,"w");
    file_ptr_ob=fopen(filename_ob,"w");
    
    /* Write instruction_bit and data_bit to the .ob file. */
    fprintf(file_ptr_ob,"%02d %02d\n",instruction_bit,data_bit);

    /* Write external symbols and their addresses to the .ext file. */
    while(ext_ptr!=NULL){
      fprintf(file_ptr_ext,"%s %04d\n",ext_ptr->symbolName,ext_ptr->Address_);
      ext_ptr=ext_ptr->nextSymbolTable;
   }
   fclose(file_ptr_ext);
   /* Write machine code instructions and their addresses to the .ob file. */
   while(ptr_ob!=NULL){
    fprintf(file_ptr_ob,"%04d %s\n",ptr_ob->Address_,ptr_ob->encoding);
    ptr_ob=ptr_ob->nextNode;
   }
   fclose(file_ptr_ob);
   /* Write entry symbols and their addresses to the .ent file. */
   while(ent_ptr!=NULL){
      fprintf(file_ptr_ent,"%s %04d\n",ent_ptr->symbolName,ent_ptr->Address_);
      ent_ptr=ent_ptr->nextSymbolTable;
   }
   fclose(file_ptr_ent);  
}

/* Function: utility_node */
/* This utility function helps in creating and maintaining the bitNode linked list. */
void utility_node(struct bitNode ** begin,struct bitNode ** curr,int ** instruction_bit,int * Address_,char binary_in_encode[],char * Token){
      if((*begin)==NULL){
         (**instruction_bit)++;
         (*begin)=new_bit_Code_Node((*Address_)++,binary_in_encode,Token);
         (*curr)=(*begin);
         }
     else{
         (**instruction_bit)++;
         (*curr)->nextNode=new_bit_Code_Node((*Address_)++,binary_in_encode,Token);
         (*curr)=(*curr)->nextNode;
      }
}

/* Function: utility_encoding */
/* This utility function helps in encoding binary instructions based on given parameters. */
void utility_encoding(int Count, char binary_in_encode[],int binary_code){
       while(Count>=0){
          int res;
          res=binary_code&3;
          if(res==0){
            binary_in_encode[Count]='*';
          }
          else if(res==1){
            binary_in_encode[Count]='#';
          }
          else if(res==2){
            binary_in_encode[Count]='%';
          }
          else{
            binary_in_encode[Count]='!';
          }
          binary_code=binary_code>>2;
          Count--;
        }
}


/* 
   This function creates the machine code from the assembly file.
   It takes parameters:
   - file_am_ptr: Pointer to the assembly file
   - ptr_sym_table: Pointer to the symbol table
   - instruction_bit: Pointer to the number of instruction bits
   - data_bit: Pointer to the number of data bits
   - ext_ptr: Pointer to the external symbols table
   - ent_ptr: Pointer to the entry symbols table
   - table_org_ext: Pointer to the original external symbols table
   - table_org_ent: Pointer to the original entry symbols table
*/
struct bitNode * createMachineCode(FILE * file_am_ptr, symbol_table * ptr_sym_table,int * instruction_bit,int *data_bit,symbol_ext * ext_ptr,symbol_ent * ent_ptr,struct extern_table ** table_org_ext, struct entry_table ** table_org_ent){
   /* Declaration and initialization of variables */
   char Line_number[499 + 1];
   int Address_;
   int token_count;
   struct bitNode * begin=NULL;
   struct bitNode * curr=NULL;
   struct entry_table * ptr_ent_table=NULL;
   struct entry_table * ptr_curr_entTable=NULL;
   struct extern_table * ptr_ext_table=NULL;
   struct extern_table * ptr_curr_extTable=NULL;
   Address_=100;
   token_count=0;
   count_line=0;
   /* Reading each line from the assembly file */
   while(fgets(Line_number,sizeof(Line_number),file_am_ptr)){
      char * Tokens[10];
      char * Token;
      int ID=0;
      int op_Code;
      int binary_code;
      int Count;
      char binary_in_encode[8];
      count_line++;
      token_count=0;
      /* Tokenizing the line */
      Token=strtok(Line_number," \t");
      while(Token!=NULL && Token[0]!='\0' && Token[0]!='\n'){
         Tokens[token_count++]=Token;
         Token=strtok(NULL," \t\n");
      }
      /* Iterating through tokens */
      while(ID<token_count){
         /* If it's the first token in the line */
         if(ID==0){
            int LEN=0;
            /* Checking for directives or commands */
            if(strcmp(Tokens[ID],".extern")==0) break;
            else if(strcmp(Tokens[ID],".entry")==0) break;
            else if(strcmp(Tokens[ID],".define")==0){
               break;
            }
            else if(strchr(Tokens[ID],';')!=0) break;
            while(Tokens[ID][LEN]!='\0'){
               LEN++;
            }
            /* If it's not a label */
            if(Tokens[ID][LEN-1]!=':'){
               int id_cmd;
               id_cmd=0;
               /* Finding the command */
               while(strcmp(commands[id_cmd].pCmd,Tokens[ID])){
                           id_cmd++;
                           if(id_cmd>15){
                            printf("error in line %d: invalid command operator\n",count_line);
                            e_flag=1;
                            break;
                           }
               }
               /* Checking the number of operands for the command */
               if((check_command(Tokens[ID])) && ((token_count-ID-1)!=commands[id_cmd].number_of_operands)){
                  printf("error in line %d: incorrect arguments\n",count_line);
                  e_flag=1;
                }
               /* Encoding for commands with 0 operands */
               if(commands[id_cmd].number_of_operands==0){
                  op_Code=commands[id_cmd].cmd_number;
                  binary_code=op_Code<<6;
                  binary_in_encode[7]='\0';
                  Count=6;
                  utility_encoding(Count,binary_in_encode,binary_code);
                  utility_node(&begin,&curr,&instruction_bit,&Address_,binary_in_encode,Tokens[ID]);    
               }
               /* Encoding for commands with 1 operand */
               else if(commands[id_cmd].number_of_operands==1){
                  int destination_add_type;
                  op_Code=id_cmd;
                  destination_add_type=check_addressType(Tokens[ID+1]);
                  binary_code=(op_Code<<6)|(destination_add_type<<2);
                  binary_in_encode[7]='\0';
                  Count=6;
                  utility_encoding(Count,binary_in_encode,binary_code);
                  utility_node(&begin,&curr,&instruction_bit,&Address_,binary_in_encode,Tokens[ID]);
               }
               /* Encoding for commands with 2 operands */
               else if(commands[id_cmd].number_of_operands==2){
                  int source_add_type;
                  int destination_add_type;
                  op_Code=id_cmd;
                  source_add_type=check_addressType(Tokens[ID+1]);
                  destination_add_type=check_addressType(Tokens[ID+2]);
                  binary_code=(op_Code<<6)|(destination_add_type<<2)|(source_add_type<<4);
                  binary_in_encode[7]='\0';
                  Count=6;
                  utility_encoding(Count,binary_in_encode,binary_code);
                  utility_node(&begin,&curr,&instruction_bit,&Address_,binary_in_encode,Tokens[ID]);
               }
            }
         } 
         /* If it's not the first token */
         else{
            int i=0;
            char sym[20];
            int LEN=0;
            int iscommand=0;
            iscommand=check_command(Tokens[ID]);
            /* Checking if it's a command */
            while(Tokens[ID][LEN]!=',' && Tokens[ID][LEN]!='\0'){
                LEN++;
            }
            for(i=0;i<LEN;i++){
               sym[i]=Tokens[ID][i];
            }
            sym[LEN]='\0';
            if(iscommand){
               /* Checking the number of operands for the command */
               if((token_count-ID-1)!=commands[iscommand-1].number_of_operands){
                  printf("error in line %d: incorrect arguments\n",count_line);
                  e_flag=1;
               }
               /* Encoding for commands with 0 operands */
               if(commands[iscommand-1].number_of_operands==0){
                  op_Code=iscommand-1;
                  binary_code=op_Code<<6;
                  binary_in_encode[7]='\0';
                  Count=6;
                  utility_encoding(Count,binary_in_encode,binary_code);
                  utility_node(&begin,&curr,&instruction_bit,&Address_,binary_in_encode,Tokens[ID]);   
               }
               /* Encoding for commands with 1 operand */
               else if(commands[iscommand-1].number_of_operands==1){
                  int destination_add_type;
                  op_Code=iscommand-1;
                  destination_add_type=check_addressType(Tokens[ID+1]);
                  binary_code=(op_Code<<6)|(destination_add_type<<2);
                  binary_in_encode[7]='\0';
                  Count=6;
                  utility_encoding(Count,binary_in_encode,binary_code);
                  utility_node(&begin,&curr,&instruction_bit,&Address_,binary_in_encode,Tokens[ID]);
               }
               /* Encoding for commands with 2 operands */
               else if(commands[iscommand-1].number_of_operands==2){
                  int source_add_type;
                  int destination_add_type;
                  op_Code=iscommand-1;
                  destination_add_type=check_addressType(Tokens[ID+2]);
                  source_add_type=check_addressType(Tokens[ID+1]);
                  binary_code=(op_Code<<6)|(destination_add_type<<2)|(source_add_type<<4);
                  binary_in_encode[7]='\0';
                  Count=6;
                  utility_encoding(Count,binary_in_encode,binary_code);
                  utility_node(&begin,&curr,&instruction_bit,&Address_,binary_in_encode,Tokens[ID]);
               }
            }
            /* If it's a register */
            else if(check_register(Tokens[ID])){
                  int i=0;
                  char reg[3];
                  for(i=0;i<2;i++){
                     reg[i]=Tokens[ID][i];
                  }
                  reg[2]='\0';
                  /* If it's the last token */
                  if(ID+2==token_count){
                      if(Tokens[ID+1][0]=='r'){
                          int sourceReg;
                          int destReg;
                          sourceReg=Tokens[ID][1]-'0';
                          destReg=Tokens[ID+1][1]-'0';
                          sourceReg=sourceReg<<5;
                          destReg=destReg<<2;
                          binary_code=sourceReg|destReg;
                          ID++;
                      }
                      else{
                         int sourceReg;
                         sourceReg=Tokens[ID][1]-'0';
                         binary_code=sourceReg<<5;
                      }
                  }
                  /* If it's not the last token */
                  else{
                     int destReg;
                     destReg=Tokens[ID][1]-'0';
                     binary_code=destReg<<5;
                  }
                  binary_in_encode[7]='\0';
                  Count=6;
                  utility_encoding(Count,binary_in_encode,binary_code);
                  utility_node(&begin,&curr,&instruction_bit,&Address_,binary_in_encode,reg);
               }
            /* If it's a constant */
            else if(check_constant(Tokens[ID])){
                  if((Tokens[ID][1]>=48 && Tokens[ID][1]<=57)|| Tokens[ID][1]=='-'){
                      int digit_place;
                      int constant;
                      int LEN=0;
                      while(Tokens[ID][LEN]!='\0'){
                         LEN++;
                      }
                      digit_place=1;
                      constant=0;
                      if(Tokens[ID][1]=='-'){
                        int i;
                        i=LEN-1;
                           for(i=LEN-1;i>1;i--){
                          constant=constant+(Tokens[ID][i]-'0')*digit_place;
                          digit_place*=10;
                      }
                        constant=~constant+1;
                      }
                      else{
                        int i;
                        i=LEN-1;
                      for(i=LEN-1;i>0;i--){
                          constant=constant+(Tokens[ID][i]-'0')*digit_place;
                          digit_place*=10;
                      }}
                      binary_code=constant;
                  }
                  else{
                    int LEN=0;
                    int i;
                    char symbol[10];
                    
                    while(Tokens[ID][LEN]!='\0'){
                         LEN++;
                      }
                    i=1;
                    for(i=1;i<=LEN;i++){
                      symbol[i-1]=Tokens[ID][i];
                    }
                    symbol[LEN]='\0';
                    binary_code=fetch_const_data(symbol,ptr_sym_table);
                  }
                  binary_code=binary_code<<2;
                  
                  binary_in_encode[7]='\0';
                  Count=6;
                  utility_encoding(Count,binary_in_encode,binary_code);
                  
                  utility_node(&begin,&curr,&instruction_bit,&Address_,binary_in_encode,Tokens[ID]);
               }
               /* If it's an array object */
               else if(check_array_Obj(Tokens[ID])){
                  int idx=0;
                  char insideArray[10];
                  int total_len;
                  int entAdd;

                  int address_code;
                  int i=0;
                  char symbol[10];
                  int LEN=0;
          
                   while(Tokens[ID][LEN]!='['){
                      LEN++;
                   }
                   for(i=0;i<LEN;i++){
                      symbol[i]=Tokens[ID][i];
                   }
                   symbol[LEN]='\0';
                   if(check_In_symbolTable(symbol,ptr_sym_table)==0){
                     printf("error in line %d: invalid token\n",count_line);
                     e_flag=1;
                   }
                   address_code=address_symbol(symbol,ptr_sym_table);
                   entAdd=address_code;
                   address_code=address_code<<2;
                   binary_code=address_code|2;
                    binary_in_encode[7]='\0';
                   Count=6;
                   utility_encoding(Count,binary_in_encode,binary_code);
                   
                 utility_node(&begin,&curr,&instruction_bit,&Address_,binary_in_encode,Tokens[ID]);
                 
                 if(check_In_entryTable(symbol,ent_ptr)){
                     if(ptr_ent_table==NULL){
                        ptr_ent_table=new_ent_table_node(symbol,entAdd);
                        ptr_curr_entTable=ptr_ent_table;
                     }
                     else {
                       ptr_curr_entTable->nextSymbolTable=new_ent_table_node(symbol,entAdd);
                       ptr_curr_entTable=ptr_curr_entTable->nextSymbolTable;
                     }
                 }
                 else if(check_In_externTable(symbol,ext_ptr)){
                     if(ptr_ext_table==NULL){
                        ptr_ext_table=new_ext_table_node(symbol,entAdd);
                        ptr_curr_extTable=ptr_ext_table;
                     }
                     else {
                       ptr_curr_extTable->nextSymbolTable=new_ext_table_node(symbol,entAdd);
                       ptr_curr_extTable=ptr_curr_extTable->nextSymbolTable;
                     }
                 }
                 total_len=0;
                 
                 
                 if(ID+1==token_count){
                   int i;
                   while(Tokens[ID][total_len]!='\0'){
                      total_len++;
                   }
                   i=LEN+1;
                   for(i=LEN+1;i<total_len-1;i++){
                      insideArray[idx++]=Tokens[ID][i];
                   }
                 }
                 else{
                   int i;
                   while(Tokens[ID][total_len]!=','){
                      total_len++;
                   }
                   i=LEN+1;
                   for(i=LEN+1;i<total_len-1;i++){
                      insideArray[idx++]=Tokens[ID][i];
                   }
                 }
                 insideArray[idx]='\0';
                binary_code=fetch_const_data(insideArray,ptr_sym_table);
                if(binary_code==-1){
                   int num=0;
                   int i;
                   int digit_place;
                   if(!(insideArray[0]>='0' && insideArray[0]<='9')){
                     printf("error in line %d invalid token\n",count_line);
                     e_flag=1;
                   }
                   digit_place=1;
                   i=idx-1;
                   for(i=idx-1;i>=0;i--){
                      num=num+(insideArray[i]-'0')*digit_place;
                      digit_place*=10;
                   }
                   binary_code=num;
                }
                 binary_code=binary_code<<2;
                   Count=6;
                   utility_encoding(Count,binary_in_encode,binary_code);
                  
                 utility_node(&begin,&curr,&instruction_bit,&Address_,binary_in_encode,Tokens[ID]);
             }
             
           else if(check_In_symbolTable(sym,ptr_sym_table)){
                  int entAdd;
                 
                  entAdd=address_symbol(sym,ptr_sym_table);
                  binary_code=(entAdd<<2)|2;
                      binary_in_encode[7]='\0';
                      Count=6;
                    utility_encoding(Count,binary_in_encode,binary_code);
                    
                   utility_node(&begin,&curr,&instruction_bit,&Address_,binary_in_encode,Tokens[ID]);
                  if(check_In_entryTable(sym,ent_ptr)){
                       if(ptr_ent_table==NULL){
                          ptr_ent_table=new_ent_table_node(sym,entAdd);
                          ptr_curr_entTable=ptr_ent_table;
                       }
                       else {
                         ptr_curr_entTable->nextSymbolTable=new_ent_table_node(sym,entAdd);
                         ptr_curr_entTable=ptr_curr_entTable->nextSymbolTable;
                       }
                   }
                   else if(check_In_externTable(sym,ext_ptr)){
                       if(ptr_ext_table==NULL){
                          ptr_ext_table=new_ext_table_node(sym,entAdd);
                          ptr_curr_extTable=ptr_ext_table;
                       }
                       else {
                         ptr_curr_extTable->nextSymbolTable=new_ext_table_node(sym,entAdd);
                         ptr_curr_extTable=ptr_curr_extTable->nextSymbolTable;
                       }
                   }
                  
             }
             else if(strstr(Tokens[ID],".data")!=NULL){
                    ID++;
                    while(ID<token_count){
                        int i=0;
                        char binary_in_encode[8];
                        int Count;
                        char symbol[20];
                        int LEN=0;
                       
                        while(Tokens[ID][LEN]!='\0' && Tokens[ID][LEN]!=','){
                               LEN++;
                        }
                        for(i=0;i<LEN;i++){
                           symbol[i]=Tokens[ID][i];
                        }
                        symbol[LEN]='\0';
                        if(symbol[0]=='-'){
                           int constant=0;
                           int i;
                           int digit_place;
                           digit_place=1;
                           i=LEN-1;
                           for(i=LEN-1;i>0;i--){
                            constant=constant+(Tokens[ID][i]-'0')*digit_place;
                            digit_place*=10;
                        }
                          constant=~constant+1;
                          binary_code=constant;
                        }
                        else if(symbol[0]>=48 && symbol[0]<=57){
                           int digit_place;
                           int const_num;
                           int i;
                           digit_place=1;
                           const_num=0;
                           i=LEN-1;
                           for(i=LEN-1;i>=0;i--){
                              const_num=const_num+((symbol[i]-'0')*digit_place);
                              digit_place*=10;
                           }
                           binary_code=const_num;
                        }
                        else{
                           binary_code=fetch_const_data(symbol,ptr_sym_table);
                        }
                        
                      binary_in_encode[7]='\0';
                      Count=6;
                      utility_encoding(Count,binary_in_encode,binary_code);
                    
                   utility_node(&begin,&curr,&data_bit,&Address_,binary_in_encode,Tokens[ID]);
                      ID++;
                    }
               }
               else if(strstr(Tokens[ID],".string")!=NULL){
                  ID++;
                 while(ID<token_count){
                    char data_string[99+1];
                    int LEN=0;
                    int i;
                    while(Tokens[ID][LEN]!='\0'){
                     LEN++;
                    }
                    i=1;
                   for(i=1;i<LEN-1;i++){
                     data_string[i-1]=Tokens[ID][i];
                   }
                   data_string[LEN-2]='\0';
                   for(i=0;i<LEN-1;i++){
                     char binary_in_encode[8];
                     int Count;
                     int binary_code;
                    
                      binary_in_encode[7]='\0';
                      binary_code=data_string[i];
                      Count=6;
                      utility_encoding(Count,binary_in_encode,binary_code);
                    
                   if(begin==NULL){
                     char d[2];
                     d[0]=data_string[i];
                     d[1]='\0';
                     (*data_bit)++;
                     begin=new_bit_Code_Node(Address_++,binary_in_encode,d);
                     curr=begin;
                   }
                   else{
                     char d[2];
                     d[0]=data_string[i];
                     d[1]='\0';
                     (*data_bit)++;
                     curr->nextNode=new_bit_Code_Node(Address_++,binary_in_encode,d);
                     curr=curr->nextNode;
                   }
                   }
                   ID++;
                 }
               }
               else if(check_In_entryTable(sym,ent_ptr)){
                  int entAdd;
                 
                  entAdd=address_symbol(sym,ptr_sym_table);
                  binary_code=(entAdd<<2)|2;
                     
                     Count=6;
                     utility_encoding(Count,binary_in_encode,binary_code);
                    
                   utility_node(&begin,&curr,&instruction_bit,&Address_,binary_in_encode,Tokens[ID]);
                  if(ptr_ent_table==NULL){
                          ptr_ent_table=new_ent_table_node(sym,Address_-1);
                          ptr_curr_entTable=ptr_ent_table;
                       }
                       else {
                         ptr_curr_entTable->nextSymbolTable=new_ent_table_node(sym,Address_-1);
                         ptr_curr_entTable=ptr_curr_entTable->nextSymbolTable;
                       }
               }
               else if(check_In_externTable(sym,ext_ptr)){
                    
                     Count=6;
                     binary_code=1;
                     utility_encoding(Count,binary_in_encode,binary_code);
                     
                  utility_node(&begin,&curr,&instruction_bit,&Address_,binary_in_encode,Tokens[ID]);
                  if(ptr_ext_table==NULL){
                          ptr_ext_table=new_ext_table_node(sym,Address_-1);
                          ptr_curr_extTable=ptr_ext_table;
                       }
                       else {
                         ptr_curr_extTable->nextSymbolTable=new_ext_table_node(sym,Address_-1);
                         ptr_curr_extTable=ptr_curr_extTable->nextSymbolTable;
                       }
               }
               else if(!((sym[0]>='0' && sym[0]<='9') || (sym[0]=='-'))){
                 printf("error in line %d: invalid token\n",count_line);
                 e_flag=1;
               }
             }
      ID++;
      }
   }
   (*table_org_ext)=ptr_ext_table;
   (*table_org_ent)=ptr_ent_table;
   return begin;
}
/*
 * Initializing an array of structures to represent assembly commands along with their attributes.
 */
Command commands[] =
        {{"mov", 2, 1}, 
	{"cmp", 2, 2}, 
	{"add", 2, 3}, 
	{"sub", 2, 4}, 
	{"not", 1, 5}, 
	{"clr", 1, 6}, 
	{"lea", 2, 7}, 
	{"inc", 1, 8}, 
	{"dec", 1, 9},
	{"jmp", 1, 10}, 
	{"bne", 1, 11}, 
	{"red", 1, 12}, 
	{"prn", 1, 13}, 
	{"jsr", 1, 14}, 
	{"rts", 0, 15}, 
	{"hlt", 0, 16}};

/*
 * The main function that processes assembly files and generates machine code files.
 */
int main(int argc, char** arg )
{  
   /*
    * Declaring and initializing variables for file processing.
    */
   int i;
   i=1;
   /*
    * Looping through command-line arguments, assuming each argument is a filename without extension.
    */
   while(i<argc){
   char name_file[20];
   char name_am_file[20];
   FILE *file_ptr;
   FILE *file_am_ptr;
   Node_macro_Table *Macro_table_ptr = NULL;
   symbol_ext * ext_ptr=NULL;
   symbol_ent * ent_ptr=NULL;
   int data_bit=0;
   int instruction_bit=0;
   struct bitNode * ptr_bitCode_table;
   struct entry_table * ptr_ent_table=NULL;
   struct extern_table * ptr_ext_table=NULL;
   symbol_table * begin;
   /*
    * Generating file names for assembly and machine code files.
    */
   sprintf(name_file, "%s.as", arg[i]);
   sprintf(name_am_file, "%s.am", arg[i]);
   /*
    * Opening the assembly file for reading.
    */
   file_ptr = fopen(name_file, "r");
   if(file_ptr==NULL){
      printf("Error in opening %s\n",name_file);
      return -1;
   }
   /*
    * Extracting macros from the assembly file (if any).
    */
   if (file_ptr != NULL)
   {
      Macro_table_ptr = extracting_macros(file_ptr);
   }
   /*
    * Rewinding the file pointer to the beginning of the file.
    */
   rewind(file_ptr);
   
   /*
    * Opening a new machine code file for writing.
    */
   file_am_ptr=fopen(name_am_file,"w+");
   if(file_am_ptr==NULL){
      printf("Error in opening %s\n",name_am_file);
      return -1;
   }
   /*
    * Generating machine code file based on the assembly file and extracted macros.
    */
   createAmFile(file_ptr,arg[i],Macro_table_ptr,&file_am_ptr);
   /*
    * Creating symbol table based on the assembly file.
    */
   begin=createSymTable(&ext_ptr,&ent_ptr,file_am_ptr,&count_line,&e_flag);
   /*
    * Generating machine code based on the symbol table and other necessary parameters.
    */
   if(e_flag==0){
   ptr_bitCode_table=createMachineCode(file_am_ptr,begin,&instruction_bit,&data_bit,ext_ptr,ent_ptr,&ptr_ext_table,&ptr_ent_table);
   }
   /*
    * Closing the machine code file.
    */
   fclose(file_am_ptr);
   /*
    * If no errors occurred during the process, creating output files and freeing allocated memory.
    */
   if(e_flag==0){
    File_creation(arg[i],ptr_ext_table,ptr_ent_table,ptr_bitCode_table,instruction_bit,data_bit);
   }
   /*
    * Moving to the next command-line argument.
    */
   i++;
   /*
    * Freeing memory allocated for various data structures.
    */
   freeSymbolEntMemory(ent_ptr);
   freeSymbolExtMemory(ext_ptr);
   freeMacrosTableMemory(Macro_table_ptr);
   freebitCodeNodeMemory(ptr_bitCode_table);
   freeExtTableMemory(ptr_ext_table);
   freeEntTableMemory(ptr_ent_table);
   freeTableSymbolMemory(begin);
   /*
    * Closing the assembly file.
    */
   fclose(file_ptr);
   }
   /*
    * Returning 0 to indicate successful execution.
    */
   return 0;
}
