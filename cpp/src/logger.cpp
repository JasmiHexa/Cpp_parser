#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>
#include <mutex>
#include <queue>
#include <sstream>
#include <iomanip>
#include <vector> // Added for std::vector
#include <map>    // Added for std::map
#include <algorithm> // Added for std::find

using namespace std;

class Logger {
private:
    ofstream logFile;
    string logPath;
    mutex logMutex;
    queue<string> logQueue;
    
    // Decision making variables
    bool enableFileLogging;
    bool enableConsoleLogging;
    string logLevel;
    int maxQueueSize;
    
    // Calculation variables
    int totalLogs;
    int errorCount;
    int warningCount;
    
public:
    Logger(const string& filename) : logPath(filename), enableFileLogging(true),
                                   enableConsoleLogging(true), logLevel("INFO"),
                                   maxQueueSize(1000), totalLogs(0), errorCount(0),
                                   warningCount(0) {
        initializeLogger();
    }
    
    ~Logger() {
        flushQueue();
        if (logFile.is_open()) {
            logFile.close();
        }
    }
    
    void log(const string& message, const string& level = "INFO") {
        // Decision making - check log level
        if (!shouldLog(level)) {
            return;
        }
        
        string formattedMessage = formatMessage(message, level);
        
        // Decision making - add to queue or write directly
        if (logQueue.size() >= maxQueueSize) {
            flushQueue();
        }
        
        {
            lock_guard<mutex> lock(logMutex);
            logQueue.push(formattedMessage);
        }
        
        updateStats(level);
        
        // Decision making - immediate flush for critical messages
        if (level == "CRITICAL" || level == "ERROR") {
            flushQueue();
        }
    }
    
    void logInfo(const string& message) {
        log(message, "INFO");
    }
    
    void logWarning(const string& message) {
        log(message, "WARNING");
    }
    
    void logError(const string& message) {
        log(message, "ERROR");
    }
    
    void logDebug(const string& message) {
        log(message, "DEBUG");
    }
    
    void logCritical(const string& message) {
        log(message, "CRITICAL");
    }
    
    void flush() {
        flushQueue();
    }
    
    void setLogLevel(const string& level) {
        // Decision making - validate log level
        vector<string> validLevels = {"DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL"};
        auto it = find(validLevels.begin(), validLevels.end(), level);
        
        if (it != validLevels.end()) {
            logLevel = level;
            log("Log level changed to " + level, "INFO");
        } else {
            log("Invalid log level: " + level, "WARNING");
        }
    }
    
    void enableFileLogging(bool enable) {
        enableFileLogging = enable;
        if (!enable && logFile.is_open()) {
            logFile.close();
        } else if (enable && !logFile.is_open()) {
            openLogFile();
        }
    }
    
    void enableConsoleLogging(bool enable) {
        enableConsoleLogging = enable;
    }
    
    map<string, int> getLogStats() {
        map<string, int> stats;
        stats["total_logs"] = totalLogs;
        stats["errors"] = errorCount;
        stats["warnings"] = warningCount;
        stats["queue_size"] = logQueue.size();
        
        return stats;
    }
    
    void rotateLogFile() {
        // Decision making - rotate log file if it's too large
        if (logFile.is_open()) {
            streampos fileSize = logFile.tellp();
            if (fileSize > 10 * 1024 * 1024) { // 10MB
                logFile.close();
                
                // IO call - rename current log file
                string backupName = logPath + ".backup";
                rename(logPath.c_str(), backupName.c_str());
                
                openLogFile();
                log("Log file rotated", "INFO");
            }
        }
    }
    
private:
    void initializeLogger() {
        // Decision making - open log file
        if (enableFileLogging) {
            openLogFile();
        }
        
        // Log initialization
        log("Logger initialized", "INFO");
    }
    
    void openLogFile() {
        logFile.open(logPath, ios::app);
        if (!logFile.is_open()) {
            cerr << "Failed to open log file: " << logPath << endl;
            enableFileLogging = false;
        }
    }
    
