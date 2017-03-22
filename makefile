CC = gcc
FILES = main.c regFile.c decode.c
OBJECTS = main.o regFile.o decode.o

main.run: $(OBJECTS)
	$(CC) -Llib $(OBJECTS) -lm

$(OBJECTS): $(FILES)
	$(CC) -Wall -Iinclude -c $(FILES)

clean:
	rm *.o
	rm a.out
