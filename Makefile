CC=gcc
CFLAGS=-Wall
LDFLAGS=-Wall

all : compilation clean

fonctions.o : fonctions.c
	$(CC) -c fonctions.c $(CFLAGS)

bitmap.o : bitmap.c
	$(CC) -c bitmap.c $(CFLAGS)

text_hiding.o : text_hiding.c
	$(CC) -c text_hiding.c $(CFLAGS)

main.o : main.c
	$(CC) -c main.c fonctions.c text_hiding.c bitmap.c $(CFLAGS)

compilation : bitmap.o fonctions.o text_hiding.o main.o
	$(CC) -o stega  bitmap.o fonctions.o text_hiding.o main.o $(CFLAGS)

clean:
	rm -rf *.o

proper:
	rm -rf *.o
	rm -rf stega
