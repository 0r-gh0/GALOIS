# Compiler
CC = gcc

# Compiler flags
CFLAGS = -w

# Output file
OUTPUT = run

# Source files
SRC = main.c utils.c mul.c exp.c

# Header files (for clarity, not explicitly needed in compilation)
HEADERS = main.h utils.h add_sub.h mul.h exp.h

# Object files
OBJS = $(SRC:.c=.o)

# Default target
all: $(OUTPUT)
		@echo "RUN! It using ./$(OUTPUT)"

# Link the object files to create the executable
$(OUTPUT): $(OBJS)
		$(CC) $(CFLAGS) -o $@ $^

# Compile each .c file into .o file
%.o: %.c $(HEADERS)
		$(CC) $(CFLAGS) -c $< -o $@

# Clean up
clean:
		rm -f $(OBJS) $(OUTPUT)
