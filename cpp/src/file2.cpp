#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;

// Function declarations for functions in other files
void validateInput(const string& input);
void generateReport(const string& reportType);
void displayResults(const string& title, const vector<string>& results);

// Functions in this file
void processData(const string& data) {
    cout << "File2: Processing data: " << data << endl;
    
    if (data.length() > 50) {
        cout << "File2: Data is too long, truncating..." << endl;
        string truncated = data.substr(0, 50) + "...";
        cout << "File2: Truncated data: " << truncated << endl;
    } else {
        cout << "File2: Data length is acceptable" << endl;
    }
    
    // Convert to uppercase
    string upperData = data;
    transform(upperData.begin(), upperData.end(), upperData.begin(), ::toupper);
    cout << "File2: Uppercase version: " << upperData << endl;
    
    // Call function from file1
    validateInput(upperData);
}

void calculateMetrics(const vector<double>& values) {
    cout << "File2: Calculating metrics for " << values.size() << " values" << endl;
    
    if (values.empty()) {
        cout << "File2: No values to calculate metrics" << endl;
        return;
    }
    
    // Calculate various statistical metrics
    double sum = accumulate(values.begin(), values.end(), 0.0);
    double mean = sum / values.size();
    
    double variance = 0.0;
    for (double value : values) {
        variance += pow(value - mean, 2);
    }
    variance /= values.size();
    
    double stdDev = sqrt(variance);
    
    auto minMax = minmax_element(values.begin(), values.end());
    double min = *minMax.first;
    double max = *minMax.second;
    
    cout << "File2: Metrics calculated:" << endl;
    cout << "  Sum: " << sum << endl;
    cout << "  Mean: " << mean << endl;
    cout << "  Variance: " << variance << endl;
    cout << "  Standard Deviation: " << stdDev << endl;
    cout << "  Min: " << min << ", Max: " << max << endl;
    
    // Call function from file3
    vector<string> metricResults;
    metricResults.push_back("Sum: " + to_string(sum));
    metricResults.push_back("Mean: " + to_string(mean));
    metricResults.push_back("StdDev: " + to_string(stdDev));
    metricResults.push_back("Range: " + to_string(max - min));
    
    displayResults("File2 Metrics", metricResults);
}

void sortData(vector<int>& data) {
    cout << "File2: Sorting data with " << data.size() << " elements" << endl;
    
    if (data.empty()) {
        cout << "File2: No data to sort" << endl;
        return;
    }
    
    // Sort in ascending order
    sort(data.begin(), data.end());
    
    cout << "File2: Sorted data: ";
    for (int value : data) {
        cout << value << " ";
    }
    cout << endl;
    
    // Call function from file1
    string reportType = "Sorted Data Report";
    generateReport(reportType);
}

void filterData(const vector<string>& data, const string& filter) {
    cout << "File2: Filtering data with filter: " << filter << endl;
    
    vector<string> filteredData;
    for (const string& item : data) {
        if (item.find(filter) != string::npos) {
            filteredData.push_back(item);
        }
    }
    
    cout << "File2: Filtered " << data.size() << " items to " << filteredData.size() << " items" << endl;
    
    if (!filteredData.empty()) {
        cout << "File2: Filtered results:" << endl;
        for (const string& item : filteredData) {
            cout << "  - " << item << endl;
        }
    }
    
    // Call function from file3
    displayResults("File2 Filtered Data", filteredData);
}

void transformData(vector<double>& data, double factor) {
    cout << "File2: Transforming data with factor: " << factor << endl;
    
    if (data.empty()) {
        cout << "File2: No data to transform" << endl;
        return;
    }
    
    // Apply transformation to each element
    for (double& value : data) {
        value *= factor;
    }
    
    cout << "File2: Transformed data: ";
    for (double value : data) {
        cout << value << " ";
    }
    cout << endl;
    
    // Call function from file2 (recursive call to calculateMetrics)
    calculateMetrics(data);
}

// Main function for file2
int main() {
    cout << "=== File2 Main Function ===" << endl;
    
    // Test processData function
    cout << "\n1. Testing processData:" << endl;
    processData("Hello World");
    processData("This is a very long string that should be truncated because it exceeds the maximum length allowed");
    
    // Test calculateMetrics function
    cout << "\n2. Testing calculateMetrics:" << endl;
    vector<double> testValues = {1.5, 2.5, 3.5, 4.5, 5.5};
    calculateMetrics(testValues);
    calculateMetrics({});
    
    // Test sortData function
    cout << "\n3. Testing sortData:" << endl;
    vector<int> unsortedData = {30, 10, 50, 20, 40};
    sortData(unsortedData);
    sortData({});
    
    // Test filterData function
    cout << "\n4. Testing filterData:" << endl;
    vector<string> stringData = {"apple", "banana", "cherry", "date", "elderberry"};
    filterData(stringData, "a");
    filterData(stringData, "xyz");
    
    // Test transformData function
    cout << "\n5. Testing transformData:" << endl;
    vector<double> transformData = {1.0, 2.0, 3.0, 4.0};
    transformData(transformData, 2.5);
    transformData({}, 1.5);
    
    cout << "\n=== File2 Main Function Completed ===" << endl;
    return 0;
}