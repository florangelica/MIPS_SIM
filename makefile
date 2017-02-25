CC = gcc
FILES = main.c regFile.c
OBJECTS = main.o regFile.o

main.run: $(OBJECTS)
	$(CC) -Llib $(OBJECTS) -lm

$(OBJECTS): $(FILES)
	$(CC) -Wall -Iinclude -c $(FILES)

clean:
	rm *.o
	rm a.out
