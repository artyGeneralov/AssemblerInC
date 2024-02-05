#include "./headers/utils.h"
#include "./headers/assemble.h"


/* Function recieves an assembly file array, sends it for evaluation and if no errors have been found,
 * creates .ob , .ent , .ext file as needed.
 * 
 * wholeFile: an array that holds the whole file
 * name: the name of the file, the created files will use that name
 * */
void assemble(char * wholeFile, char * name)
{

	nt_ptr * ptrToTable;
	imgPtr * ptrToImage;
	
	FILE * file_out;
	char * temp_name;
	int cnt = 100;
	
	firstPass(wholeFile);
	if(!hasError)
		secondPass(wholeFile);
	
	/*Create the files if no errors have been found*/
	if(!hasError)
	{
		/*Create or open the .ent file*/
		ptrToTable = &entries_table;
		temp_name = (char *) malloc(sizeof(char) * strlen(name) + 5);
		mem_alloc_check(temp_name);
		strcpy(temp_name,name);
		strcat(temp_name,".ent");
		if(!(file_out = fopen(temp_name,"w")))
		{
			printf("Error: cannot open file\n");
			exit(0);
		}
		
		/*if entries table is empty, .ent file will not be created*/
		while(*ptrToTable)
		{
			fprintf(file_out, "%s %.6d\n",(*ptrToTable)->name, (*ptrToTable)->value);
			(*ptrToTable) = (*ptrToTable)->next;
		}
		free(temp_name);
		fclose(file_out);
		
		/*Create or open the .ext file*/
		
		ptrToTable = &extern_table;
		temp_name = (char *) malloc(sizeof(char) * strlen(name) + 5);
		mem_alloc_check(temp_name);
		strcpy(temp_name,name);
		strcat(temp_name,".ext");
		if(!(file_out = fopen(temp_name,"w")))
		{
			printf("Error: cannot open file\n");
			exit(0);
		}
		
		/*if extern table is empty, .ext file will not be created*/
		while(*ptrToTable)
		{	
			fprintf(file_out, "%s %.6d\n",(*ptrToTable)->name, (*ptrToTable)->value);
			(*ptrToTable) = (*ptrToTable)->next;
		}
		fclose(file_out);
		free(temp_name);
		
		/*Create or open the .ob file*/
		ptrToImage = &ins_table;
		temp_name = (char *) malloc(sizeof(char) * strlen(name) + 4);
		mem_alloc_check(temp_name);
		strcpy(temp_name,name);
		strcat(temp_name,".ob");
		if(!(file_out = fopen(temp_name,"w")))
		{
			printf("Error: cannot open file\n");
			exit(0);
		}
		
		fprintf(file_out, "%7d %d\n", instructions_counter - 100, data_counter);
		while(*ptrToImage)
		{
			fprintf(file_out, "%.6d %.6x\n",cnt++, (*ptrToImage)->encodedValue);
			(*ptrToImage) = (*ptrToImage)->next;
		}
		fclose(file_out);
		free(temp_name);
		
	}
	/* For every file with errors, prompts which file could not be assembled
	 * no prompt is shown for correctly assembled files*/
	if(hasError)
		printf("Could not assemble file \"%s\"\n",name);
	hasError = FALSE;
	freeMem();
} 

/* 
 * Function to handle the first pass - reads each line in input and passes each line for evaluation.
 * after all lines have been evaluated, prepares the tables for the second pass and ititiates second pass.
 */
void firstPass(char * wholeFile)
{
	char line[MAX_LINE_SIZE];
	int c;
	int countLine = 1;
	int i, count;
	i = count = 0;
	c = wholeFile[i++];
	while(c != '\0')
	{
		line[count++] = c;
		if(count > MAX_LINE_SIZE)
		{
			printf("Error: in line %d, character limit reached\n", countLine);
			hasError = TRUE;
			count = 0;
			while((c = wholeFile[i++]) != '\n')
			;
			countLine++;
			continue;
		}
		if(c == '\n' || c == '\0')
		{
			line[count-1] = '\0';
			evalLine(line, countLine, 1);
			count = 0;
			countLine++;
		}
		c = wholeFile[i++];
	}
	line[count] = '\0';
	evalLine(line,countLine,1);
	
	
	/*Now we should have both tables ready*/
	if(!hasError)
	{
		/*If there was an error, the 2nd pass does not start*/
		appendImage(ins_table, dat_table); /* Append the second table to the first */
		fixSymbolTable(instructions_counter);
	}
}

