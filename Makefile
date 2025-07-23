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

# Source files
SRCS := main.cpp \
        $(wildcard $(SRCDIR)/lexer/*.cpp)

# Object files
OBJS := $(patsubst %.cpp,$(BUILDDIR)/%.o,$(SRCS))

# Include directories
INCLUDES := -I$(SRCDIR)

# Default rule
all: setup $(TARGET)

# Debug build
debug: CXXFLAGS += $(DEBUGFLAGS)
debug: all

# Setup build directories
setup:
	@mkdir -p $(BUILDDIR)/$(SRCDIR)/lexer
	@mkdir -p $(BINDIR)

# Link the final executable
$(TARGET): $(OBJS)
	@echo "Linking $@"
	@$(CXX) $(CXXFLAGS) $^ -o $@

# Compile main.cpp
$(BUILDDIR)/main.o: main.cpp $(wildcard $(SRCDIR)/lexer/*.hpp)
	@echo "Compiling $<"
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Compile lexer source files
$(BUILDDIR)/$(SRCDIR)/lexer/%.o: $(SRCDIR)/lexer/%.cpp $(SRCDIR)/lexer/%.hpp $(SRCDIR)/lexer/token.hpp $(SRCDIR)/lexer/tokenType.hpp
	@echo "Compiling $<"
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Clean build artifacts
clean:
	@rm -rf $(BUILDDIR) $(BINDIR)

# Default file to run if none specified
FILE ?= examples/sample1.c

# Run the compiler with a file (use: make run FILE=examples/sample2.c)
run: all
	@echo "Running $(TARGET) with $(FILE)"
	@$(TARGET) $(FILE)

# Install the compiler (to /usr/local/bin)
install: all
	@echo "Installing $(TARGET) to /usr/local/bin"
	@cp $(TARGET) /usr/local/bin/vscc

# Helper to print variables for debugging
print-%:
	@echo $* = $($*)

# Phony targets
.PHONY: all debug clean setup run install

# Dependency tracking
-include $(OBJS:.o=.d)
