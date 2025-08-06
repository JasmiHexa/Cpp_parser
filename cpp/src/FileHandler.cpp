#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>
#include <filesystem>
#include <thread>
#include <mutex>

using namespace std;

class FileHandler {
private:
    string inputPath;
    string outputPath;
    string logPath;
    mutex fileMutex;
    
    // Decision making variables
    bool enableCompression;
    bool enableEncryption;
    int maxFileSize;
    
    // Calculation variables
    long long totalBytesRead;
    long long totalBytesWritten;
    int fileCount;
    
    // IO buffer management
    vector<char> readBuffer;
    vector<char> writeBuffer;
    
public:
    FileHandler() : enableCompression(false), enableEncryption(false), 
                   maxFileSize(1024*1024*100), totalBytesRead(0), 
                   totalBytesWritten(0), fileCount(0) {
        readBuffer.resize(8192);
        writeBuffer.resize(8192);
    }
    
    bool initialize(const string& input, const string& output) {
        inputPath = input;
        outputPath = output;
        
        // Decision making - create directories if they don't exist
        if (!filesystem::exists(inputPath)) {
            filesystem::create_directories(inputPath);
        }
        if (!filesystem::exists(outputPath)) {
            filesystem::create_directories(outputPath);
        }
        
        // IO call - test file access
        string testFile = outputPath + "/test.tmp";
        ofstream testStream(testFile);
        if (!testStream.is_open()) {
            cerr << "Cannot write to output directory" << endl;
            return false;
        }
        testStream.close();
        filesystem::remove(testFile);
        
        return true;
    }
    
    vector<string> readDataBatch(int batchSize) {
        vector<string> data;
        lock_guard<mutex> lock(fileMutex);
        
        // Decision making - determine file to read
        string filename = selectNextFile();
        if (filename.empty()) {
            return data;
        }
        
        // IO call - read file
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Failed to open file: " << filename << endl;
            return data;
        }
        
        // Loop - read lines from file
        string line;
        int count = 0;
        while (getline(file, line) && count < batchSize) {
            // Decision making - validate line
            if (isValidData(line)) {
                data.push_back(line);
                count++;
            }
        }
        
        totalBytesRead += file.tellg();
        file.close();
        