/*
 * Function to handle the 2nd pass - again, reads each line in input and passes it for evaluation.
 * after all lines have been evaluated, initiates the files creation. */
void secondPass(char * wholeFile)
{
	char line[MAX_LINE_SIZE];
	int c;
	int countLine = 1;
	int i, count;
	i = count = 0;
	
	while((c = wholeFile[i++]) != '\0')
	{
		line[count++] = c;
		if(c == '\n' || c == '\0')
		{
			line[count-1] = '\0';
			evalLine(line, countLine, 2);
			count = 0;
			countLine++;
		}
	}
	
	line[count] = '\0';
	evalLine(line,countLine,2);
}

/*Appends the data image to the instruction image, to be used after the first pass*/
void appendImage(imgPtr ins_img, imgPtr dat_img)
{
	imgPtr * temp;
	imgPtr * tempDat;
	tempDat = &dat_img;
	temp = &ins_img;
	while((*temp)->next)
		(*temp) = (*temp)->next;
	(*temp)->next = *tempDat;
}

/*Adds a data entry to the data image*/
void addData(int val)
{
	imgPtr * head = &dat_table;
	imgPtr p1 = *head;
	imgPtr temp = (imgPtr) malloc(sizeof(node));
	imgPtr holder;
	mem_alloc_check(temp);
	temp->encodedValue = val;
	temp->next = NULL;
	while(p1)
	{
		holder = p1;
		p1 = p1->next;
	}
	
	if(p1 == *head)
		*head = temp;
	else
		holder->next = temp;
}

/*Adds instruction entry to instruction image*/
void addInstruction(int val)
{
	imgPtr * head = &ins_table;
	imgPtr p1 = *head;
	imgPtr temp = (imgPtr) malloc(sizeof(node));
	imgPtr holder;
	mem_alloc_check(temp);
	temp->encodedValue = val;
	temp->next = NULL;
	while(p1)
	{
		holder = p1;
		p1 = p1->next;
	}
	
	if(p1 == *head)
		*head = temp;
	else
		holder->next = temp;
}

/*Function to recieve a word str and return its type:
 * 1.) LABEL: word starts with a character, ends with ":"
 * 2.) DAT: .data or .string
 * 3.) ENTRY: .entry
 * 4.) EXTERN: .extern
 * 5.) -1 : a different sequence of characters*/
int checkType(char * str)
{
	int i = 0;
	char s[80];
	strcpy(s, str);
	
	if(s[0] == '.')
	{
		while(s[i] != '\0')
			s[i] = s[++i];
		if(strcmp(s, ".data") == 0 || strcmp(s, ".string") == 0)
			return DAT;
		if(strcmp(s, ".entry") == 0)
			return ENTRY;
		if(strcmp(s, ".extern") == 0)
			return EXTERN;
	}
	if(getopcode(s) != -1)
		return COMMAND;
	i = 0;
	while(s[i++] != '\0')
	;

	if(s[i-2] == ':')
	{
		s[i-2] = '\0';
		
		if(getopcode(s) == -1 && ((s[0] >= 'a' && s[0] <= 'z') || (s[0] >= 'A' && s[0] <= 'Z')))
		{
			if(strlen(s) == 2 && s[0] == 'r' && (s[1] >= '0' && s[1] <= '7'))
				return -1;
			else
				return LABEL;
		}
	}
	return -1;
}


/*Input parameters:
 * word: holds the command
 * line: holds the rest of the line trimmed after the command
 * countLine: the number of the line in the input file, for error handling
 * 
 * The function reads up to two parameters after the command and encodes the command accordingly.
 * The command encoding is added to the instruction table - ins_table
 * The parameters are encoded if possible to ins_table.
 * If encoding is impossible - adds a 0 value word to the table.*/
