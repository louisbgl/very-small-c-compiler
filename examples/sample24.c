int main() {
    int a = 42;
    int b = 13;
    int c = 7;
    int d = 0;
    int result = 0;
    int counter = 0;
    int factorial = 1;
    int sum = 0;
    int temp = 0;
    
    // Test complex arithmetic with precedence
    result = a + b * c - d / 2 + 3 * (a - b);
    
    // Nested if-else with complex conditions
    if (a > b) {
        if (c < b) {
            result = result + a * c;
            if (a > 30) {
                result = result - 5;
            } else {
                result = result + 10;
            }
        } else {
            if (b == 13) {
                result = result * 2;
                if (c > 5) {
                    result = result / 3;
                } else {
                    result = result + c;
                }
            } else {
                result = result - b;
            }
        }
    } else {
        if (a == b) {
            result = 100;
        } else {
            result = result + a + b + c;
            if (result > 100) {
                result = result - 50;
            }
        }
    }
    
    // Nested while loops with complex logic
    counter = 0;
    while (counter < 5) {
        temp = counter;
        while (temp > 0) {
            sum = sum + temp;
            temp = temp - 1;
            
            if (temp == 2) {
                sum = sum * 2;
            } else {
                if (temp == 1) {
                    sum = sum + 1;
                }
            }
        }
        counter = counter + 1;
    }
    
    // Calculate factorial with nested conditions
    factorial = 1;
    counter = 1;
    while (counter <= 6) {
        factorial = factorial * counter;
        
        if (counter > 3) {
            if (factorial > 50) {
                factorial = factorial - 10;
            } else {
                factorial = factorial + 5;
            }
        }
        
        counter = counter + 1;
    }
    
    // Complex nested structure with multiple conditions
    if (factorial > sum) {
        counter = 0;
        while (counter < 5) {
            if (sum < factorial / 2) {
                sum = sum + 3;
                if (sum > 20) {
                    if (sum < 40) {
                        sum = sum * 2;
                    } else {
                        sum = sum - 15;
                        if (sum < 10) {
                            sum = sum + 25;
                        }
                    }
                }
            }
            counter = counter + 1;
        }
    } else {
        counter = 0;
        while (counter < 3) {
            if (counter == 0) {
                sum = sum + factorial;
            } else {
                if (counter == 1) {
                    sum = sum - result;
                } else {
                    sum = sum + a * b;
                    if (sum > 200) {
                        sum = sum / 4;
                    }
                }
            }
            counter = counter + 1;
        }
    }
    
    // Final crazy nested combination
    if (a + b > c * d + 30) {
        counter = 0;
        while (counter < 4) {
            if (counter == 0) {
                temp = a;
            } else {
                if (counter == 1) {
                    temp = b;
                } else {
                    if (counter == 2) {
                        temp = c;
                    } else {
                        temp = d;
                    }
                }
            }
            
            while (temp > 0) {
                result = result + temp / 2 + 1;
                temp = temp - 3;
                
                if (temp < 5) {
                    if (temp > 0) {
                        result = result * temp;
                    } else {
                        result = result + 7;
                    }
                }
            }
            
            counter = counter + 1;
        }
    }
    
    // Test operator precedence in conditions
    if (a + b * c > d + result / 2) {
        if (factorial - sum * 2 < a * b + c) {
            result = result + factorial + sum;
        } else {
            result = result - factorial + sum;
        }
    }
    
    return result + sum + factorial;
}