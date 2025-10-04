# compiler
CC = gcc

# compiler flags
CFLAGS = -std=c17 -Wall -O2 -Iinclude

# build directory
BUILD_DIR = build

# final executable name
TARGET = remap

# path to the final executable
EXECUTABLE = $(BUILD_DIR)/$(TARGET)

# source files (all .c files in src/)
SRCS = $(wildcard src/*.c)

# object files (corresponding .o file for each .c)
OBJS = $(patsubst src/%.c,$(BUILD_DIR)/%.o,$(SRCS))

# default rule: build everything
all: $(EXECUTABLE)

# rule to link the executable
$(EXECUTABLE): $(OBJS)
	@echo "Linking..."
	$(CC) $(OBJS) -o $@ -lm

# rule to comile source files into object files in BUILD_DIR
$(BUILD_DIR)/%.o: src/%.c
	@mkdir -p $(BUILD_DIR)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# rule to clean up comiled files
clean:
	@echo "Clearning up..."
	rm -rf $(BUILD_DIR)

# rule to run the program
run: all
	@echo "Running program..."
	./$(EXECUTABLE)

# commands
.PHONY: all clean run
