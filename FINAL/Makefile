# Makefile for compiling the project

CC = gcc
CFLAGS = -lcrypto -lssl

# Targets
all: run

run: main.c add_sub.h read.h
	$(CC) main.c -o run $(CFLAGS)  # Make sure this line starts with a tab

clean:
	rm -f run  # Make sure this line also starts with a tab
