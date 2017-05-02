CC = gcc
FILES = main.c  stages.c structs.c mainMemory.c
OBJECTS = main.o stages.o structs.o mainMemory.o

main.run: $(OBJECTS)
	$(CC) -Llib $(OBJECTS) -lm

$(OBJECTS): $(FILES)
	$(CC) -Wall -Iinclude -c $(FILES)

clean:
	rm *.o
	rm a.out
