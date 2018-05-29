CFLAGS = -c -Wall -Werror -ansi
all: compresser
compresser: main.o FileCompressor.o PriorityQueue.o Encryption.o
	gcc main.o PriorityQueue.o FileCompressor.o Encryption.o -o compresser
main.o:
	gcc $(CFLAGS) main.c
FileCompressor.o:
	gcc $(CFLAGS) FileCompressor.c
PriorityQueue.o:
	gcc $(CFLAGS) PriorityQueue.c
Encryption.o:
	gcc $(CFLAGS) Encryption.c
clean:
	rm -rf *o compresser