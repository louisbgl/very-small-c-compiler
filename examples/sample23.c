int main() {
    int x = 10;
    int y = 3;
    int result = 0;
    
    while (x > 0) {
        if (x > 5) {
            result = result + y * 2;
        } else {
            result = result + y;
        }
        x = x - 1;
    }
    
    return result;
}