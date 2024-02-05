FILES = ./libs/funcTable.o ./libs/symbolsTable.o ./libs/assemble.o ./libs/assembler.o
HEADER = ./libs/headers
FLAGS = -Wall -ansi -Werror -pedantic -g


assembler: $(FILES)
	gcc -g $(FILES) -o assembler -lm

funcTable.o: ./libs/funcTable.c $(HEADER)/func_table_header.h $(HEADER)/utils.h
	gcc -c ./libs/funcTable.c -o $(FLAGS) funcTable.o

symbolsTable.o: ./libs/symbolsTable.c $(HEADER)/symbols_table_header.h $(HEADER)/utils.h
	gcc -c ./libs/symbolsTable.c $(FLAGS) -o symbolsTable.o

assemble.o: assemble.c $(HEADER)/utils.h $(HEADER)/assemble.h
	gcc -c ./libs/assemble.c -o $(FLAGS) assemble.o

assembler.o: assembler.c $(HEADER)/utils.h
	gcc -c ./libs/assembler.c -o $(FLAGS) assembler.o