void manageCommand(char * word, char * or_line, int countLine)
{
	int allowedOperands, currOperand;
	int allowedSourceAddresses[AMOUNT_OF_ADDRESSES];
	int allowedDestAddresses[AMOUNT_OF_ADDRESSES];
	
	char temp[MAX_LINE_SIZE] = "";
	char line[MAX_LINE_SIZE];
	char reg[3];
	
	unsigned int * instruct_word = (unsigned int *)(&instruction);

	enum{src, dest};
	int current_op_handle = src;
	int i = 0;
	int k;
	int ops[2] = {0,0};
	int additionalWords = 0;
	int mode3 = 0;
	int c;

	*instruct_word = 0;
	
	
	for(i = 0; or_line[i] != '\0'; i++)
		line[i] = or_line[i];
	line[i] = '\0';
	i = 0;
	/*Get Relevant values*/
	getLegalSourceAddresses(word, allowedSourceAddresses);
	getLegalDestinationAddresses(word, allowedDestAddresses);
	allowedOperands = getOperands(word);
	/* Set trivial values */
	instruction.opcode = getopcode(word);
	instruction.funct = getfunct(word);
						
	/* Check for more trivial values */
	if(allowedOperands == 1)
	{
		instruction.sourceAdd = 0;
		instruction.sourceReg = 0;
		current_op_handle = dest;
	}
	else if(allowedOperands == 0)
	{
		instruction.sourceAdd = 0;
		instruction.sourceReg = 0;
		instruction.destAdd = 0;
		instruction.destReg = 0;
		current_op_handle = dest+1;
	}
	/*keep reading opearnds*/
	while(current_op_handle <= dest)
	{
		mode3 = FALSE;
		while((c=line[i++]) == ' ' || c == '\t')
		;
		switch(c)
		{
			case '#':
				/*Addressing mode 0*/
				/*Check that Addressing 0 is allowed*/
				if((current_op_handle == dest ? allowedDestAddresses[0] == 0 : allowedSourceAddresses[0] == 0))
				{
					printf("Error: line %d invalid operator for command %s\n", countLine, word);
					hasError = TRUE;
					return;
				}
				
				k = 0;
				/*Read the next opreator */
				if(line[i] == '-' || line[i] == '+')
					temp[k++] = line[i++];
				while((c = line[i++]) != ' ' && c != '\t' && c != '\n' && c != '\0' && (current_op_handle == src ? c != ',' : 1))
				{
					if(isdigit(c))
						temp[k++] = c;
					else
					{
						printf("Error: line %d invalid operator. Expected integer - instead recieved %c\n",countLine,c);
						hasError = TRUE;
						return;
					}
				}
				/*Check that its within the 21 bit boundry*/
				if((k = atoi(temp)) > MAX_POS_21 || k < MAX_NEG_21)
				{
					printf("Error: line %d value %d out of bounds\n",countLine,k);
					hasError = TRUE;
					return;
				}
				/*Add the parameter to the ops array*/
				ops[current_op_handle] = ((k << 3) + A) & 0xffffff;
				additionalWords++;
				if(current_op_handle == dest)
				{
					instruction.destAdd = 0;
					instruction.destReg = 0;
					current_op_handle++;
				}
				else
				{
					instruction.sourceAdd = 0;
					instruction.sourceReg = 0;
					current_op_handle++;
				}
				break;
			case '&':
				/*Addressing mode 2*/
				/*Check that its one of the allowed commands*/
				if(instruction.opcode != 9)
				{
					printf("Error: line %d invalid operator for command %s\n", countLine, word);
					hasError = TRUE;
					return;
				}
				/*Go to the end of the parameter */
				while((c = line[i++]) != ' ' && c != '\t' && c != '\n' && c != '\0')
				;
				additionalWords++;
				instruction.destAdd = 2;
				instruction.destReg = 0;
				current_op_handle++;
				break;
			default:
				k = 0;
				while(c != ' ' && c != '\t' && c != '\0' && (current_op_handle == src ? c != ',' : 1))
				{
					temp[k++] = c;
					c = line[i++];
				}
				temp[k] = '\0';
				/* Check if temp is one of the registers*/
				for(k = 0; k < 8; k++)
				{
					reg[0] = 'r';
					reg[1] = '0' + k;
					reg[2] = '\0';
					if(!strcmp(reg, temp))
					{
						/*Addressing mode 3*/
						mode3 = TRUE;
						if((current_op_handle == dest ? allowedDestAddresses[3] == 0 : allowedSourceAddresses[3] == 0))
						{
							printf("Error: line %d invalid operator for command %s\n", countLine, word);
							hasError = TRUE;
							return;
						}
						
						if(current_op_handle == dest)
						{
							instruction.destAdd = 3;
							instruction.destReg = k;
							current_op_handle++;
						}
						else
						{
							instruction.sourceAdd = 3;
							instruction.sourceReg = k;
							current_op_handle++;
						}
						
						break;
					}
				}
				if(!mode3)
				{
					/*Adressing 1*/
					additionalWords++; 
					if(current_op_handle == dest)
					{
						instruction.destAdd = 1;
						instruction.destReg = 0;
						current_op_handle++;
					}
					else
					{
						instruction.sourceAdd = 1;
						instruction.sourceReg = 0;
						current_op_handle++;
					}
				}
				break;
		}
	}
	i--;
	while((c = line[i++]) == ' ' || c == '\t')
	;
	/*Check that there is no nonsense written after a valid line*/
	
	if(c != '\n' && c!= '\0')
	{
		printf("c = %c\n",c);
		printf("Error: line %d syntax error, line is %s\n",countLine,line);
		hasError = TRUE;
		return;
	}
	/* Finish encoding the isntruction */
	instruction.A = 1;
	addInstruction(*instruct_word);
	instructions_counter++;
	/* Add additional entries to the instructions table according to the amount of additional words calculated
	 * fill every new line with the corresponding value in ops array (default 0)*/
	while(additionalWords > 0)
	{
		instructions_counter++;
		addInstruction(ops[additionalWords % 2]);
		additionalWords--;
	}
}


