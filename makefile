CC = gcc
FILES = main.c

all: $(FILES)
	$(CC) $(FILES) -o run

clean:
	rm run
