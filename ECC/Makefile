# Makefile for Galois Field Arithmetic Program

# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -g

# Source files
SRCS = main.c utils.c add_sub.c mul.c exp.c

# Object files directory
OBJ_DIR = obj

# Object files
OBJS = $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRCS))

# Executable name
EXEC = run

# Create obj directory
$(shell mkdir -p $(OBJ_DIR))

# Default target
all: $(EXEC)
	@echo "Run it using ./run"

# Linking the program
$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Compiling source files into object files
$(OBJ_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up compiled files
clean:
	rm -rf $(OBJ_DIR) $(EXEC)

# Separate run target without dependencies
execute:
	./$(EXEC)

.PHONY: all clean run execute
