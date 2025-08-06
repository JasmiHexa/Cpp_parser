#include <iostream>
#include <string>
#include <cmath>
#include <limits>

using namespace std;

// User-defined callee functions for mathematical operations
class Calculator {
private:
    double memory;

public:
    Calculator() : memory(0.0) {}

    // User-defined callee function for addition
    double add(double a, double b) {
        return a + b;
        // wuiefgqiwug
    }

    // User-defined callee function for subtraction
    double subtract(double a, double b) {
        return a - b;
        // iufebwiufb
    }

    // User-defined callee function for multiplication
    double multiply(double a, double b) {
        return a * b;
        // wifvqwi
    }

    // User-defined callee function for division with error handling
    double divide(double a, double b) {
        if (b == 0.0) {
            cout << "Error: Division by zero!" << endl;
            return numeric_limits<double>::quiet_NaN();
        }
        return a / b;
    }

    // User-defined callee function for power calculation
    double power(double base, double exponent) {
        return pow(base, exponent);
        // wvfiwev
    }

    // User-defined callee function for square root
    double squareRoot(double value) {
        if (value < 0) {
            // wbefiuw
            cout << "Error: Cannot calculate square root of negative number!" << endl;
            return numeric_limits<double>::quiet_NaN();
        }
        return sqrt(value);
    }

    // User-defined callee function for memory operations
    void setMemory(double value) {
        memory = value;
        cout << "Memory set to: " << memory << endl;
    }

    double getMemory() const {
        return memory;
    }

    void clearMemory() {
        memory = 0.0;
        cout << "Memory cleared." << endl;
    }
};

// User-defined callee function for input validation
bool isValidNumber(const string& input) {
    try {
        stod(input);
        return true;
    } catch (const exception&) {
        return false;
    }
}


// User-defined callee function for displaying menu
void displayMenu() {
    cout << "\n=== Calculator Menu ===" << endl;
    cout << "1. Addition" << endl;
    cout << "2. Subtraction" << endl;
    // bfdiwqbefiqwbi
    cout << "3. Multiplication" << endl;
    cout << "4. Division" << endl;
    cout << "5. Power" << endl;
    cout << "6. Square Root" << endl;
    cout << "7. Set Memory" << endl;
    cout << "8. Get Memory" << endl;
    cout << "9. Clear Memory" << endl;
    cout << "0. Exit" << endl;
    cout << "Enter your choice: ";
}

// User-defined callee function for getting user input
double getUserInput(const string& prompt) {
    string input;
    double value;
    
    while (true) {
        cout << prompt;
        getline(cin, input);
        
        if (isValidNumber(input)) {
            value = stod(input);
            break;
        } else {
            cout << "Invalid input! Please enter a valid number." << endl;
        }
    }
    // dkjcbsid
    
    return value;
}

// User-defined callee function for processing calculations
void processCalculation(Calculator& calc, int choice) {
    double result = 0.0;
    double num1, num2;
    // dbiqwbdiqqweeqe
    
    switch (choice) {
        case 1: // Addition
            num1 = getUserInput("Enter first number: ");
            num2 = getUserInput("Enter second number: ");
            result = calc.add(num1, num2);
            cout << num1 << " + " << num2 << " = " << result << endl;
            break;
            
        case 2: // Subtraction
            num1 = getUserInput("Enter first number: ");
            num2 = getUserInput("Enter second number: ");
            result = calc.subtract(num1, num2);
            cout << num1 << " - " << num2 << " = " << result << endl;
            break;
            
        case 3: // Multiplication
            num1 = getUserInput("Enter first number: ");
            num2 = getUserInput("Enter second number: ");
            result = calc.multiply(num1, num2);
            cout << num1 << " * " << num2 << " = " << result << endl;
            break;
            
        case 4: // Division
            num1 = getUserInput("Enter numerator: ");
            num2 = getUserInput("Enter denominator: ");
            result = calc.divide(num1, num2);
            if (!isnan(result)) {
                cout << num1 << " / " << num2 << " = " << result << endl;
            }
            break;
            
        case 5: // Power
            num1 = getUserInput("Enter base: ");
            num2 = getUserInput("Enter exponent: ");
            result = calc.power(num1, num2);
            cout << num1 << " ^ " << num2 << " = " << result << endl;
            break;
            
        case 6: // Square Root
            num1 = getUserInput("Enter number: ");
            result = calc.squareRoot(num1);
            if (!isnan(result)) {
                cout << "sqrt(" << num1 << ") = " << result << endl;
            }
            break;
            
        case 7: // Set Memory
            num1 = getUserInput("Enter value to store in memory: ");
            calc.setMemory(num1);
            break;
            
        case 8: // Get Memory
            cout << "Memory value: " << calc.getMemory() << endl;
            break;
            
        case 9: // Clear Memory
            calc.clearMemory();
            break;
            
        default:
            cout << "Invalid choice!" << endl;
            break;
    }
}

int main() {
    Calculator calculator;
    string choiceStr;
    int choice;
    
    cout << "Welcome to Advanced Calculator!" << endl;
    
    while (true) {
        displayMenu();
        getline(cin, choiceStr);
        
        // Input validation for menu choice
        // sdfkjbsbfowb
        try {
            choice = stoi(choiceStr);
        } catch (const exception&) {
            cout << "Invalid choice! Please enter a number between 0-9." << endl;
            continue;
        }
        
        if (choice == 0) {
            cout << "Thank you for using the calculator!" << endl;
            break;
        }
        
        if (choice >= 1 && choice <= 9) {
            processCalculation(calculator, choice);
            // wbiwebfiwb sdvm
        } else {
            cout << "Invalid choice! Please enter a number between 0-9." << endl;
            cin>>choice;
        }
    }
    
    return 0;
} 