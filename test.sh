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
    local expected_exit_code="$2"
    local test_name="$3"
    
    echo -n "Testing $test_name... "
    
    # Run the compiler and capture its exit code
    ./bin/vscc "$test_file" > /dev/null 2>&1
    actual_exit_code=$?
    
    TOTAL=$((TOTAL + 1))
    
    if [ $actual_exit_code -eq $expected_exit_code ]; then
        echo -e "${GREEN}PASSED${NC} (exit code: $actual_exit_code)"
        PASSED=$((PASSED + 1))
    else
        echo -e "${RED}FAILED${NC} (expected: $expected_exit_code, got: $actual_exit_code)"
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

# Run tests
echo "Running tests..."
echo

run_test "examples/sample1.c" 0 "Return zero"
run_test "examples/sample2.c" 1 "Simple addition (0 + 1)"
run_test "examples/sample3.c" 3 "Complex addition (0 + 1 + 2)"
run_test "examples/sample4.c" 5 "Subtraction (7 - 2)"
run_test "examples/sample5.c" 19 "Complex expression (0 + 1 + ... + 10)"

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
