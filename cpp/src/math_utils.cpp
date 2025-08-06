#include <iostream>
#include <cmath>

using namespace std;

// Forward declarations for functions in calculator.cpp
extern double calculate_circle_area(double radius);
extern double calculate_hypotenuse(double a, double b);
extern int calculate_combinations(int n, int r);
extern double calculate_compound_interest(double principal, double rate, int time);

// Function to calculate square root
double square_root(double x) {
    cout << "  Computing square root of: " << x << endl;
    if (x < 0) {
        cout << "  Error: Cannot calculate square root of negative number" << endl;
        return 0.0;
    }
    double result = sqrt(x);
    cout << "  Square root result: " << result << endl;
    return result;
}

// Function to calculate power
double power(double base, double exponent) {
    cout << "  Computing " << base << " raised to power " << exponent << endl;
    double result = pow(base, exponent);
    cout << "  Power result: " << result << endl;
    return result;
}

// Function to calculate factorial
int factorial(int n) {
    cout << "  Computing factorial of: " << n << endl;
    if (n < 0) {
        cout << "  Error: Cannot calculate factorial of negative number" << endl;
        return 0;
    }
    if (n == 0 || n == 1) {
        cout << "  Factorial result: 1" << endl;
        return 1;
    }
    
    int result = 1;
    for (int i = 2; i <= n; i++) {
        result *= i;
    }
    cout << "  Factorial result: " << result << endl;
    return result;
}

// Function to calculate absolute value
double absolute_value(double x) {
    cout << "  Computing absolute value of: " << x << endl;
    double result = abs(x);
    cout << "  Absolute value result: " << result << endl;
    return result;
}

// Function to calculate logarithm
double logarithm(double x, double base = 10.0) {
    cout << "  Computing log base " << base << " of: " << x << endl;
    if (x <= 0) {
        cout << "  Error: Cannot calculate logarithm of non-positive number" << endl;
        return 0.0;
    }
    double result = log(x) / log(base);
    cout << "  Logarithm result: " << result << endl;
    return result;
}

// Function to demonstrate calling calculator functions
void demonstrate_calculator_functions() {
    cout << "\n=== Math Utils calling Calculator Functions ===" << endl;
    
    // Call calculator functions from math_utils
    double area = calculate_circle_area(3.0);
    double hyp = calculate_hypotenuse(5.0, 12.0);
    int comb = calculate_combinations(6, 3);
    double interest = calculate_compound_interest(500.0, 0.08, 2);
    
    cout << "\n=== Math Utils Summary ===" << endl;
    cout << "Called circle area calculation: " << area << endl;
    cout << "Called hypotenuse calculation: " << hyp << endl;
    cout << "Called combinations calculation: " << comb << endl;
    cout << "Called compound interest calculation: " << interest << endl;
}

// Function to perform complex mathematical operations
double complex_calculation(double x, double y) {
    cout << "Performing complex calculation with x=" << x << " and y=" << y << endl;
    
    // Use our own utility functions
    double abs_x = absolute_value(x);
    double abs_y = absolute_value(y);
    double power_x = power(abs_x, 2);
    double power_y = power(abs_y, 2);
    double sum = power_x + power_y;
    double result = square_root(sum);
    
    // Also call a calculator function
    double circle_area = calculate_circle_area(result);
    
    cout << "Complex calculation result: " << result << endl;
    cout << "Associated circle area: " << circle_area << endl;
    
    return result;
}

// Main function for math_utils.cpp
int main() {
    cout << "=== Math Utils Functions Demo ===" << endl;
    
    // Test basic math utility functions
    cout << "\nTesting basic math functions:" << endl;
    double sqrt_result = square_root(16.0);
    double pow_result = power(2.0, 8.0);
    int fact_result = factorial(5);
    double abs_result = absolute_value(-15.7);
    double log_result = logarithm(100.0);
    
    cout << "\n=== Math Utils Summary ===" << endl;
    cout << "Square root of 16: " << sqrt_result << endl;
    cout << "2^8: " << pow_result << endl;
    cout << "5!: " << fact_result << endl;
    cout << "|-15.7|: " << abs_result << endl;
    cout << "log10(100): " << log_result << endl;
    
    // Test complex calculation
    cout << "\nTesting complex calculation:" << endl;
    double complex_result = complex_calculation(3.0, 4.0);
    cout << "Complex calculation final result: " << complex_result << endl;
    
    return 0;
} 