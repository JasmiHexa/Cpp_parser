#include <iostream>
#include "math_functions.h"

using namespace std;

int main() {
    cout << "=== Example Usage of Functions from Multiple Files ===" << endl;
    
    // Using functions from file_a.cpp
    int sum = add(10, 20);
    cout << "Sum: " << sum << endl;
    
    int numbers[] = {1, 2, 3, 4, 5};
    displayArray(numbers, 5);
    
    double avg = calculateAverage(numbers, 5);
    cout << "Average: " << avg << endl;
    
    // Using functions from file_b.cpp
    int product = multiply(5, 6);
    cout << "Product: " << product << endl;
    
    string message = getMessage();
    cout << "Message: " << message << endl;
    
    int powerResult = power(2, 3);
    cout << "2^3 = " << powerResult << endl;
    
    printTable(5);
    
    return 0;
} 