/*Input parameters:
 * word: holds the data type - string or data
 * line: holds the rest of the line trimmed after the word
 * countLine: the number of the line in the input file, for error handling
 * 
 * */
void manageData(char * word, char * line, int countLine)
{
	char temp[MAX_LINE_SIZE];
	int c;
	int i, countTemp, num;
	
	i = countTemp = 0;
	c = line[i];
	
	/* Differentiate between .data and .string*/
	if(strcmp(word, ".data") == 0) 
	{
		while(c != '\0'){
			countTemp = 0;
			while((c = line[i++]) == ' ' || c == '\t')
			;
			
			if(c == '+' || c == '-')
			{
				temp[countTemp++] = c;
				c = line[i++];
			}
			
			while(c != ',' && c != '\0' && c != ' ' && c != '\t')
			{

				if(!isdigit(c))
				{
					printf("Error: line %d integer expected after .data tag, instead recieved \"%c\"\n",countLine,c);
					hasError = TRUE;
					return;
				}
				temp[countTemp++] = c;
				c = line[i++];
			}
			temp[countTemp] = '\0';

			num = atoi(temp);
			/* Check that the number is within the 24 bit boundry */
			if(num > MAX_POS_24 || num < MAX_NEG_24)
			{
				printf("Error: line %d value %d out of bounds\n", countLine, num);
				hasError = TRUE;
				return;
			}
			else
			{	
				/* add the number to the table*/
				if(num < 0)
					num = num & 0xffffff;
				addData(num);
				data_counter++;
			}
			
			if(c == '\0')
				break;
			if(c != ',')
				while((c = line[i]) == ' ' || c == '\t')
					i++;
		}
	}
	else /*.string*/
	{
		while((c = line[i++]) == ' ' || c == '\t')
		;
		
		if(c != '\"')
		{
			printf("Error: line %d invalid string format\n", countLine);
			hasError = TRUE;
			return;
		}
		
		while((c = line[i++]) != '\"')
		{
			addData(c);
			data_counter++;
		}
		addData('\0');
		data_counter++;
	}
}

/*Function to trim a sting from a certain place onward*/
void stringFrom(int place, char * line)
{
	int i, k;
	i = place;
	k = 0;
	while(line[i] != '\0')
		line[k++] = line[i++];
	line[k] = '\0';
	line[k+1] = '\0';
}

/*manages .entry cases, adds "entry" to a symbols type in the symbols table*/
void manageEntry(char * line, int countLine)
{
	int c;
	int i = 0;
	char buf[MAX_LINE_SIZE];
	int bufCount = 0;
	while((c = line[i]) == ' ' || c == '\t')
		i++;
	while((c = line[i++]) != ' ' && c != '\n' && c != '\t')
		buf[bufCount++] = c;
	buf[bufCount] = '\0';
	if(symbolExists(buf))
	{
		addType(buf, entry_type);
		addToEntries(buf, getSymbolValue(buf));
	}
	else
	{
		printf("Error: line %d .entry symbol %s does not exist\n",countLine, buf);
		hasError = TRUE;
	}
}

