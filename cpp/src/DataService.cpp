#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <chrono>
#include <thread>
#include <mutex>
#include <queue>
#include <memory>

using namespace std;

// Forward declaration
class DatabaseManager;

class DataService {
private:
    DatabaseManager* databaseManager;
    vector<string> processedResults;
    queue<string> processingQueue;
    map<string, int> dataStats;
    mutex serviceMutex;
    
    // Decision making variables
    bool enableValidation;
    bool enableTransformation;
    int maxRetries;
    
    // Calculation variables
    int totalProcessed;
    int successfulProcessed;
    double averageProcessingTime;
    
    // Service state
    bool initialized;
    string currentMode;
    
public:
    DataService() : databaseManager(nullptr), enableValidation(true), 
                   enableTransformation(true), maxRetries(3), totalProcessed(0),
                   successfulProcessed(0), averageProcessingTime(0.0), 
                   initialized(false), currentMode("normal") {}
    
    bool initialize(DatabaseManager* dbManager) {
        databaseManager = dbManager;
        initialized = true;
        
        // Decision making - set processing mode based on database state
        if (databaseManager) {
            currentMode = "database_enabled";
        } else {
            currentMode = "memory_only";
        }
        
        cout << "DataService initialized in " << currentMode << " mode" << endl;
        return true;
    }
    
    bool processItem(const string& item, const string& mode) {
        if (!initialized) return false;
        
        auto start = chrono::high_resolution_clock::now();
        
        // Decision making - validate input
        if (enableValidation && !validateData(item)) {
            cerr << "Data validation failed for item" << endl;
            return false;
        }
        
        // Service call - transform data
        string processedItem = item;
        if (enableTransformation) {
            processedItem = transformData(item, mode);
        }
        
        // Decision making - determine processing strategy
        string strategy = selectProcessingStrategy(processedItem, mode);
        
        // Loop - retry processing if needed
        bool success = false;
        for (int attempt = 0; attempt < maxRetries && !success; ++attempt) {
            success = executeProcessing(processedItem, strategy);
            
            if (!success && attempt < maxRetries - 1) {
                this_thread::sleep_for(chrono::milliseconds(100 * (attempt + 1)));
            }
        }
        
        if (success) {
            // Service call - store result
            storeResult(processedItem);
            updateStats(processedItem);
        }
        
        auto end = chrono::high_resolution_clock::now();
        double processingTime = chrono::duration<double>(end - start).count();
        updateProcessingMetrics(processingTime, success);
        
        return success;
    }
    
    vector<string> getProcessedResults() {
        lock_guard<mutex> lock(serviceMutex);
        vector<string> results = processedResults;
        processedResults.clear();
        return results;
    }
    
    string getResult(int index) {
        lock_guard<mutex> lock(serviceMutex);
        if (index >= 0 && index < static_cast<int>(processedResults.size())) {
            return processedResults[index];
        }
        return "";
    }
    
    int getResultCount() {
        lock_guard<mutex> lock(serviceMutex);
        return static_cast<int>(processedResults.size());
    }
    
    bool validateData(const string& data) {
        // Decision making - comprehensive validation
        if (data.empty()) return false;
        
        // Calculation - check data characteristics
        int alphaCount = 0, digitCount = 0, specialCount = 0;
        
        // Loop - analyze each character
        for (char c : data) {
            if (isalpha(c)) {
                alphaCount++;
            } else if (isdigit(c)) {
                digitCount++;
            } else {
                specialCount++;
            }
        }
        
        // Decision making - validate composition
        double totalChars = data.length();
        double alphaRatio = alphaCount / totalChars;
        double digitRatio = digitCount / totalChars;
        
        return alphaRatio > 0.1 && digitRatio > 0.1 && specialCount < totalChars * 0.5;
    }
    
    string transformData(const string& data, const string& mode) {
        string transformed = data;
        
        // Decision making - apply transformations based on mode
        if (mode == "fast") {
            // Minimal transformation
            transform(transformed.begin(), transformed.end(), transformed.begin(), ::toupper);
        } else if (mode == "normal") {
            // Standard transformation
            transform(transformed.begin(), transformed.end(), transformed.begin(), ::toupper);
            // Remove extra spaces
            transformed.erase(unique(transformed.begin(), transformed.end(), 
                [](char a, char b) { return a == ' ' && b == ' '; }), transformed.end());
        } else if (mode == "thorough") {
            // Comprehensive transformation
            transform(transformed.begin(), transformed.end(), transformed.begin(), ::toupper);
            // Remove special characters
            transformed.erase(remove_if(transformed.begin(), transformed.end(), 
                [](char c) { return !isalnum(c) && c != ' '; }), transformed.end());
            // Normalize spaces
            string::iterator new_end = unique(transformed.begin(), transformed.end(),
                [](char a, char b) { return a == ' ' && b == ' '; });
            transformed.erase(new_end, transformed.end());
        }
        
        return transformed;
    }
    
