#include <ctype.h>

enum{LABEL, DAT, ENTRY, EXTERN, COMMAND}; /*types of possible words that can start a line*/
enum{A = 4, R = 2, E = 1};
/*The struct for the data image: */
typedef struct tableNode * imgPtr;
typedef struct tableNode{
	int encodedValue;
	imgPtr next;
} node;

/*A struct to hold the names and values for the entry and externals files*/
typedef struct ent_table * nt_ptr;
typedef struct ent_table{
	char name[MAX_LABEL_SIZE];
	int value;
	nt_ptr next;
} nt_node;

/*Instruction encoding*/
struct{
	unsigned int E:1;
	unsigned int R:1;
	unsigned int A:1;
	unsigned int funct:5;
	unsigned int destReg:3;
	unsigned int destAdd:2;
	unsigned int sourceReg:3;
	unsigned int sourceAdd:2;
	unsigned int opcode:6;
} instruction; /* a variable "instruction". Its values would be re-assigned each time an instruction (command) line is evaluated*/

/* Prototype Start */
void firstPass(char *);
void secondPass(char *);
void evalLine(char *, int, int);
void manageExtern(char * , int );
void manageSecondPassCommand(char * , int );
void manageEntry(char * , int );
void manageData(char * , char * , int );
void manageCommand(char * , char * , int );
void addData(int);
void addInstruction(int);
void stringFrom(int, char *);
int checkType(char * );
void appendImage(imgPtr, imgPtr);
void addToExtern(char *, int);
void addToEntries(char *, int);
void freeMem();
/* Prototype End */

/*Globally declared variables for the assemble.c file*/
static int data_counter = 0;
static int instructions_counter = 100;
static int sec_pass_counter = 0; /*For tracking current instruction in the second pass*/

/*initial variables for the tables*/
static imgPtr ins_table = NULL;
static imgPtr dat_table = NULL;
static nt_ptr entries_table = NULL;
static nt_ptr extern_table = NULL;

static int lineCount = 1; /*For tracking the line number - used for error maintnence*/
static int hasError = FALSE; /*For tracking errors globally*/
