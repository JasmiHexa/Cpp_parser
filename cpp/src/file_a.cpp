#include <iostream>
#include <string>

using namespace std;

// Function declarations for functions defined in file_b.cpp
extern int multiply(int a, int b);
extern string getMessage();

// Function defined in this file that will be used in file_b.cpp
int add(int a, int b) {
    return a + b;
}

double calculateAverage(int arr[], int size) {
    if (size <= 0) return 0.0;
    
    int sum = 0;
    for (int i = 0; i < size; i++) {
        sum += arr[i];
    }
    return static_cast<double>(sum) / size;
}

void displayArray(int arr[], int size) {
    cout << "Array elements: ";
    for (int i = 0; i < size; i++) {
        cout << arr[i];
        if (i < size - 1) cout << ", ";
    }
    cout << endl;
}

int main() {
    cout << "=== File A Main Function ===" << endl;
    
    // Using functions from this file
    int result1 = add(10, 20);
    cout << "Addition result: " << result1 << endl;
    
    int numbers[] = {5, 10, 15, 20, 25};
    int size = 5;
    displayArray(numbers, size);
    
    double avg = calculateAverage(numbers, size);
    cout << "Average: " << avg << endl;
    
    // Using functions from file_b.cpp
    int result2 = multiply(5, 6);
    cout << "Multiplication result: " << result2 << endl;
    
    string message = getMessage();
    cout << "Message from file B: " << message << endl;
    
    return 0;
} 