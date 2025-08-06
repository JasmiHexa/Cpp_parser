#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <fstream>
#include <sstream>

using namespace std;

// Function declarations for functions in other files
void validateInput(const string& input);
void processData(const string& data);
void calculateMetrics(const vector<double>& values);

// Functions in this file
void displayResults(const string& title, const vector<string>& results) {
    cout << "File3: Displaying results for: " << title << endl;
    cout << "File3: " << string(50, '=') << endl;
    
    if (results.empty()) {
        cout << "File3: No results to display" << endl;
        return;
    }
    
    for (size_t i = 0; i < results.size(); ++i) {
        cout << "File3: [" << setw(2) << (i + 1) << "] " << results[i] << endl;
    }
    
    cout << "File3: " << string(50, '=') << endl;
    cout << "File3: Total results displayed: " << results.size() << endl;
    
    // Call function from file1
    string summary = "Displayed " + to_string(results.size()) + " results";
    validateInput(summary);
}

void saveToFile(const string& filename, const vector<string>& data) {
    cout << "File3: Saving " << data.size() << " items to file: " << filename << endl;
    
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "File3: Failed to open file for writing" << endl;
        return;
    }
    
    file << "=== Data Export ===" << endl;
    file << "Timestamp: " << __DATE__ << " " << __TIME__ << endl;
    file << "Total items: " << data.size() << endl;
    file << endl;
    
    for (size_t i = 0; i < data.size(); ++i) {
        file << (i + 1) << ". " << data[i] << endl;
    }
    
    file.close();
    cout << "File3: Data saved successfully to " << filename << endl;
    
    // Call function from file2
    processData("File saved: " + filename);
}

void formatOutput(const vector<double>& values, const string& format) {
    cout << "File3: Formatting " << values.size() << " values with format: " << format << endl;
    
    if (values.empty()) {
        cout << "File3: No values to format" << endl;
        return;
    }
    
    cout << "File3: Formatted output:" << endl;
    
    if (format == "table") {
        cout << "File3: " << setw(10) << "Index" << setw(15) << "Value" << setw(15) << "Squared" << endl;
        cout << "File3: " << string(40, '-') << endl;
        
        for (size_t i = 0; i < values.size(); ++i) {
            cout << "File3: " << setw(10) << i << setw(15) << fixed << setprecision(2) 
                 << values[i] << setw(15) << values[i] * values[i] << endl;
        }
    } else if (format == "list") {
        for (size_t i = 0; i < values.size(); ++i) {
            cout << "File3: [" << (i + 1) << "] " << fixed << setprecision(3) << values[i] << endl;
        }
    } else {
        cout << "File3: Unknown format, using default list format" << endl;
        for (double value : values) {
            cout << "File3: " << value << endl;
        }
    }
    
    // Call function from file2
    calculateMetrics(values);
}

void createSummary(const vector<string>& items) {
    cout << "File3: Creating summary for " << items.size() << " items" << endl;
    
    if (items.empty()) {
        cout << "File3: No items to summarize" << endl;
        return;
    }
    
    vector<string> summary;
    summary.push_back("Summary Report");
    summary.push_back("Generated: " + string(__DATE__) + " " + string(__TIME__));
    summary.push_back("Total items: " + to_string(items.size()));
    
    // Count items by first character
    map<char, int> charCount;
    for (const string& item : items) {
        if (!item.empty()) {
            charCount[tolower(item[0])]++;
        }
    }
    
    summary.push_back("Items by first letter:");
    for (const auto& pair : charCount) {
        summary.push_back("  " + string(1, pair.first) + ": " + to_string(pair.second));
    }
    
    // Call function from file3 (recursive call to displayResults)
    displayResults("File3 Summary", summary);
}

void exportData(const vector<int>& data, const string& format) {
    cout << "File3: Exporting " << data.size() << " integers in " << format << " format" << endl;
    
    if (data.empty()) {
        cout << "File3: No data to export" << endl;
        return;
    }
    
    stringstream ss;
    
    if (format == "csv") {
        for (size_t i = 0; i < data.size(); ++i) {
            ss << data[i];
            if (i < data.size() - 1) ss << ",";
        }
    } else if (format == "json") {
        ss << "[";
        for (size_t i = 0; i < data.size(); ++i) {
            ss << data[i];
            if (i < data.size() - 1) ss << ",";
        }
        ss << "]";
    } else {
        for (int value : data) {
            ss << value << " ";
        }
    }
    
    string exportedData = ss.str();
    cout << "File3: Exported data: " << exportedData << endl;
    
    // Call function from file1
    validateInput(exportedData);
}

// Main function for file3
int main() {
    cout << "=== File3 Main Function ===" << endl;
    
    // Test displayResults function
    cout << "\n1. Testing displayResults:" << endl;
    vector<string> testResults = {"Result 1", "Result 2", "Result 3", "Result 4"};
    displayResults("Test Results", testResults);
    displayResults("Empty Results", {});
    
    // Test saveToFile function
    cout << "\n2. Testing saveToFile:" << endl;
    vector<string> fileData = {"Line 1", "Line 2", "Line 3"};
    saveToFile("test_output.txt", fileData);
    
    // Test formatOutput function
    cout << "\n3. Testing formatOutput:" << endl;
    vector<double> formatValues = {1.234, 2.345, 3.456, 4.567};
    formatOutput(formatValues, "table");
    formatOutput(formatValues, "list");
    formatOutput(formatValues, "unknown");
    
    // Test createSummary function
    cout << "\n4. Testing createSummary:" << endl;
    vector<string> summaryItems = {"Apple", "Banana", "Cherry", "Date", "Elderberry"};
    createSummary(summaryItems);
    createSummary({});
    
    // Test exportData function
    cout << "\n5. Testing exportData:" << endl;
    vector<int> exportData = {10, 20, 30, 40, 50};
    exportData(exportData, "csv");
    exportData(exportData, "json");
    exportData(exportData, "space");
    
    cout << "\n=== File3 Main Function Completed ===" << endl;
    return 0;
}