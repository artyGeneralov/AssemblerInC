#include "./headers/utils.h"
#include "./headers/symbols_table_header.h"

/* Functino to add a symbol to a symbols table
 * Recieves the name and value as parameters
 * The symbol types are initialized to 0*/
void addSymbol(char * name, int value)
{
	symbolNodePtr * pointerToHead;
	symbolNodePtr symbolPointer;
	symbolNodePtr temp;
	symbolNodePtr prev;
	pointerToHead = &symbolTableHead;
	int i;
	
	symbolPointer = (symbolNodePtr) malloc(sizeof(symbol));
	mem_alloc_check(symbolPointer);
	strcpy(symbolPointer->name,name);
	symbolPointer->value = value;
	for(i = 0; i < AMOUNT_OF_TYPES; i++)
		(symbolPointer->types)[i] = 0;
	symbolPointer->next = NULL;
	
	temp = *pointerToHead;
	while(temp)
	{
		
		prev = temp;
		temp = temp->next;
	}
	if(temp == *pointerToHead)
		*pointerToHead = symbolPointer;
	else
		prev->next = symbolPointer;
}

/* Function recieves a symbol name and a type,
 * turns on the corresponding flag in the symbols types array */
void addType(char * name, int type)
{
	symbolNodePtr * pointerToHead = &symbolTableHead;
	symbolNodePtr ptr = *pointerToHead;
	while(ptr)
	{
		if(!strcmp(name,ptr->name))
		{
			ptr->types[type] = 1;
			return;
		}
		ptr = ptr->next;
	}
	printf("Error: symbolTypes.c addType(char*,int) could not find symbol %s\n", name);
}

/* Function that adds the value n to the value of all data type symbols
 * to be used after the assemblers first pass */
void fixSymbolTable(int n)
{	
	symbolNodePtr * pointerToHead  = &symbolTableHead;
	symbolNodePtr ptr = *pointerToHead;
	while(ptr)
	{
		if(ptr->types[data_type] == 1)
			ptr->value += n;
		ptr = ptr->next;
	}
}

/* Function recieves a symbol name and a new value for that symbol */
void setSymbolValue(char * name, int value)
{
	symbolNodePtr * pointerToHead = &symbolTableHead;
	symbolNodePtr ptr = *pointerToHead;
	while(ptr)
	{
		if(!strcmp(ptr->name, name))
		{
			ptr->value = value;
			return;
		}
	}
	printf("Error: symbolTypes.c setSymbolValue(char*,int) could not find symbol %s\n", name);
}

/*Function recieves a symbol name and a types array and fills the array with the symbols types*/
void getSymbolTypes(char * name, int * types)
{
	int i;
	symbolNodePtr * pointerToHead = &symbolTableHead;
	symbolNodePtr ptr = *pointerToHead;
	while(ptr)
	{
		if(!strcmp(ptr->name,name))
		{
			for(i = 0; i < AMOUNT_OF_TYPES; i++)
				types[i] = ptr->types[i];
			return;
		}
		ptr = ptr->next;
	}
	printf("Error: symbolTypes.c getSymbolTypes(char*,int) could not find symbol %s\n", name);
}

/* Function recieves a symbols name and returns its value */
int getSymbolValue(char * name)
{
	symbolNodePtr * pointerToHead = &symbolTableHead;
	symbolNodePtr ptr = *pointerToHead;
	while(ptr)
	{
		if(!strcmp(name, ptr->name))
			return ptr->value;
		ptr = ptr->next;
	}
	printf("Error: symbolsTable.c getSymbolValue(char *), symbol %s does not exist.\n", name);
	return -1;
}

/* Function recieves a symbols name and returns 1 if it exists in the symbols table and 0 if not*/
int symbolExists(char * name)
{
	symbolNodePtr * pointerToHead = &symbolTableHead;
	symbolNodePtr ptr = *pointerToHead;
	while(ptr)
	{
		if(!strcmp(name, ptr->name))
			return TRUE;
		ptr = ptr->next;
	}
	return FALSE;
}

/* Function to free allocated memmory */
void freeSymbolsTable()
{
	symbolNodePtr * ptrToTable = &symbolTableHead;
	symbolNodePtr ptr;
	while(*ptrToTable)
	{
		ptr = *ptrToTable;
		*ptrToTable = (*ptrToTable)->next;
		free(ptr);
	}
}

