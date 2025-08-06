#include <iostream>
#include <string>
#include <cmath>

using namespace std;

// Function declarations for functions defined in file_a.cpp
extern int add(int a, int b);
extern double calculateAverage(int arr[], int size);
extern void displayArray(int arr[], int size);

// Function defined in this file that will be used in file_a.cpp
int multiply(int a, int b) {
    return a * b;
}

string getMessage() {
    return "Hello from File B!";
}

int power(int base, int exponent) {
    return static_cast<int>(pow(base, exponent));
}

void printTable(int n) {
    cout << "Multiplication table for " << n << ":" << endl;
    for (int i = 1; i <= 10; i++) {
        cout << n << " x " << i << " = " << multiply(n, i) << endl;
    }
}

int main() {
    cout << "=== File B Main Function ===" << endl;
    
    // Using functions from this file
    int result1 = multiply(8, 9);
    cout << "Multiplication result: " << result1 << endl;
    
    int powerResult = power(2, 5);
    cout << "2^5 = " << powerResult << endl;
    
    printTable(7);
    
    // Using functions from file_a.cpp
    int result2 = add(15, 25);
    cout << "Addition result: " << result2 << endl;
    
    int data[] = {2, 4, 6, 8, 10, 12};
    int size = 6;
    displayArray(data, size);
    
    double avg = calculateAverage(data, size);
    cout << "Average of data: " << avg << endl;
    
    return 0;
} 