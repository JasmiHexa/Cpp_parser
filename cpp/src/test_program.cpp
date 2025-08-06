#include <iostream>

using namespace std;

// Forward declarations
extern void demonstrate_calculator_functions();
extern double complex_calculation(double x, double y);

int main() {
    cout << "=== Cross-File Function Call Demonstration ===" << endl;
    
    // Test the complex calculation function from math_utils.cpp
    cout << "\nTesting complex calculation:" << endl;
    double result1 = complex_calculation(3.0, 4.0);
    
    // Test the calculator functions demonstration
    demonstrate_calculator_functions();
    
    // Test another complex calculation
    cout << "\nTesting another complex calculation:" << endl;
    double result2 = complex_calculation(-5.0, 12.0);
    
    cout << "\n=== Final Results ===" << endl;
    cout << "Complex calculation 1 result: " << result1 << endl;
    cout << "Complex calculation 2 result: " << result2 << endl;
    
    return 0;
} 