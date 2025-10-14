# compiler
CXX = g++

# compiler flags
CXXFLAGS = -std=c++17 -Wall -O2 -Iinclude

# build directory
BUILD_DIR = build

# final executable name
TARGET = remap

# path to the final executable
EXECUTABLE = $(BUILD_DIR)/$(TARGET)

# source files (all .cpp files in src/)
SRCS = $(wildcard src/*.cpp) $(wildcard src/mapper/*/*.cpp)

# object files (corresponding .o file for each .cpp)
OBJS = $(patsubst src/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

# default rule: build everything
all: $(EXECUTABLE)

# rule to link the executable
$(EXECUTABLE): $(OBJS)
	@echo "Linking..."
	$(CXX) $(OBJS) -o $@ -lm

# rule to comile source files into object files in BUILD_DIR
$(BUILD_DIR)/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	@echo "Compiling $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

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
