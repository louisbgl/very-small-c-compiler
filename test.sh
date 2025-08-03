#!/bin/bash

# Test script for very-small-c-compiler
# Usage: ./test.sh

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Test results
PASSED=0
FAILED=0
TOTAL=0

# Function to run a single test
run_test() {
    local test_file="$1"
    local test_name="$2"
    
    echo -n "Testing $test_name... "
    
    # Compile and run with GCC to get expected result
    gcc "$test_file" -o gcc_test_output 2>/dev/null
    if [ $? -ne 0 ]; then
        echo -e "${YELLOW}SKIPPED${NC} (GCC compilation failed)"
        rm -f gcc_test_output
        return
    fi
    
    ./gcc_test_output > /dev/null 2>&1
    expected_exit_code=$?
    rm -f gcc_test_output
    
    # Run our compiler and capture its exit code
    ./bin/vscc "$test_file" > /dev/null 2>&1
    actual_exit_code=$?
    
    TOTAL=$((TOTAL + 1))
    
    if [ $actual_exit_code -eq $expected_exit_code ]; then
        echo -e "${GREEN}PASSED${NC} (exit code: $actual_exit_code, matches GCC)"
        PASSED=$((PASSED + 1))
    else
        echo -e "${RED}FAILED${NC} (VSCC: $actual_exit_code, GCC: $expected_exit_code)"
        FAILED=$((FAILED + 1))
    fi
}

echo "========================================"
echo "  Very Small C Compiler - Test Suite"
echo "========================================"
echo

# Check if compiler is built
if [ ! -f "./bin/vscc" ]; then
    echo -e "${RED}Error: Compiler not found. Run 'make' first.${NC}"
    exit 1
fi

# Check if GCC is available
if ! command -v gcc &> /dev/null; then
    echo -e "${RED}Error: GCC not found. Please install GCC for reference testing.${NC}"
    exit 1
fi

# Run tests
echo "Running tests (comparing against GCC)..."
echo

run_test "examples/sample1.c" "[sample1]  Return zero"
run_test "examples/sample2.c" "[sample2]  Simple addition"
run_test "examples/sample3.c" "[sample3]  Complex addition"
run_test "examples/sample4.c" "[sample4]  Subtraction"
run_test "examples/sample5.c" "[sample5]  Complex expression"
run_test "examples/sample6.c" "[sample6]  Expression with parentheses"
run_test "examples/sample7.c" "[sample7]  Nested expressions with parentheses"
run_test "examples/sample8.c" "[sample8]  Multiplication"
run_test "examples/sample9.c" "[sample9]  Division"
run_test "examples/sample10.c" "[sample10] Multiplication with addition"
run_test "examples/sample11.c" "[sample11] Complex nested expression"
run_test "examples/sample12.c" "[sample12] Variable assignment and usage"
run_test "examples/sample13.c" "[sample13] Variable assignements and complex usage"
run_test "examples/sample14.c" "[sample14] Variable assignment with arithmetic"
run_test "examples/sample15.c" "[sample15] Simple if statement"
run_test "examples/sample16.c" "[sample16] Nested if statements and arithmetic"
run_test "examples/sample17.c" "[sample17] If else statement"
run_test "examples/sample18.c" "[sample18] Nested if else statements"
run_test "examples/sample19.c" "[sample19] Comparison in if statement"
run_test "examples/sample20.c" "[sample20] Complex if statement with multiple conditions"

echo
echo "========================================"
echo "Test Results:"
echo -e "  ${GREEN}Passed: $PASSED${NC}"
echo -e "  ${RED}Failed: $FAILED${NC}"
echo "  Total:  $TOTAL"
echo "========================================"

if [ $FAILED -eq 0 ]; then
    echo -e "${GREEN}All tests passed!${NC}"
    exit 0
else
    echo -e "${RED}Some tests failed.${NC}"
    exit 1
fi