    bool shouldLog(const string& level) {
        // Decision making - check if level should be logged
        map<string, int> levelPriority = {
            {"DEBUG", 0},
            {"INFO", 1},
            {"WARNING", 2},
            {"ERROR", 3},
            {"CRITICAL", 4}
        };
        
        auto currentPriority = levelPriority.find(logLevel);
        auto messagePriority = levelPriority.find(level);
        
        if (currentPriority != levelPriority.end() && messagePriority != levelPriority.end()) {
            return messagePriority->second >= currentPriority->second;
        }
        
        return true;
    }
    
    string formatMessage(const string& message, const string& level) {
        // Calculation - format timestamp
        auto now = chrono::system_clock::now();
        auto time_t = chrono::system_clock::to_time_t(now);
        auto ms = chrono::duration_cast<chrono::milliseconds>(
            now.time_since_epoch()) % 1000;
        
        stringstream ss;
        ss << put_time(localtime(&time_t), "%Y-%m-%d %H:%M:%S");
        ss << "." << setfill('0') << setw(3) << ms.count();
        
        // Calculation - thread ID
        thread::id threadId = this_thread::get_id();
        
        // Decision making - format based on level
        string levelStr = "[" + level + "]";
        string timestamp = "[" + ss.str() + "]";
        string threadStr = "[Thread-" + to_string(hash<thread::id>{}(threadId)) + "]";
        
        return timestamp + " " + levelStr + " " + threadStr + " " + message;
    }
    
    void flushQueue() {
        lock_guard<mutex> lock(logMutex);
        
        // Loop - process all queued messages
        while (!logQueue.empty()) {
            string message = logQueue.front();
            logQueue.pop();
            
            // IO call - write to file
            if (enableFileLogging && logFile.is_open()) {
                logFile << message << endl;
                logFile.flush();
            }
            
            // IO call - write to console
            if (enableConsoleLogging) {
                cout << message << endl;
            }
        }
    }
    
    void updateStats(const string& level) {
        totalLogs++;
        
        // Decision making - update specific counters
        if (level == "ERROR" || level == "CRITICAL") {
            errorCount++;
        } else if (level == "WARNING") {
            warningCount++;
        }
    }
    
    void cleanupOldLogs(int daysOld) {
        // Decision making - cleanup old log files
        if (logFile.is_open()) {
            logFile.close();
        }
        
        // IO call - remove old backup files
        string backupName = logPath + ".backup";
        remove(backupName.c_str());
        
        openLogFile();
        log("Old log files cleaned up", "INFO");
    }
    
    double calculateErrorRate() {
        if (totalLogs == 0) return 0.0;
        return static_cast<double>(errorCount) / totalLogs;
    }
    
    bool shouldRotateLog() {
        // Decision making - check if log rotation is needed
        if (logFile.is_open()) {
            streampos fileSize = logFile.tellp();
            return fileSize > 5 * 1024 * 1024; // 5MB
        }
        return false;
    }
    
    void performLogRotation() {
        if (shouldRotateLog()) {
            rotateLogFile();
        }
    }
};