        return data;
    }
    
    bool writeResults(const vector<string>& results) {
        if (results.empty()) return true;
        
        lock_guard<mutex> lock(fileMutex);
        
        // Decision making - determine output file
        string filename = generateOutputFilename();
        
        // IO call - write to file
        ofstream file(filename);
        if (!file.is_open()) {
            cerr << "Failed to create output file: " << filename << endl;
            return false;
        }
        
        // Loop - write each result
        for (const auto& result : results) {
            file << result << endl;
        }
        
        totalBytesWritten += file.tellp();
        file.close();
        
        // Decision making - compress if enabled
        if (enableCompression && shouldCompressFile(filename)) {
            compressFile(filename);
        }
        
        return true;
    }
    
    bool writeBatchResults(const vector<string>& results) {
        return writeResults(results);
    }
    
    void flush() {
        lock_guard<mutex> lock(fileMutex);
        // Force flush any buffered data
        cout << "File handler flushed" << endl;
    }
    
    bool shouldCompressFile(const string& filename) {
        // Decision making based on file size
        filesystem::path path(filename);
        if (filesystem::exists(path)) {
            auto fileSize = filesystem::file_size(path);
            return fileSize > maxFileSize / 2;
        }
        return false;
    }
    
    string selectNextFile() {
        // Decision making - select next file to process
        vector<string> files = getInputFiles();
        
        if (files.empty()) {
            return "";
        }
        
        // Decision making - prioritize files by size
        sort(files.begin(), files.end(), [this](const string& a, const string& b) {
            return getFileSize(a) < getFileSize(b);
        });
        
        return files.empty() ? "" : files[0];
    }
    
    string generateOutputFilename() {
        // Calculation - generate unique filename
        auto now = chrono::system_clock::now();
        auto timestamp = chrono::duration_cast<chrono::milliseconds>(
            now.time_since_epoch()).count();
        
        stringstream ss;
        ss << outputPath << "/result_" << timestamp << "_" << fileCount++ << ".txt";
        return ss.str();
    }
    
    bool isValidData(const string& data) {
        // Decision making - validate data format
        if (data.empty()) return false;
        
        // Calculation - check data length
        if (data.length() < 3 || data.length() > 1000) return false;
        
        // Decision making - check for valid characters
        for (char c : data) {
            if (c < 32 && c != '\t' && c != '\n' && c != '\r') {
                return false;
            }
        }
        
        return true;
    }
    
    double calculateProcessingEfficiency() {
        // Calculation - efficiency based on I/O operations
        if (totalBytesRead == 0) return 1.0;
        
        double throughput = static_cast<double>(totalBytesWritten) / totalBytesRead;
        double fileEfficiency = min(1.0, static_cast<double>(fileCount) / 100.0);
        
        return (throughput + fileEfficiency) / 2.0;
    }
    
    long long getFileSize(const string& filename) {
        filesystem::path path(filename);
        if (filesystem::exists(path)) {
            return filesystem::file_size(path);
        }
        return 0;
    }
    
    vector<string> getInputFiles() {
        vector<string> files;
        
        // Loop - scan input directory
        for (const auto& entry : filesystem::directory_iterator(inputPath)) {
            if (entry.is_regular_file()) {
                string extension = entry.path().extension().string();
                
                // Decision making - filter by file type
                if (extension == ".txt" || extension == ".csv" || extension == ".dat") {
                    files.push_back(entry.path().string());
                }
            }
        }
        
        return files;
    }
    
    void compressFile(const string& filename) {
        // Service call - compression (simplified)
        cout << "Compressing file: " << filename << endl;
        
        // Calculation - estimate compression ratio
        long long originalSize = getFileSize(filename);
        double compressionRatio = 0.7; // Assume 30% compression
        
        // IO call - create compressed file
        string compressedName = filename + ".gz";
        ofstream compressedFile(compressedName);
        if (compressedFile.is_open()) {
            compressedFile << "Compressed content" << endl;
            compressedFile.close();
            
            // Decision making - remove original if compression successful
            if (getFileSize(compressedName) < originalSize * compressionRatio) {
                filesystem::remove(filename);
            }
        }
    }
    
    bool backupFiles(const string& backupPath) {
        // IO call - create backup directory
        if (!filesystem::exists(backupPath)) {
            filesystem::create_directories(backupPath);
        }
        
        vector<string> files = getInputFiles();
        int successCount = 0;
        
        // Loop - backup each file
        for (const auto& file : files) {
            filesystem::path sourcePath(file);
            filesystem::path destPath = filesystem::path(backupPath) / sourcePath.filename();
            
            try {
                filesystem::copy_file(sourcePath, destPath);
                successCount++;
            } catch (const exception& e) {
                cerr << "Failed to backup " << file << ": " << e.what() << endl;
            }
        }
        
        // Decision making - return success if majority of files backed up
        return successCount > files.size() / 2;
    }
    
    void cleanupOldFiles(int daysOld) {
        // Calculation - determine cutoff time
        auto cutoff = chrono::system_clock::now() - chrono::hours(24 * daysOld);
        
        vector<string> files = getInputFiles();
        int deletedCount = 0;
        
        // Loop - check and delete old files
        for (const auto& file : files) {
            filesystem::path path(file);
            auto lastWrite = filesystem::last_write_time(path);
            auto lastWriteTime = chrono::clock_cast<chrono::system_clock>(lastWrite);
            
            // Decision making - delete if file is old
            if (lastWriteTime < cutoff) {
                try {
                    filesystem::remove(path);
                    deletedCount++;
                } catch (const exception& e) {
                    cerr << "Failed to delete " << file << ": " << e.what() << endl;
                }
            }
        }
        
        cout << "Cleaned up " << deletedCount << " old files" << endl;
    }
    
    double calculateDiskUsage() {
        // Calculation - disk usage percentage
        filesystem::space_info space = filesystem::space(outputPath);
        if (space.capacity > 0) {
            return static_cast<double>(space.capacity - space.available) / space.capacity;
        }
        return 0.0;
    }
    
    bool shouldEnableCompression() {
        // Decision making based on disk usage
        double diskUsage = calculateDiskUsage();
        return diskUsage > 0.8; // Enable compression if disk is 80% full
    }
    
    void updateCompressionSettings() {
        // Decision making - update compression settings
        enableCompression = shouldEnableCompression();
        if (enableCompression) {
            cout << "Compression enabled due to high disk usage" << endl;
        }
    }
};

