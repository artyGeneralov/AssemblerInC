typedef struct node * funcNodePtr;
typedef struct node{
	char name[MAX_LINE_SIZE]; /* MAX LINE SIZE = 80*/
	int opcode,funct, operands;
	int sourceAddressing[AMOUNT_OF_ADDRESSES]; /* AMOUNT OF ADDRESSES = 4*/
	int targetAddressing[AMOUNT_OF_ADDRESSES];
	funcNodePtr next;
}instruction;

funcNodePtr funcTableHead = NULL;
