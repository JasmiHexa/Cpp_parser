#include <iostream>
#include <cmath>
#include <iomanip>
#include <vector>
#include <algorithm>

using namespace std;

// Function to calculate factorial
long long factorial(int n) {
    if (n <= 1) return 1;
    return n * factorial(n - 1);
}

// Function to calculate fibonacci number
int fibonacci(int n) {
    if (n <= 1) return n;
    return fibonacci(n - 1) + fibonacci(n - 2);
}

// Function to calculate compound interest
double compoundInterest(double principal, double rate, int time, int compoundsPerYear) {
    return principal * pow(1 + rate / compoundsPerYear, compoundsPerYear * time);
}

// Function to calculate area of different shapes
double calculateArea(const string& shape, double a, double b = 0) {
    if (shape == "circle") {
        return M_PI * a * a;
    } else if (shape == "rectangle") {
        return a * b;
    } else if (shape == "triangle") {
        return 0.5 * a * b;
    } else if (shape == "square") {
        return a * a;
    }
    return 0;
}

// Function to calculate statistics
struct Statistics {
    double mean;
    double median;
    double standardDeviation;
};

Statistics calculateStatistics(const vector<double>& numbers) {
    Statistics stats;
    
    // Calculate mean
    double sum = 0;
    for (double num : numbers) {
        sum += num;
    }
    stats.mean = sum / numbers.size();
    
    // Calculate median
    vector<double> sorted = numbers;
    sort(sorted.begin(), sorted.end());
    if (sorted.size() % 2 == 0) {
        stats.median = (sorted[sorted.size()/2 - 1] + sorted[sorted.size()/2]) / 2;
    } else {
        stats.median = sorted[sorted.size()/2];
    }
    
    // Calculate standard deviation
    double variance = 0;
    for (double num : numbers) {
        variance += pow(num - stats.mean, 2);
    }
    stats.standardDeviation = sqrt(variance / numbers.size());
    
    return stats;
}

