CC=gcc
CFLAGS= -Wall

fxor: fxor.c
	$(CC) $(CFLAGS) -o fxor fxor.c
