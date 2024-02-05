FILES = funcTable.o symbolsTable.o assemble.o



assembler: $(FILES)
	gcc -g $(FILES) -o myAssembler -lm

funcTable.o: funcTable.c ./headers/func_table_header.h
	gcc -c funcTable.c -o funcTable.o

symbolsTable.o: symbolsTable.c ./headers/symbols_table_header.h ./headers/utils.h
	gcc -c symbolsTable.c -o symbolsTable.o

assemble.o: assemble.c ./headers/utils.h
	gcc -c assemble.c -o assemble.o
