#include <iostream>
#include <cmath>

using namespace std;

// Forward declarations for functions in math_utils.cpp
extern double square_root(double x);
extern double power(double base, double exponent);
extern int factorial(int n);

// Function to calculate the area of a circle
double calculate_circle_area(double radius) {
    cout << "Calculating circle area with radius: " << radius << endl;
    double pi = 3.14159;
    double area = pi * power(radius, 2);
    cout << "Circle area: " << area << endl;
    return area;
}

// Function to calculate the hypotenuse of a right triangle
double calculate_hypotenuse(double a, double b) {
    cout << "Calculating hypotenuse for sides: " << a << " and " << b << endl;
    double sum_squares = power(a, 2) + power(b, 2);
    double hypotenuse = square_root(sum_squares);
    cout << "Hypotenuse: " << hypotenuse << endl;
    return hypotenuse;
}

// Function to calculate combinations (nCr)
int calculate_combinations(int n, int r) {
    cout << "Calculating combinations C(" << n << ", " << r << ")" << endl;
    if (r > n) return 0;
    if (r == 0 || r == n) return 1;
    
    int numerator = factorial(n);
    int denominator = factorial(r) * factorial(n - r);
    int result = numerator / denominator;
    cout << "Combinations result: " << result << endl;
    return result;
}

// Function to calculate compound interest
double calculate_compound_interest(double principal, double rate, int time) {
    cout << "Calculating compound interest for principal: " << principal 
         << ", rate: " << rate << ", time: " << time << endl;
    double amount = principal * power(1 + rate, time);
    double interest = amount - principal;
    cout << "Compound interest: " << interest << endl;
    return interest;
}

// Main function to demonstrate cross-file function calls
int main() {
    cout << "=== Calculator Functions Demo ===" << endl;
    
    // Test circle area calculation
    double circle_area = calculate_circle_area(5.0);
    
    // Test hypotenuse calculation
    double hyp = calculate_hypotenuse(3.0, 4.0);
    
    // Test combinations calculation
    int comb = calculate_combinations(5, 2);
    
    // Test compound interest calculation
    double interest = calculate_compound_interest(1000.0, 0.05, 3);
    
    cout << "\n=== Summary ===" << endl;
    cout << "Circle area: " << circle_area << endl;
    cout << "Hypotenuse: " << hyp << endl;
    cout << "Combinations: " << comb << endl;
    cout << "Compound interest: " << interest << endl;
    
    return 0;
} 