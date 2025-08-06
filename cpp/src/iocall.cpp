#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

class FileProcessor {
private:
    string filename;
    ifstream inputFile;
    ofstream outputFile;
    fstream dataFile;

public:
    // Constructor with file operations
    FileProcessor(const string& fname) : filename(fname) {
        cout << "Initializing FileProcessor for: " << filename << endl;
    }

    // Console I/O operations
    void getUserInput() {
        string name;
        int age;
        double salary;

        cout << "Enter your name: ";
        cin >> name;

        cout << "Enter your age: ";
        cin >> age;

        cout << "Enter your salary: ";
        cin >> salary;

        // Input validation
        if (cin.fail()) {
            cin.clear();
            cin.ignore(512, '\n');
            cout << "Invalid input! Please try again." << endl;
            return;
        }

        cout << "\nUser Information:" << endl;
        cout << "Name: " << name << endl;
        cout << "Age: " << age << endl;
        cout << "Salary: $" << fixed << setprecision(2) << salary << endl;
    }

    // File reading operations
    bool readFromFile() {
        inputFile.open(filename);
        if (!inputFile.is_open()) {
            cout << "Error: Cannot open file " << filename << " for reading" << endl;
            return false;
        }

        string line;
        cout << "Reading from file: " << filename << endl;
        while (getline(inputFile, line)) {
            cout << line << endl;
        }

        inputFile.close();
        return true;
    }

    // File writing operations
    bool writeToFile(const string& content) {
        outputFile.open(filename, ios::app); // Append mode
        if (!outputFile.is_open()) {
            cout << "Error: Cannot open file " << filename << " for writing" << endl;
            return false;
        }

        outputFile << content << endl;
        outputFile.flush(); // Ensure data is written immediately
        outputFile.close();
        
        cout << "Successfully wrote to file: " << filename << endl;
        return true;
    }

    // Binary file operations
    bool writeBinaryData(const vector<int>& data) {
        ofstream binFile(filename + ".bin", ios::binary);
        if (!binFile.is_open()) {
            cout << "Error: Cannot create binary file" << endl;
            return false;
        }

        binFile.write(reinterpret_cast<const char*>(data.data()), 
                     data.size() * sizeof(int));
        binFile.close();
        
        cout << "Binary data written successfully" << endl;
        return true;
    }

    bool readBinaryData(vector<int>& data) {
        ifstream binFile(filename + ".bin", ios::binary);
        if (!binFile.is_open()) {
            cout << "Error: Cannot open binary file" << endl;
            return false;
        }

        // Get file size
        binFile.seekg(0, ios::end);
        streamsize size = binFile.tellg();
        binFile.seekg(0, ios::beg);

        data.resize(size / sizeof(int));
        binFile.read(reinterpret_cast<char*>(data.data()), size);
        binFile.close();
        
        cout << "Binary data read successfully" << endl;
        return true;
    }

    // String stream operations
    void processStringStream() {
        string data = "John,25,75000.50";
        stringstream ss(data);
        
        string name;
        int age;
        double salary;
        char comma;

        // Parse CSV-like data
        getline(ss, name, ',');
        ss >> age >> comma >> salary;

        cout << "Parsed data from string stream:" << endl;
        cout << "Name: " << name << endl;
        cout << "Age: " << age << endl;
        cout << "Salary: $" << fixed << setprecision(2) << salary << endl;
    }

    // Formatted I/O operations
    void formattedIO() {
        int number = 42;
        double pi = 3.14159;
        string text = "Hello World";

        // Formatted output
        cout << setw(10) << left << "Number:" << setw(10) << right << number << endl;
        cout << setw(10) << left << "Pi:" << setw(10) << right << fixed << setprecision(3) << pi << endl;
        cout << setw(10) << left << "Text:" << setw(15) << right << text << endl;

        // Using printf-style formatting
        printf("Formatted: Number=%d, Pi=%.3f, Text=%s\n", number, pi, text.c_str());
    }

    // Error handling with I/O
    void robustFileOperations() {
        string testFile = "test_data.txt";
        
        // Try to open file for reading
        ifstream testInput(testFile);
        if (testInput.fail()) {
            cout << "File " << testFile << " does not exist. Creating it..." << endl;
            
            // Create the file
            ofstream testOutput(testFile);
            if (testOutput.is_open()) {
                testOutput << "Sample data line 1" << endl;
                testOutput << "Sample data line 2" << endl;
                testOutput << "Sample data line 3" << endl;
                testOutput.close();
                cout << "File created successfully" << endl;
            }
        } else {
            testInput.close();
        }

        // Now read the file
        ifstream readTest(testFile);
        if (readTest.is_open()) {
            string line;
            int lineCount = 0;
            while (getline(readTest, line)) {
                lineCount++;
                cout << "Line " << lineCount << ": " << line << endl;
            }
            readTest.close();
        }
    }

    // Destructor
    ~FileProcessor() {
        if (inputFile.is_open()) inputFile.close();
        if (outputFile.is_open()) outputFile.close();
        if (dataFile.is_open()) dataFile.close();
        cout << "FileProcessor destroyed" << endl;
    }
};

int main() {
    cout << "=== C++ I/O Operations Examples ===" << endl << endl;

    // Create FileProcessor instance
    FileProcessor processor("sample.txt");

    // Demonstrate various I/O operations
    cout << "1. Console I/O Operations:" << endl;
    processor.getUserInput();
    cout << endl;

    cout << "2. String Stream Processing:" << endl;
    processor.processStringStream();
    cout << endl;

    cout << "3. Formatted I/O:" << endl;
    processor.formattedIO();
    cout << endl;

    cout << "4. File Operations:" << endl;
    processor.writeToFile("This is a test line written by the program");
    processor.readFromFile();
    cout << endl;

    cout << "5. Binary File Operations:" << endl;
    vector<int> data = {1, 2, 3, 4, 5, 10, 20, 30, 40, 50};
    processor.writeBinaryData(data);
    
    vector<int> readData;
    processor.readBinaryData(readData);
    
    cout << "Read binary data: ";
    for (int val : readData) {
        cout << val << " ";
    }
    cout << endl << endl;

    cout << "6. Robust File Operations:" << endl;
    processor.robustFileOperations();
    cout << endl;

    cout << "=== Program completed successfully ===" << endl;
    return 0;
} 