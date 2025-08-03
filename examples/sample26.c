// Test: Complex function calls without parameters - "Function Call Salad"
int getBase() {
    return 10;
}

int getMultiplier() {
    int temp = 3;
    return temp;
}

int getOffset() {
    if (getBase() > 5) {
        return 7;
    } else {
        return 2;
    }
}

int calculate() {
    return getBase() * getMultiplier() + getOffset();
}

int finalBonus() {
    return 16;
}

int main() {
    int result = calculate();
    
    if (result > 30) {
        result = result + finalBonus();
    }
    
    while (result > 50) {
        result = result - getBase();
    }
    
    return result;  // Should return 43: (10*3+7)+16-10 = 53-10 = 43
}
