##############################################################
# Makefile for very-small-c-compiler
##############################################################

# Compiler and standard settings
CXX := g++
CXXSTD := -std=c++20
CXXFLAGS := $(CXXSTD) -Wall -Wextra -Wpedantic -Werror -O2
DEBUGFLAGS := -g -O0 -DDEBUG

# Directories
SRCDIR := src
BUILDDIR := build
BINDIR := bin

# Output binary
TARGET := $(BINDIR)/vscc

# Source files - automatically discover all .cpp files
SRCS := main.cpp $(shell find $(SRCDIR) -name "*.cpp" -type f)

# Object files
OBJS := $(patsubst %.cpp,$(BUILDDIR)/%.o,$(SRCS))

# Automatically create list of all subdirectories that need to be created
BUILD_DIRS := $(sort $(dir $(OBJS)))

# Include directories
INCLUDES := -I$(SRCDIR)

# Default rule
all: setup $(TARGET)

# Debug build
debug: CXXFLAGS += $(DEBUGFLAGS)
debug: all

# Setup build directories - automatically create all needed directories
setup:
	@mkdir -p $(BUILD_DIRS) $(BINDIR)

# Link the final executable
$(TARGET): $(OBJS)
	@echo "Linking $@"
	@$(CXX) $(CXXFLAGS) $^ -o $@

# Compile main.cpp
$(BUILDDIR)/main.o: main.cpp $(shell find $(SRCDIR) -name "*.hpp" -type f)
	@echo "Compiling $<"
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Generic rule for compiling any .cpp file in src/
$(BUILDDIR)/$(SRCDIR)/%.o: $(SRCDIR)/%.cpp
	@echo "Compiling $<"
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Clean build artifacts
clean:
	@rm -rf $(BUILDDIR) $(BINDIR)

# Default file to run if none specified
FILE ?= examples/sample1.c

# Run the compiler with a file (use: make run FILE=examples/sample2.c)
run: all
	@echo "Running $(TARGET) with $(FILE)"
	@$(TARGET) $(FILE); EXIT_CODE=$$?; \
	echo "Exit code was: $$EXIT_CODE"; \
	exit 0

# Run tests
test: all
	@echo "Running test suite..."
	@./test.sh; EXIT_CODE=$$?; \
	echo "Test suite exit code: $$EXIT_CODE"; \
	exit 0

# Install the compiler (to /usr/local/bin)
install: all
	@echo "Installing $(TARGET) to /usr/local/bin"
	@cp $(TARGET) /usr/local/bin/vscc

# Helper to print variables for debugging
print-%:
	@echo $* = $($*)

# Phony targets
.PHONY: all debug clean setup run test install

# Dependency tracking
-include $(OBJS:.o=.d)
