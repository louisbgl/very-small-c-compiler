int main() {
    int a = 2;
    int b = 3;
    int c = 4;
    int result = a * b + c;
    int temp = result - a;
    temp = temp * b;
    result = temp / c + a;
    return result;
}