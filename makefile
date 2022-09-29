CC = gcc

all: shell batchfile

shell: shell.c

	$(CC) -o shell shell.c

batch: batchfile.c

	$(CC) -o batchfile batchfile.c 

clean:
	rm -f shell.c
	rm -f batchfile.c