#include "./headers/utils.h"


/*	Two Pass Assembler
 * 	Written by : Artyom Generalov 319525671
 * 	*/



int main(int argc, char * argv[])
{

	tableConstructor();

	int i, c, k;
	int size;
	FILE * input_file;
	char * name;
	char * appendedName;
	char * fileArray;
	tableConstructor();
	
	if(argc == 1)
	{
		printf("\n Please enter at least 1 file as argument\n");
		exit(0);
	}
	
	for(i = 1; i < argc; i++)
	{
		name = malloc(sizeof(char) * (strlen(argv[i]) + 1));
		mem_alloc_check(name);
		strcpy(name,argv[i]);
		appendedName = (char *) malloc(sizeof(char) * (strlen(name)+1) + 4);
		mem_alloc_check(appendedName);
		strcpy(appendedName,name);
		strcat(appendedName,".as");
		
		if(!(input_file = fopen(appendedName,"r")))
		{
			printf("Error: cannot open file %s\n",argv[i]);
			exit(0);
		}
		free(appendedName);
		
		/*Getting the file inside an array so that the logic wont have to deal with prompting a file*/
		k = 0;
		fseek(input_file,0,SEEK_END);
		size = ftell(input_file);
		fseek(input_file,0,SEEK_SET);
		
		fileArray = malloc(size);
		mem_alloc_check(fileArray);
		while((c = fgetc(input_file)) != EOF)
			fileArray[k++] = c;
		fileArray[k] = 0;
		rewind(input_file);
		fclose(input_file);
		assemble(fileArray, name);
		free(fileArray);
		free(name);
	}
	
	freeFuncTable();
	return 0;
}