/*manages .extern cases, adds "external" to a symbols type in the symbols table*/
void manageExtern(char * line, int countLine)
{
	char temp[MAX_LINE_SIZE];
	int c;
	int i,countTemp;
	i = countTemp = 0;
	
	while((c = line[i++]) == ' ' || c == '\t')
	;
	while(c != ' ' && c != '\t' && c != '\0')
	{
		temp[countTemp++] = c;
		c = line[i++];
	}
	temp[countTemp] = '\0';
	/*Check for invalid input after label name*/	
	while(c == ' ' || c == '\t')
		c = line[i++];
	if(c != '\0')
	{
		printf("Error: line %d syntax error\n", countLine);
		hasError = TRUE;
		return;
	}
	
	i = temp[0];
	if( (i >= 'a' && i <= 'z' ) || (i >= 'A' && i <= 'Z') )
	{
		if(!symbolExists(temp))
		{
			addSymbol(temp, 0);
			addType(temp, external_type);
		}
	}
}

/*manages a command in the second pass of the assembler, encodes the relevant value in the relevant place in the image*/
void manageSecondPassCommand(char * line, int countLine)
{
	
	imgPtr * tableHead = &ins_table;
	imgPtr n = *tableHead;
	int c;
	int i = 0;
	char temp[MAX_LINE_SIZE];
	int countBuf;
	int symbolTypes[4];
	char reg[3];
	int are_value;
	unsigned int dist;
	int currInst = sec_pass_counter;
	int k = 0;
	int regis;
	int inst = sec_pass_counter;
	/*find the current node to work on*/
	while(currInst-- > 0)
		n = n->next;
	c = line[i];
	while(c != '\0')
	{
		i = 0;
		countBuf = 0;
		regis = FALSE;	
		while((c = line[i]) == ' ' || c == '\t')
			c = line[++i];
		while(c != ' ' && c != '\t' && c != ',' && c != '\0')
		{
			temp[countBuf++] = c;
			c = line[++i];
		}
		
		temp[countBuf] = '\0';
		switch(temp[0])
		{
			case '#':
				n = n->next;
				sec_pass_counter++;
				break;
			case '&':
				sec_pass_counter++;
				n = n->next;
				/*This is distance to a label of a COMMAND (code_type)*/
				stringFrom(1,temp);
				if(symbolExists(temp))
				{
					dist = getSymbolValue(temp) - (sec_pass_counter + 99);
					getSymbolTypes(temp, symbolTypes);
					if(symbolTypes[code_type])
						n->encodedValue = ((dist << 3) + A)&0xffffff;
				}
				else
				{
					printf("Error: Line %d symbol %s does not exist\n", countLine, temp);
					hasError = TRUE;
				}
				break;
			default:
				for(int j = 0; j < 8; j++)
				{
					reg[0] = 'r';
					reg[1] = '0' + j;
					reg[2] = '\0';
					/*Check that we're not in addressing mode 3*/
					if(!strcmp(temp,reg))
						regis = TRUE;
				}
				
				/*if not in addressing 3:*/
				if(!regis)
				{

					/*Addressing mode 1*/
					if(symbolExists(temp))
					{
						n = n->next;
						sec_pass_counter++;
						getSymbolTypes(temp, symbolTypes);
						if(symbolTypes[external_type])
						{
							are_value = E;
							addToExtern(temp, inst+1);
						}
						else
							are_value = R;
						n->encodedValue = (getSymbolValue(temp) << 3) + are_value;
						
					}
					else
					{
						printf("Error: Line %d symbol %s does not exist\n", countLine, temp);
						hasError = TRUE;
					}
				}
				break;
		}
		while(c == ' ' || c == '\t')
			c = line[i++];
		if(c != '\0')
			stringFrom(i+1,line);
	}
	sec_pass_counter++;
}

/*Input parameters:
 * or_line: a line to be evaluated
 * countLine: line in input file, for error handling
 * pass: which pass we are doing - 1 or 2.
 * 
 * Function recieves a line of assembly code,
 * interprets the type of the line, assigns label into symbol table
 * and calls the appropriate function for the type of instruction - data or command.
 * handles lines for both the first and second pass.
 * */