// Main function to demonstrate FileHandler
int main() {
    cout << "=== FileHandler Demo ===" << endl;
    
    FileHandler fileHandler;
    
    // Initialize file handler
    string inputDir = "input_files";
    string outputDir = "output_files";
    
    if (!fileHandler.initialize(inputDir, outputDir)) {
        cerr << "Failed to initialize FileHandler" << endl;
        return 1;
    }
    
    // Create some test input files
    cout << "\n--- Creating Test Files ---" << endl;
    
    // Create input directory and test files
    filesystem::create_directories(inputDir);
    
    vector<string> testFiles = {
        "test1.txt",
        "test2.csv",
        "test3.dat"
    };
    
    vector<vector<string>> testData = {
        {"Line 1: Simple text data", "Line 2: More text data", "Line 3: Final line"},
        {"Name,Age,City", "John,25,New York", "Jane,30,Los Angeles", "Bob,28,Chicago"},
        {"Data1,Value1", "Data2,Value2", "Data3,Value3", "Data4,Value4", "Data5,Value5"}
    };
    
    for (size_t i = 0; i < testFiles.size(); ++i) {
        string filePath = inputDir + "/" + testFiles[i];
        ofstream file(filePath);
        if (file.is_open()) {
            for (const auto& line : testData[i]) {
                file << line << endl;
            }
            file.close();
            cout << "Created test file: " << filePath << endl;
        }
    }
    
    // Test file reading
    cout << "\n--- File Reading Tests ---" << endl;
    
    for (int batchSize = 2; batchSize <= 5; batchSize += 3) {
        cout << "\nReading with batch size " << batchSize << ":" << endl;
        
        auto data = fileHandler.readDataBatch(batchSize);
        cout << "Read " << data.size() << " lines:" << endl;
        
        for (const auto& line : data) {
            cout << "  " << line << endl;
        }
    }
    
    // Test file writing
    cout << "\n--- File Writing Tests ---" << endl;
    
    vector<string> outputData = {
        "Processed result 1",
        "Processed result 2",
        "Processed result 3",
        "Processed result 4",
        "Processed result 5"
    };
    
    if (fileHandler.writeResults(outputData)) {
        cout << "Results written successfully" << endl;
    } else {
        cout << "Failed to write results" << endl;
    }
    
    // Test batch writing
    vector<string> batchOutputData = {
        "Batch item 1",
        "Batch item 2",
        "Batch item 3"
    };
    
    if (fileHandler.writeBatchResults(batchOutputData)) {
        cout << "Batch results written successfully" << endl;
    } else {
        cout << "Failed to write batch results" << endl;
    }
    
    // Test file operations
    cout << "\n--- File Operations ---" << endl;
    
    // Get input files
    auto inputFiles = fileHandler.getInputFiles();
    cout << "Found " << inputFiles.size() << " input files:" << endl;
    for (const auto& file : inputFiles) {
        long long size = fileHandler.getFileSize(file);
        cout << "  " << file << " (size: " << size << " bytes)" << endl;
    }
    
    // Test file selection
    cout << "\n--- File Selection ---" << endl;
    string nextFile = fileHandler.selectNextFile();
    if (!nextFile.empty()) {
        cout << "Next file to process: " << nextFile << endl;
    } else {
        cout << "No files available for processing" << endl;
    }
    
    // Test data validation
    cout << "\n--- Data Validation ---" << endl;
    vector<string> validationTests = {
        "Valid data line",
        "",  // Empty
        "Too short",  // Too short
        "This is a very long line that exceeds the maximum length limit and should be considered invalid for processing purposes",
        "Data with invalid chars \x01\x02\x03",
        "Normal valid data with numbers 123"
    };
    
    for (const auto& test : validationTests) {
        bool isValid = fileHandler.isValidData(test);
        cout << "Data: \"" << test << "\" -> Valid: " << (isValid ? "Yes" : "No") << endl;
    }
    
    // Test processing efficiency
    cout << "\n--- Processing Efficiency ---" << endl;
    double efficiency = fileHandler.calculateProcessingEfficiency();
    cout << "Processing efficiency: " << efficiency << endl;
    
    // Test disk usage calculation
    cout << "\n--- Disk Usage ---" << endl;
    double diskUsage = fileHandler.calculateDiskUsage();
    cout << "Disk usage: " << (diskUsage * 100) << "%" << endl;
    
    // Test compression settings
    cout << "\n--- Compression Settings ---" << endl;
    fileHandler.updateCompressionSettings();
    
    // Test backup functionality
    cout << "\n--- Backup Test ---" << endl;
    string backupPath = "backup_files";
    if (fileHandler.backupFiles(backupPath)) {
        cout << "Files backed up successfully" << endl;
    } else {
        cout << "Backup failed" << endl;
    }
    
    // Test file compression
    cout << "\n--- File Compression ---" << endl;
    string testFile = inputDir + "/test1.txt";
    if (fileHandler.shouldCompressFile(testFile)) {
        cout << "File should be compressed: " << testFile << endl;
        fileHandler.compressFile(testFile);
    } else {
        cout << "File does not need compression: " << testFile << endl;
    }
    
    // Test cleanup operations
    cout << "\n--- Cleanup Operations ---" << endl;
    fileHandler.cleanupOldFiles(1); // Clean files older than 1 day
    
    // Test flush operation
    cout << "\n--- Flush Operation ---" << endl;
    fileHandler.flush();
    
    cout << "\nFileHandler demo completed successfully!" << endl;
    return 0;
}