int main() {
    cout << "=== C++ Calculator with Various Calculations ===\n\n";
    
    // 1. Basic Arithmetic Operations
    cout << "1. Basic Arithmetic Operations:\n";
    double a = 15.5, b = 7.3;
    cout << "a = " << a << ", b = " << b << "\n";
    cout << "Addition: " << a << " + " << b << " = " << a + b << "\n";
    cout << "Subtraction: " << a << " - " << b << " = " << a - b << "\n";
    cout << "Multiplication: " << a << " * " << b << " = " << a * b << "\n";
    cout << "Division: " << a << " / " << b << " = " << fixed << setprecision(2) << a / b << "\n";
    cout << "Modulo: " << (int)a << " % " << (int)b << " = " << (int)a % (int)b << "\n";
    cout << "Power: " << a << " ^ " << b << " = " << pow(a, b) << "\n\n";
    
    // 2. Mathematical Functions
    cout << "2. Mathematical Functions:\n";
    double angle = 45.0;
    double radians = angle * M_PI / 180.0;
    cout << "Angle: " << angle << " degrees\n";
    cout << "Sine: sin(" << angle << "°) = " << sin(radians) << "\n";
    cout << "Cosine: cos(" << angle << "°) = " << cos(radians) << "\n";
    cout << "Tangent: tan(" << angle << "°) = " << tan(radians) << "\n";
    cout << "Square root of " << a << " = " << sqrt(a) << "\n";
    cout << "Natural log of " << a << " = " << log(a) << "\n";
    cout << "Log base 10 of " << a << " = " << log10(a) << "\n\n";
    
    // 3. Factorial and Fibonacci
    cout << "3. Factorial and Fibonacci:\n";
    int n = 10;
    cout << "Factorial of " << n << " = " << factorial(n) << "\n";
    cout << "Fibonacci sequence (first " << n << " terms): ";
    for (int i = 0; i < n; i++) {
        cout << fibonacci(i) << " ";
    }
    cout << "\n\n";
    
    // 4. Compound Interest Calculation
    cout << "4. Compound Interest Calculation:\n";
    double principal = 10000.0;
    double rate = 0.05; // 5% annual rate
    int time = 5; // 5 years
    int compoundsPerYear = 12; // monthly compounding
    
    double finalAmount = compoundInterest(principal, rate, time, compoundsPerYear);
    cout << "Principal: $" << principal << "\n";
    cout << "Annual Rate: " << rate * 100 << "%\n";
    cout << "Time: " << time << " years\n";
    cout << "Compounding: " << compoundsPerYear << " times per year\n";
    cout << "Final Amount: $" << fixed << setprecision(2) << finalAmount << "\n";
    cout << "Interest Earned: $" << finalAmount - principal << "\n\n";
    
    // 5. Area Calculations
    cout << "5. Area Calculations:\n";
    cout << "Circle (radius = 5): " << calculateArea("circle", 5) << " square units\n";
    cout << "Rectangle (5 x 8): " << calculateArea("rectangle", 5, 8) << " square units\n";
    cout << "Triangle (base=6, height=4): " << calculateArea("triangle", 6, 4) << " square units\n";
    cout << "Square (side=7): " << calculateArea("square", 7) << " square units\n\n";
    
    // 6. Statistics Calculation
    cout << "6. Statistics Calculation:\n";
    vector<double> data = {12.5, 15.2, 18.7, 14.1, 16.8, 13.9, 17.3, 19.2, 11.8, 20.1};
    cout << "Data set: ";
    for (double num : data) {
        cout << num << " ";
    }
    cout << "\n";
    
    Statistics stats = calculateStatistics(data);
    cout << "Mean: " << fixed << setprecision(2) << stats.mean << "\n";
    cout << "Median: " << stats.median << "\n";
    cout << "Standard Deviation: " << stats.standardDeviation << "\n\n";
    
    // 7. Temperature Conversion
    cout << "7. Temperature Conversion:\n";
    double celsius = 25.0;
    double fahrenheit = (celsius * 9/5) + 32;
    double kelvin = celsius + 273.15;
    
    cout << celsius << "°C = " << fahrenheit << "°F\n";
    cout << celsius << "°C = " << kelvin << "K\n\n";
    
    // 8. Simple Calculator Loop
    cout << "8. Interactive Calculator (Enter 'q' to quit):\n";
    char operation;
    double num1, num2;
    
    while (true) {
        cout << "Enter operation (+, -, *, /, ^, sqrt) or 'q' to quit: ";
        cin >> operation;
        
        if (operation == 'q' || operation == 'Q') {
            break;
        }
        
        if (operation == 'sqrt') {
            cout << "Enter number: ";
            cin >> num1;
            cout << "√" << num1 << " = " << sqrt(num1) << "\n";
        } else {
            cout << "Enter first number: ";
            cin >> num1;
            cout << "Enter second number: ";
            cin >> num2;
            
            switch (operation) {
                case '+':
                    cout << num1 << " + " << num2 << " = " << num1 + num2 << "\n";
                    break;
                case '-':
                    cout << num1 << " - " << num2 << " = " << num1 - num2 << "\n";
                    break;
                case '*':
                    cout << num1 << " * " << num2 << " = " << num1 * num2 << "\n";
                    break;
                case '/':
                    if (num2 != 0) {
                        cout << num1 << " / " << num2 << " = " << num1 / num2 << "\n";
                    } else {
                        cout << "Error: Division by zero!\n";
                    }
                    break;
                case '^':
                    cout << num1 << " ^ " << num2 << " = " << pow(num1, num2) << "\n";
                    break;
                default:
                    cout << "Invalid operation!\n";
            }
        }
        cout << "\n";
    }
    
    // 9. Ternary Operator Examples
    cout << "9. Ternary Operator Examples:\n";
    cout << "Max of a and b: " << ((a > b) ? a : b) << "\n";
    int sample = 7;
    cout << sample << " is " << ((sample % 2 == 0) ? "even" : "odd") << "\n\n";

    cout << "Thank you for using the C++ Calculator!\n";
    return 0;
}