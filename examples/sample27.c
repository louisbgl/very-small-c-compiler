// Test: Functions with parameters
int add(int a, int b) {
    return a + b;
}

int main() {
    int result = add(5, 10);
    if (result == 15) {
        return 0;
    } else {
        return 1;
    }
    return result;
}