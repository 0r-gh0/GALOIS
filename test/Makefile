# Makefile for compiling the project

CC = gcc
CFLAGS = -lcrypto -lssl

# Targets
all: run
		@echo "Program Compiled! Run using ./run"

run: main.c add_sub.h read.h
		$(CC) main.c -o run $(CFLAGS)

clean:
		rm -f run
