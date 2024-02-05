#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* useful definitions */
#define AMOUNT_OF_TYPES 4
#define AMOUNT_OF_ADDRESSES 4
#define MAX_LINE_SIZE 80
#define MAX_LABEL_SIZE 31

#define MAX_POS_21 1048575
#define MAX_NEG_21 -1048576

#define MAX_POS_24 8388607
#define MAX_NEG_24 -8388608

/*macro for checking if malloc worked*/
#define mem_alloc_check(X)\
if(!X)\
{\
	printf("Error: could not allocate memmory\n");\
	exit(0);\
}\


enum{FALSE, TRUE};
enum{data_type, code_type, entry_type, external_type}; /*Symbol table types for labels*/
/* funcTable.c: functions table methods */
void tableConstructor();

int getopcode(char *);	  /* Getters: 			      */
int getfunct(char *);	  /* return specified value   */
int getOperands(char *);  /* on error: return -1 	  */
void getLegalDestinationAddresses(char *, int []);
void getLegalSourceAddresses(char *, int []);
int commandExists(char *); /* Returns True/False */
void freeFuncTable();
/***********************/


/*symbolsTable.c: a library to handle the dynamic allocation of the symbols in the assembly code */
void addSymbol(char *, int);
int getSymbolValue(char *); /* Returns -1 if entry does not exist. */
void getSymbolTypes(char *, int *);
void setSymbolValue(char *, int);
void fixSymbolTable(int);
void addType(char *, int);
int symbolExists(char *); /* Returns True/False*/
void freeSymbolsTable();
/*********************/

/*assemble.c: the main logic for the assembler*/
void assemble(char *, char *); /* Should recieve an array of an assembly file and a name
								  the files created on success would be of that name */
/********************/
