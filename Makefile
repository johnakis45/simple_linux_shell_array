# Author: Ioannis Davanos
# Academic ID: 4622
# E-mail: csd4622@csd.uoc.gr

CC = gcc
CFLAGS = -Wall -Werror

# Define the source files and the output binary
SOURCES = shell.c utils.c
EXECUTABLE = shell

# Define the object files based on source files
OBJECTS = $(SOURCES:.c=.o)

# Default target (all) to build the executable
all: $(EXECUTABLE)

# Compile the source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Link the object files to create the executable
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(EXECUTABLE)

# Clean up the object files and the executable
clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

# Run the compiled program
run: $(EXECUTABLE)
	./$(EXECUTABLE)

.PHONY: all clean run