    string selectProcessingStrategy(const string& data, const string& mode) {
        // Decision making based on data characteristics and mode
        int complexity = calculateDataComplexity(data);
        
        if (mode == "fast") {
            return "minimal";
        } else if (complexity < 10) {
            return "standard";
        } else {
            return "advanced";
        }
    }
    
    bool executeProcessing(const string& data, const string& strategy) {
        // Service call - process data according to strategy
        if (strategy == "minimal") {
            return processMinimal(data);
        } else if (strategy == "standard") {
            return processStandard(data);
        } else if (strategy == "advanced") {
            return processAdvanced(data);
        }
        
        return false;
    }
    
    bool processMinimal(const string& data) {
        // Simple processing - just validate and store
        if (data.length() > 0) {
            return true;
        }
        return false;
    }
    
    bool processStandard(const string& data) {
        // Standard processing with basic analysis
        // Calculation - perform basic analysis
        int wordCount = count(data.begin(), data.end(), ' ') + 1;
        double avgWordLength = static_cast<double>(data.length()) / wordCount;
        
        // Decision making - accept if reasonable
        return avgWordLength > 2.0 && avgWordLength < 20.0;
    }
    
    bool processAdvanced(const string& data) {
        // Advanced processing with complex analysis
        // Calculation - perform advanced analysis
        vector<int> wordLengths;
        stringstream ss(data);
        string word;
        
        // Loop - analyze each word
        while (ss >> word) {
            wordLengths.push_back(word.length());
        }
        
        if (wordLengths.empty()) return false;
        
        // Calculation - statistical analysis
        double mean = accumulate(wordLengths.begin(), wordLengths.end(), 0.0) / wordLengths.size();
        
        double variance = 0.0;
        for (int length : wordLengths) {
            variance += pow(length - mean, 2);
        }
        variance /= wordLengths.size();
        
        double stdDev = sqrt(variance);
        
        // Decision making - accept based on statistical criteria
        return mean > 3.0 && stdDev < 5.0;
    }
    
    void storeResult(const string& result) {
        lock_guard<mutex> lock(serviceMutex);
        processedResults.push_back(result);
        
        // Service call - store in database if available
        if (databaseManager) {
            vector<string> params = {result, "processed"};
            // databaseManager->executePreparedQuery("insert_data", params);
        }
    }
    
    void updateStats(const string& data) {
        // Calculation - update statistics
        dataStats["total_length"] += data.length();
        dataStats["word_count"] += count(data.begin(), data.end(), ' ') + 1;
        dataStats["processed_count"]++;
    }
    
    void updateProcessingMetrics(double processingTime, bool success) {
        totalProcessed++;
        if (success) successfulProcessed++;
        
        // Calculation - update average processing time
        averageProcessingTime = (averageProcessingTime * (totalProcessed - 1) + processingTime) / totalProcessed;
    }
    
    int calculateDataComplexity(const string& data) {
        // Calculation - complexity score based on multiple factors
        int complexity = 0;
        
        // Loop - analyze data characteristics
        for (char c : data) {
            if (isupper(c)) complexity += 1;
            if (isdigit(c)) complexity += 2;
            if (!isalnum(c) && c != ' ') complexity += 3;
        }
        
        // Decision making - adjust complexity based on length
        if (data.length() > 100) complexity += 10;
        if (data.length() > 500) complexity += 20;
        
        return complexity;
    }
    
    double calculateProcessingEfficiency() {
        // Calculation - efficiency based on success rate and performance
        if (totalProcessed == 0) return 1.0;
        
        double successRate = static_cast<double>(successfulProcessed) / totalProcessed;
        double timeEfficiency = max(0.1, 1.0 - averageProcessingTime);
        
        return (successRate + timeEfficiency) / 2.0;
    }
    
    void cleanup() {
        lock_guard<mutex> lock(serviceMutex);
        processedResults.clear();
        processingQueue = queue<string>();
        dataStats.clear();
    }
    
    map<string, double> getPerformanceMetrics() {
        map<string, double> metrics;
        metrics["efficiency"] = calculateProcessingEfficiency();
        metrics["average_time"] = averageProcessingTime;
        metrics["success_rate"] = totalProcessed > 0 ? static_cast<double>(successfulProcessed) / totalProcessed : 0.0;
        metrics["total_processed"] = static_cast<double>(totalProcessed);
        
        return metrics;
    }
    
    bool shouldEnableValidation() {
        // Decision making based on error rate
        double errorRate = 1.0 - calculateProcessingEfficiency();
        return errorRate > 0.1; // Enable validation if error rate > 10%
    }
    