// Main function to demonstrate Logger
int main() {
    cout << "=== Logger Demo ===" << endl;
    
    Logger logger("demo_log.txt");
    
    // Test basic logging functionality
    cout << "\n--- Basic Logging ---" << endl;
    logger.logInfo("Application started");
    logger.logDebug("Debug information: Processing user data");
    logger.logWarning("Warning: High memory usage detected");
    logger.logError("Error: Failed to connect to database");
    logger.logCritical("Critical: System shutdown required");
    
    // Test different log levels
    cout << "\n--- Log Level Testing ---" << endl;
    
    vector<string> levels = {"DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL"};
    
    for (const auto& level : levels) {
        logger.setLogLevel(level);
        cout << "Log level set to: " << level << endl;
        
        logger.logDebug("This is a debug message");
        logger.logInfo("This is an info message");
        logger.logWarning("This is a warning message");
        logger.logError("This is an error message");
        logger.logCritical("This is a critical message");
        
        cout << "---" << endl;
    }
    
    // Reset to INFO level
    logger.setLogLevel("INFO");
    
    // Test logging with different content
    cout << "\n--- Content Testing ---" << endl;
    logger.logInfo("Processing batch of 1000 records");
    logger.logInfo("User authentication successful for user: john.doe");
    logger.logWarning("Disk space is running low: 85% used");
    logger.logError("Network timeout after 30 seconds");
    logger.logInfo("Data backup completed successfully");
    
    // Test logging settings
    cout << "\n--- Logging Settings ---" << endl;
    
    // Test file logging toggle
    logger.enableFileLogging(false);
    logger.logInfo("This message should only appear in console");
    logger.enableFileLogging(true);
    logger.logInfo("This message should appear in both console and file");
    
    // Test console logging toggle
    logger.enableConsoleLogging(false);
    logger.logInfo("This message should only appear in file");
    logger.enableConsoleLogging(true);
    logger.logInfo("This message should appear in both console and file");
    
    // Test log rotation
    cout << "\n--- Log Rotation ---" << endl;
    logger.rotateLogFile();
    
    // Test performance logging
    cout << "\n--- Performance Logging ---" << endl;
    for (int i = 1; i <= 10; ++i) {
        logger.logInfo("Processing item " + to_string(i) + " of 10");
        this_thread::sleep_for(chrono::milliseconds(100));
    }
    
    // Test error scenarios
    cout << "\n--- Error Scenarios ---" << endl;
    logger.logError("Database connection failed: Connection refused");
    logger.logError("File not found: /path/to/missing/file.txt");
    logger.logError("Invalid input format: Expected JSON, got XML");
    logger.logWarning("Retrying operation after 5 second delay");
    logger.logInfo("Operation completed after 3 retries");
    
    // Test critical events
    cout << "\n--- Critical Events ---" << endl;
    logger.logCritical("System resources critically low");
    logger.logCritical("Security breach detected");
    logger.logCritical("Emergency shutdown initiated");
    
    // Test flush operation
    cout << "\n--- Flush Operation ---" << endl;
    logger.flush();
    cout << "Log buffer flushed to file" << endl;
    
    // Get logging statistics
    cout << "\n--- Logging Statistics ---" << endl;
    auto stats = logger.getLogStats();
    for (const auto& stat : stats) {
        cout << stat.first << ": " << stat.second << endl;
    }
    
    // Test log rotation decision
    cout << "\n--- Log Rotation Analysis ---" << endl;
    logger.performLogRotation();
    
    // Test cleanup operations
    cout << "\n--- Cleanup Operations ---" << endl;
    // Note: In a real application, you might want to clean up old logs
    // logger.cleanupOldLogs(7); // Clean logs older than 7 days
    
    // Test high-volume logging
    cout << "\n--- High Volume Logging ---" << endl;
    for (int i = 1; i <= 50; ++i) {
        logger.logInfo("High volume log entry " + to_string(i));
    }
    logger.flush();
    
    // Test different message types
    cout << "\n--- Message Types ---" << endl;
    logger.logInfo("User login: admin@example.com");
    logger.logWarning("Performance degradation detected in module X");
    logger.logError("API endpoint /api/users returned 500 error");
    logger.logDebug("Variable 'user_id' = 12345");
    logger.logCritical("System crash detected in thread pool");
    
    // Test concurrent logging (simulated)
    cout << "\n--- Concurrent Logging Simulation ---" << endl;
    vector<thread> threads;
    
    for (int i = 0; i < 5; ++i) {
        threads.emplace_back([&logger, i]() {
            for (int j = 1; j <= 5; ++j) {
                logger.logInfo("Thread " + to_string(i) + " - Message " + to_string(j));
            }
        });
    }
    
    for (auto& thread : threads) {
        thread.join();
    }
    
    logger.flush();
    
    cout << "\nLogger demo completed successfully!" << endl;
    cout << "Check 'demo_log.txt' for the complete log output." << endl;
    
    return 0;
}