void evalLine(char * or_line, int countLine, int pass)
{
	
	char line[80];
	int type;
	int wordCounter = 0;
	char word[80];
	char label[31];
	int c;
	int i = 0;
	int hasLabel = FALSE;
	int encoding;
	int counterHolder;
	
	strcpy(line, or_line); /*to use local variable line insted of the main array*/
	while((c = line[i]) == ' ' || c == '\t')
		i++;
	
	if(c == ';' || c == '\n' || c == '\0')
		return;
	
	/*Would read max two words*/
	while(TRUE){
		wordCounter = 0;
		i = 0;
		while((c = line[i]) == ' ' || c == '\t')
			i++;

		while((c = line[i++]) != ' ' && c != '\t' && c!='\0')
			word[wordCounter++] = c;
			
		word[wordCounter] = '\0';
		type = checkType(word);
		switch(type)
		{
			case LABEL:
					word[--wordCounter] = '\0';
					strcpy(label, word);
					hasLabel = TRUE;
					stringFrom(i, line);
				break;
			case DAT:
				if(pass == 1)
				{
					stringFrom(i, line);
					counterHolder = data_counter;
					manageData(word, line, countLine);
					if(hasLabel)
					{
						addSymbol(label, counterHolder);
						addType(label, data_type);
					}
					return;
				}
				else
					return;
				break;
			case ENTRY:
				if(pass == 1)
					return;
				else
				{
					stringFrom(i, line);
					manageEntry(line, countLine);
					return;
				}
				break;
			case EXTERN:
				if(pass == 1)
				{
					stringFrom(i, line);
					manageExtern(line, countLine);
					if(hasLabel)
						printf("Warning: line %d, Meaningless label \"%s\" in line with .extern",countLine,label);
					return;
				}
				else
					return;
				break;
			case COMMAND:
				if(pass == 1)
				{
					stringFrom(i, line);
					counterHolder = instructions_counter;
					manageCommand(word, line, countLine);
					if(hasLabel)
					{
						
						addSymbol(label, counterHolder);
						addType(label, code_type);
					}
					return; 
				}
				else
				{
					stringFrom(i,line);
					manageSecondPassCommand(line, countLine);
					return;
				}
				break;
			default:
				if(pass == 1)
				{
					printf("Error: line %d syntax error, line is %s\n", countLine,line);
					hasError = TRUE;
					return;
				}
				else
				{

				}
				break;
		}
	}
}

/*add value to extern table*/
void addToExtern(char * name, int value)
{
	
	nt_ptr * ptrToTable = &extern_table;
	nt_ptr p1 = *ptrToTable;
	nt_ptr temp = (nt_ptr) malloc(sizeof(nt_node));
	nt_ptr holder;
	mem_alloc_check(temp);
	strcpy(temp->name, name);
	temp->value = value;
	temp->next = NULL;
	
	while(p1)
	{
		holder = p1;
		p1 = p1->next;
	}
	
	if(p1 == *ptrToTable)
		*ptrToTable = temp;
	else
		holder->next = temp;
}

/*Add value to entries table*/
void addToEntries(char * name, int value)
{
	
	nt_ptr * ptrToTable = &entries_table;
	nt_ptr p1 = *ptrToTable;
	nt_ptr temp = (nt_ptr) malloc(sizeof(nt_node));
	nt_ptr holder;
	mem_alloc_check(temp);
	strcpy(temp->name, name);
	temp->value = value;
	temp->next = NULL;
	
	while(p1)
	{
		holder = p1;
		p1 = p1->next;
	}
	
	if(p1 == *ptrToTable)
		*ptrToTable = temp;
	else
		holder->next = temp;
}

/* Function to free allocated memmory */
void freeMem()
{
	nt_ptr * ptrToTable;
	imgPtr * ptrToImg;
	
	nt_ptr tmp;
	imgPtr ptr;
	
	
	ptrToTable = &entries_table;
	while(*ptrToTable)
	{
		tmp = *ptrToTable;
		*ptrToTable = (*ptrToTable)->next;
		free(tmp);
	}

	ptrToTable = &extern_table;
	while(*ptrToTable)
	{
		tmp = *ptrToTable;
		*ptrToTable = (*ptrToTable)->next;
		free(tmp);
	}
	
	ptrToImg = &ins_table;
	while(*ptrToImg)
	{
		ptr = *ptrToImg;
		*ptrToImg = (*ptrToImg)->next;
		free(ptr);
	}
	
	ptrToImg = &dat_table;
	while(*ptrToImg)
	{
		ptr = *ptrToImg;
		*ptrToImg = (*ptrToImg)->next;
		free(ptr);
	}
	data_counter = 0;
	instructions_counter = 100;
	sec_pass_counter = 0;
	lineCount = 1;
	hasError = FALSE;
	freeSymbolsTable();
}