    void updateValidationSettings() {
        // Decision making - update validation settings
        enableValidation = shouldEnableValidation();
        if (enableValidation) {
            cout << "Data validation enabled due to high error rate" << endl;
        }
    }
};

// Main function to demonstrate DataService
int main() {
    cout << "=== DataService Demo ===" << endl;
    
    DataService dataService;
    
    // Initialize the service (without database for demo)
    if (!dataService.initialize(nullptr)) {
        cerr << "Failed to initialize DataService" << endl;
        return 1;
    }
    
    // Test data processing with different modes
    cout << "\n--- Data Processing Tests ---" << endl;
    
    vector<string> testData = {
        "Simple text data for processing",
        "Complex data with numbers 123 and symbols @#$%",
        "Very long text that contains many words and should be processed thoroughly",
        "MixedCase Text With 456 Numbers And Special Characters!",
        "Short text",
        "Invalid data with too many special characters @#$%^&*()_+{}|:<>?[]\\;'\",./<>?"
    };
    
    vector<string> modes = {"fast", "normal", "thorough"};
    
    for (const auto& mode : modes) {
        cout << "\n--- Processing in " << mode << " mode ---" << endl;
        
        for (const auto& data : testData) {
            cout << "Processing: \"" << data << "\"" << endl;
            
            bool success = dataService.processItem(data, mode);
            cout << "Result: " << (success ? "SUCCESS" : "FAILED") << endl;
            
            if (success) {
                string result = dataService.getResult(dataService.getResultCount() - 1);
                cout << "Processed result: \"" << result << "\"" << endl;
            }
            cout << "---" << endl;
        }
    }
    
    // Test data validation
    cout << "\n--- Data Validation Tests ---" << endl;
    vector<string> validationTests = {
        "Valid data with text and numbers 123",
        "",  // Empty string
        "Too short",  // Too short
        "This is a very long string that exceeds the maximum length limit and should be considered invalid for processing purposes",
        "Data with invalid chars \x01\x02\x03",
        "Normal valid data"
    };
    
    for (const auto& test : validationTests) {
        bool isValid = dataService.validateData(test);
        cout << "Data: \"" << test << "\" -> Valid: " << (isValid ? "Yes" : "No") << endl;
    }
    
    // Test data transformation
    cout << "\n--- Data Transformation Tests ---" << endl;
    string originalData = "  Mixed   Case   Text   With   123   Numbers   And   @#$   Symbols   ";
    
    for (const auto& mode : modes) {
        string transformed = dataService.transformData(originalData, mode);
        cout << mode << " mode: \"" << transformed << "\"" << endl;
    }
    
    // Test processing strategy selection
    cout << "\n--- Processing Strategy Selection ---" << endl;
    vector<string> strategyTests = {
        "Simple data",
        "Complex data with many special characters @#$%^&*()",
        "Very long data that contains many words and should trigger advanced processing strategy"
    };
    
    for (const auto& test : strategyTests) {
        string strategy = dataService.selectProcessingStrategy(test, "normal");
        int complexity = dataService.calculateDataComplexity(test);
        cout << "Data: \"" << test << "\"" << endl;
        cout << "Complexity: " << complexity << endl;
        cout << "Strategy: " << strategy << endl;
        cout << "---" << endl;
    }
    
    // Test processing efficiency
    cout << "\n--- Processing Efficiency ---" << endl;
    double efficiency = dataService.calculateProcessingEfficiency();
    cout << "Processing efficiency: " << efficiency << endl;
    
    // Test validation settings update
    cout << "\n--- Validation Settings Update ---" << endl;
    dataService.updateValidationSettings();
    
    // Get performance metrics
    cout << "\n--- Performance Metrics ---" << endl;
    auto metrics = dataService.getPerformanceMetrics();
    for (const auto& metric : metrics) {
        cout << metric.first << ": " << metric.second << endl;
    }
    
    // Test cleanup
    cout << "\n--- Cleanup Test ---" << endl;
    cout << "Results before cleanup: " << dataService.getResultCount() << endl;
    dataService.cleanup();
    cout << "Results after cleanup: " << dataService.getResultCount() << endl;
    
    // Test batch processing
    cout << "\n--- Batch Processing Test ---" << endl;
    vector<string> batchData = {
        "Batch item 1",
        "Batch item 2",
        "Batch item 3",
        "Batch item 4",
        "Batch item 5"
    };
    
    int successCount = 0;
    for (const auto& item : batchData) {
        if (dataService.processItem(item, "normal")) {
            successCount++;
        }
    }
    
    cout << "Batch processing completed: " << successCount << "/" << batchData.size() << " successful" << endl;
    
    // Get final results
    auto finalResults = dataService.getProcessedResults();
    cout << "Final processed results count: " << finalResults.size() << endl;
    
    cout << "\nDataService demo completed successfully!" << endl;
    return 0;
}