CC = gcc

FILES = main.c  stages.c structs.c memory.c cache.c
OBJECTS = main.o stages.o structs.o memory.o cache.o

main.run: $(OBJECTS)
	$(CC) -Llib $(OBJECTS) -lm

$(OBJECTS): $(FILES)
	$(CC) -Iinclude -c $(FILES)

clean:
	rm *.o
	rm a.out
