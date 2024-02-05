#include "./headers/utils.h"
#include "./headers/func_table_header.h"



/* The table constructor, must be run when assembly starts
 * creates all the nodes of the functions table
 * 
 * New instructions can be added to the table, but make sure all of the arrays are of the same size
 * (all the nodes must be filled for every new instruction)
 * 
 * the table is stored in funcTableHead
 * */
void tableConstructor()
{
	char *names_t[] = {"mov","cmp","add","sub","lea",
						"clr","not","inc","dec","jmp","bne","jsr","red","prn",
						"rts","stop"};
	int opcodes_t[] = {0, 1, 2, 2, 4,
						5, 5, 5, 5, 9, 9, 9, 12, 13,
						14, 15};
	int functs_t[] = {0, 0, 1, 2, 0, 
						1, 2, 3, 4, 1, 2, 3, 0, 0,
						0, 0};
	int operands_t[] = {2, 2, 2, 2, 2,
						1, 1, 1, 1, 1, 1, 1, 1, 1,
						0, 0};
	int sourceAddresses[16][4] = {{1,1,0,1}, {1,1,0,1}, {1,1,0,1}, {1,1,0,1}, {0,1,0,0}, 
								  {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0},
								  {0,0,0,0}, {0,0,0,0}};
	int targetAddresses[16][4] = {{0,1,0,1}, {1,1,0,1}, {0,1,0,1}, {0,1,0,1}, {0,1,0,1}, 
								{0,1,0,1}, {0,1,0,1}, {0,1,0,1}, {0,1,0,1}, {0,1,1,0}, {0,1,1,0}, {0,1,1,0}, {0,1,0,1}, {1,1,0,1},
								{0,0,0,0}, {0,0,0,0}};
								
	funcNodePtr * ptrToHead = &funcTableHead;
	funcNodePtr ins_ptr;
	funcNodePtr temp;
	funcNodePtr prev;
	
	int i,k;
	
	/*creates the table by the number of different opcodes*/
	for(i = 0; i < (sizeof(opcodes_t)/sizeof(opcodes_t[0])); i++)
	{
		ins_ptr = (funcNodePtr) malloc(sizeof(instruction));
		mem_alloc_check(ins_ptr);
		strcpy(ins_ptr->name,names_t[i]);
		ins_ptr->opcode = opcodes_t[i];
		ins_ptr->funct = functs_t[i];
		ins_ptr->operands = operands_t[i];
		for(k = 0; k < AMOUNT_OF_ADDRESSES; k++)
		{
			ins_ptr->sourceAddressing[k] = sourceAddresses[i][k];
			ins_ptr->targetAddressing[k] = targetAddresses[i][k];
		}
		ins_ptr->next = NULL;
		temp = *ptrToHead;
		
		while(temp)
		{
			prev = temp;
			temp = temp->next;
		}
		if(temp == *ptrToHead)
			*ptrToHead = ins_ptr;
		else
			prev->next = ins_ptr;
	}
}

/* Function recieves an instructions name
 * Returns 1 if the instruction exists in the table
 * Returns 0 if not
 * */
int commandExists(char * name)
{
	funcNodePtr * ptrToHead = &funcTableHead;
	funcNodePtr ptr = *ptrToHead;
	while(ptr)
	{
		if(!strcmp(ptr->name, name))
			return TRUE;
		ptr = ptr->next;
	}
	return FALSE;
}

/* Function recieves an instructions name and an integer array to store legal addresses
 * array is of size 4
 * fills the given array with the instructions sourceAddressing array*/
void getLegalSourceAddresses(char * name, int * arr)
{
	funcNodePtr * ptrToHead = &funcTableHead;
	funcNodePtr ptr = *ptrToHead;
	int i;
	while(ptr)
	{
		if(!strcmp(ptr->name, name))
		{
			for(i = 0; i < AMOUNT_OF_ADDRESSES; i++)
				arr[i] = (ptr->sourceAddressing)[i];
			return;
		}
		ptr = ptr->next;
	}
	printf("Error: funcTable.c getLegalSourceAddresses(char*,int*) could not find %s\n",name);
}

/* Function recieves an instructions name and an integer array to store legal addresses
 * array is of size 4
 * fills the given array with the instructions targetAddressing array*/
void getLegalDestinationAddresses(char * name, int * arr)
{
	funcNodePtr * ptrToHead = &funcTableHead;
	funcNodePtr ptr = *ptrToHead;
	int i;
	while(ptr)
	{
		if(!strcmp(ptr->name, name))
		{
			for(i = 0; i < AMOUNT_OF_ADDRESSES; i++)
				arr[i] = (ptr->targetAddressing)[i];
			return;
		}
		ptr = ptr->next;
	}
	printf("Error: funcTable.c getLegalDestinationAddresses(char*,int*) could not find %s\n",name);
}


/* Function recieves an instructions name and returns the value of its operands field
 * if instruction not found returns -1*/
int getOperands(char * name)
{
	funcNodePtr * ptrToHead = &funcTableHead;
	funcNodePtr ptr = *ptrToHead;
	while(ptr)
	{
		if(!strcmp(ptr->name, name))
			return ptr->operands;
		ptr = ptr->next;
	}
	return -1;
}

/* Function recieves an instructions name and returns the value of its funct field
 * if instruction not found return -1*/
int getfunct(char * name)
{
	funcNodePtr * ptrToHead = &funcTableHead;
	funcNodePtr ptr = *ptrToHead;
	while(ptr)
	{
		if(!strcmp(ptr->name, name))
			return ptr->funct;
		ptr = ptr->next;
	}
	return -1;
}

/* Function recieves an instructions name and returns the value of its opcode field
 * if instruction not found return -1*/
int getopcode(char * name)
{
	funcNodePtr * ptrToHead = &funcTableHead;
	funcNodePtr ptr = *ptrToHead;
	while(ptr)
	{
		if(!strcmp(ptr->name, name))
			return ptr->opcode;
		ptr = ptr->next;
	}
	return -1;
}

/* Function to free allocated memmory */
void freeFuncTable()
{
	funcNodePtr * ptrToHead = &funcTableHead;
	funcNodePtr ptr;
	while(*ptrToHead)
	{
		ptr = *ptrToHead;
		*ptrToHead = (*ptrToHead)->next;
		free(ptr);
	}
}
