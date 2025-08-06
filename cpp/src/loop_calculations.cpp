#include <iostream>
#include <iomanip>
#include <cmath>
#include <limits>

using namespace std;

class LoopCalculator {
private:
    int number;
    double result;
    
public:
    LoopCalculator() : number(0), result(0.0) {}
    
    // Input method with validation
    void inputNumber() {
        cout << "Enter a positive number: ";
        while (!(cin >> number) || number <= 0) {
            cout << "Invalid input! Please enter a positive number: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    
    // FOR LOOP examples
    void demonstrateForLoop() {
        cout << "\n=== FOR LOOP EXAMPLES ===" << endl;
        
        // 1. Simple counting
        cout << "1. Counting from 1 to " << number << ":" << endl;
        for (int i = 1; i <= number; i++) {
            cout << i << " ";
        }
        cout << endl;
        
        // 2. Sum calculation using for loop
        int sum = 0;
        for (int i = 1; i <= number; i++) {
            sum += i;
        }
        cout << "2. Sum of numbers from 1 to " << number << " = " << sum << endl;
        
        // 3. Factorial calculation
        long long factorial = 1;
        for (int i = 1; i <= number; i++) {
            factorial *= i;
        }
        cout << "3. Factorial of " << number << " = " << factorial << endl;
        
        // 4. Power calculation
        int power;
        cout << "Enter power to calculate " << number << "^power: ";
        cin >> power;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        long long powerResult = 1;
        for (int i = 0; i < power; i++) {
            powerResult *= number;
        }
        cout << "4. " << number << "^" << power << " = " << powerResult << endl;
    }
    
    // WHILE LOOP examples
    void demonstrateWhileLoop() {
        cout << "\n=== WHILE LOOP EXAMPLES ===" << endl;
        
        // 1. Countdown
        cout << "1. Countdown from " << number << ":" << endl;
        int countdown = number;
        while (countdown > 0) {
            cout << countdown << " ";
            countdown--;
        }
        cout << "Blast off!" << endl;
        
        // 2. Find divisors
        cout << "2. Divisors of " << number << ":" << endl;
        int divisor = 1;
        int divisorCount = 0;
        while (divisor <= number) {
            if (number % divisor == 0) {
                cout << divisor << " ";
                divisorCount++;
            }
            divisor++;
        }
        cout << endl;
        cout << "Total divisors: " << divisorCount << endl;
        
        // 3. Fibonacci series
        cout << "3. Fibonacci series up to " << number << " terms:" << endl;
        int a = 0, b = 1, count = 0;
        while (count < number) {
            cout << a << " ";
            int temp = a + b;
            a = b;
            b = temp;
            count++;
        }
        cout << endl;
    }
    
    // DO-WHILE LOOP examples
    void demonstrateDoWhileLoop() {
        cout << "\n=== DO-WHILE LOOP EXAMPLES ===" << endl;
        
        // 1. Menu-driven calculator
        char choice;
        do {
            cout << "\n--- Calculator Menu ---" << endl;
            cout << "1. Add " << number << " to another number" << endl;
            cout << "2. Multiply " << number << " by another number" << endl;
            cout << "3. Calculate square root of " << number << endl;
            cout << "4. Exit" << endl;
            cout << "Enter your choice (1-4): ";
            cin >> choice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            
            switch (choice) {
                case '1': {
                    int num2;
                    cout << "Enter another number: ";
                    cin >> num2;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << number << " + " << num2 << " = " << (number + num2) << endl;
                    break;
                }
                case '2': {
                    int num2;
                    cout << "Enter another number: ";
                    cin >> num2;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << number << " * " << num2 << " = " << (number * num2) << endl;
                    break;
                }
                case '3': {
                    double sqrtResult = sqrt(number);
                    cout << "Square root of " << number << " = " << fixed << setprecision(4) << sqrtResult << endl;
                    break;
                }
                case '4':
                    cout << "Exiting calculator..." << endl;
                    break;
                default:
                    cout << "Invalid choice! Please try again." << endl;
            }
        } while (choice != '4');
        
        // 2. Number guessing game
        cout << "\n--- Number Guessing Game ---" << endl;
        int secretNumber = number % 10 + 1; // Use last digit + 1 as secret number
        int guess, attempts = 0;
        
        do {
            cout << "Guess the number (1-10): ";
            cin >> guess;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            attempts++;
            
            if (guess < secretNumber) {
                cout << "Too low! Try again." << endl;
            } else if (guess > secretNumber) {
                cout << "Too high! Try again." << endl;
            } else {
                cout << "Congratulations! You guessed it in " << attempts << " attempts!" << endl;
            }
        } while (guess != secretNumber);
    }
    
    // Advanced calculations with loops
    void advancedCalculations() {
        cout << "\n=== ADVANCED CALCULATIONS ===" << endl;
        
        // 1. Prime number check
        bool isPrime = true;
        if (number <= 1) {
            isPrime = false;
        } else {
            for (int i = 2; i <= sqrt(number); i++) {
                if (number % i == 0) {
                    isPrime = false;
                    break;
                }
            }
        }
        cout << "1. " << number << " is " << (isPrime ? "prime" : "not prime") << endl;
        
        // 2. Sum of digits
        int temp = number;
        int digitSum = 0;
        while (temp > 0) {
            digitSum += temp % 10;
            temp /= 10;
        }
        cout << "2. Sum of digits of " << number << " = " << digitSum << endl;
        
        // 3. Reverse number
        temp = number;
        int reversed = 0;
        while (temp > 0) {
            reversed = reversed * 10 + temp % 10;
            temp /= 10;
        }
        cout << "3. Reverse of " << number << " = " << reversed << endl;
        
        // 4. Check if palindrome
        bool isPalindrome = (number == reversed);
        cout << "4. " << number << " is " << (isPalindrome ? "a palindrome" : "not a palindrome") << endl;
    }
    
    // Display all results
    void runAllDemonstrations() {
        cout << "Welcome to Loop Calculator!" << endl;
        cout << "This program demonstrates various loops and calculations." << endl;
        
        inputNumber();
        
        demonstrateForLoop();
        demonstrateWhileLoop();
        demonstrateDoWhileLoop();
        advancedCalculations();
        
        cout << "\n=== PROGRAM COMPLETED ===" << endl;
        cout << "Thank you for using Loop Calculator!" << endl;
    }
};

int main() {
    LoopCalculator calculator;
    calculator.runAllDemonstrations();
    return 0;
} 