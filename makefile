CC = gcc

FILES = main.c  stages.c structs.c mainMemory.c cache.c
OBJECTS = main.o stages.o structs.o mainMemory.o cache.o

main.run: $(OBJECTS)
	$(CC) -Llib $(OBJECTS) -lm

$(OBJECTS): $(FILES)
	$(CC) -Iinclude -c $(FILES)

clean:
	rm *.o
	rm a.out
