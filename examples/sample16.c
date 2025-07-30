int main() {
    int a = 10;
    int b = 5;
    int result = 0;
    
    if (a - b) {
        int temp = a * 2;
        result = temp + b;
    }
    
    if (result) {
        result = result / 3;
        if (result - 4) {
            result = result + 1;
        }
    }
    
    int final = result * (a + b) - 20;
    return final;
}