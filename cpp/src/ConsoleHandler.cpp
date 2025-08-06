#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <mutex>
#include <queue>
#include <sstream>
#include <iomanip>
#include <algorithm>

using namespace std;

class ConsoleHandler {
private:
    mutex consoleMutex;
    queue<string> outputQueue;
    queue<string> inputQueue;
    
    // Decision making variables
    bool enableColor;
    bool enableFormatting;
    bool enableInputBuffering;
    int maxQueueSize;
    
    // Calculation variables
    int totalOutputs;
    int totalInputs;
    double averageResponseTime;
    
    // Console state
    bool interactive;
    string prompt;
    
public:
    ConsoleHandler() : enableColor(true), enableFormatting(true), 
                      enableInputBuffering(true), maxQueueSize(100),
                      totalOutputs(0), totalInputs(0), averageResponseTime(0.0),
                      interactive(true), prompt("> ") {}
    
    void write(const string& message) {
        auto start = chrono::high_resolution_clock::now();
        
        // Decision making - format message if enabled
        string formattedMessage = message;
        if (enableFormatting) {
            formattedMessage = formatOutput(message);
        }
        
        // Decision making - add to queue or write directly
        if (outputQueue.size() >= maxQueueSize) {
            flushOutput();
        }
        
        {
            lock_guard<mutex> lock(consoleMutex);
            outputQueue.push(formattedMessage);
        }
        
        totalOutputs++;
        
        auto end = chrono::high_resolution_clock::now();
        double responseTime = chrono::duration<double>(end - start).count();
        updateResponseTime(responseTime);
    }
    
    void writeLine(const string& message) {
        write(message + "\n");
    }
    
    void writeError(const string& message) {
        // Decision making - format error message
        string errorMessage = "[ERROR] " + message;
        if (enableColor) {
            errorMessage = "\033[31m" + errorMessage + "\033[0m"; // Red color
        }
        writeLine(errorMessage);
    }
    
    void writeWarning(const string& message) {
        // Decision making - format warning message
        string warningMessage = "[WARNING] " + message;
        if (enableColor) {
            warningMessage = "\033[33m" + warningMessage + "\033[0m"; // Yellow color
        }
        writeLine(warningMessage);
    }
    
    void writeSuccess(const string& message) {
        // Decision making - format success message
        string successMessage = "[SUCCESS] " + message;
        if (enableColor) {
            successMessage = "\033[32m" + successMessage + "\033[0m"; // Green color
        }
        writeLine(successMessage);
    }
    
    string readLine() {
        auto start = chrono::high_resolution_clock::now();
        
        // Decision making - check input buffer first
        if (enableInputBuffering && !inputQueue.empty()) {
            lock_guard<mutex> lock(consoleMutex);
            string input = inputQueue.front();
            inputQueue.pop();
            totalInputs++;
            return input;
        }
        
        // IO call - read from console
        string input;
        if (interactive) {
            cout << prompt;
        }
        getline(cin, input);
        
        totalInputs++;
        
        auto end = chrono::high_resolution_clock::now();
        double responseTime = chrono::duration<double>(end - start).count();
        updateResponseTime(responseTime);
        
        return input;
    }
    
    string readLineWithPrompt(const string& customPrompt) {
        // Decision making - use custom prompt
        string originalPrompt = prompt;
        prompt = customPrompt;
        
        string result = readLine();
        
        prompt = originalPrompt;
        return result;
    }
    
    void flush() {
        flushOutput();
    }
    
    void setPrompt(const string& newPrompt) {
        prompt = newPrompt;
    }
    
    void enableInteractive(bool enable) {
        interactive = enable;
    }
    
    void enableColorOutput(bool enable) {
        enableColor = enable;
    }
    
    void enableFormatting(bool enable) {
        enableFormatting = enable;
    }
    
    void setMaxQueueSize(int size) {
        // Decision making - validate queue size
        if (size > 0 && size <= 10000) {
            maxQueueSize = size;
        } else {
            writeWarning("Invalid queue size: " + to_string(size));
        }
    }
    
    map<string, double> getConsoleStats() {
        map<string, double> stats;
        stats["total_outputs"] = static_cast<double>(totalOutputs);
        stats["total_inputs"] = static_cast<double>(totalInputs);
        stats["average_response_time"] = averageResponseTime;
        stats["output_queue_size"] = static_cast<double>(outputQueue.size());
        stats["input_queue_size"] = static_cast<double>(inputQueue.size());
        
        return stats;
    }
    
    void clearScreen() {
        // IO call - clear console screen
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
    }
    
    void displayProgressBar(int current, int total, int width = 50) {
        // Calculation - progress percentage
        double percentage = static_cast<double>(current) / total;
        int filledWidth = static_cast<int>(width * percentage);
        
        // Decision making - format progress bar
        string bar = "[";
        for (int i = 0; i < width; ++i) {
            if (i < filledWidth) {
                bar += "=";
            } else if (i == filledWidth) {
                bar += ">";
            } else {
                bar += " ";
            }
        }
        bar += "]";
        
        // Calculation - percentage display
        int percent = static_cast<int>(percentage * 100);
        
        stringstream ss;
        ss << bar << " " << percent << "% (" << current << "/" << total << ")";
        
        // Decision making - overwrite line for progress
        write("\r" + ss.str());
        if (current >= total) {
            writeLine(""); // New line when complete
        }
    }
    
    void displayTable(const vector<vector<string>>& data, 
                     const vector<string>& headers) {
        if (data.empty() || headers.empty()) return;
        
        // Calculation - determine column widths
        vector<int> columnWidths(headers.size(), 0);
        
        // Loop - calculate header widths
        for (size_t i = 0; i < headers.size(); ++i) {
            columnWidths[i] = max(columnWidths[i], static_cast<int>(headers[i].length()));
        }
        
        // Loop - calculate data widths
        for (const auto& row : data) {
            for (size_t i = 0; i < row.size() && i < headers.size(); ++i) {
                columnWidths[i] = max(columnWidths[i], static_cast<int>(row[i].length()));
            }
        }
        
        // Decision making - print separator line
        string separator = "+";
        for (int width : columnWidths) {
            separator += string(width + 2, '-') + "+";
        }
        
        writeLine(separator);
        
        // Loop - print headers
        string headerLine = "|";
        for (size_t i = 0; i < headers.size(); ++i) {
            headerLine += " " + padRight(headers[i], columnWidths[i]) + " |";
        }
        writeLine(headerLine);
        writeLine(separator);
        
        // Loop - print data rows
        for (const auto& row : data) {
            string dataLine = "|";
            for (size_t i = 0; i < row.size() && i < headers.size(); ++i) {
                dataLine += " " + padRight(row[i], columnWidths[i]) + " |";
            }
            writeLine(dataLine);
        }
        writeLine(separator);
    }
    
    void displayMenu(const vector<string>& options) {
        writeLine("Available options:");
        
        // Loop - display numbered options
        for (size_t i = 0; i < options.size(); ++i) {
            string option = to_string(i + 1) + ". " + options[i];
            writeLine(option);
        }
        
        writeLine("Enter your choice (1-" + to_string(options.size()) + "):");
    }
    
    int getMenuChoice(const vector<string>& options) {
        displayMenu(options);
        
        // Loop - get valid choice
        while (true) {
            string input = readLine();
            
            // Decision making - validate input
            try {
                int choice = stoi(input);
                if (choice >= 1 && choice <= static_cast<int>(options.size())) {
                    return choice;
                } else {
                    writeError("Invalid choice. Please enter a number between 1 and " + 
                              to_string(options.size()));
                }
            } catch (const exception& e) {
                writeError("Invalid input. Please enter a number.");
            }
        }
    }
    
private:
    string formatOutput(const string& message) {
        // Decision making - apply formatting
        string formatted = message;
        
        if (enableColor) {
            // Loop - apply color formatting
            vector<pair<string, string>> colorMap = {
                {"ERROR", "\033[31m"},
                {"WARNING", "\033[33m"},
                {"SUCCESS", "\033[32m"},
                {"INFO", "\033[36m"}
            };
            
            for (const auto& color : colorMap) {
                size_t pos = formatted.find(color.first);
                if (pos != string::npos) {
                    formatted.insert(pos, color.second);
                    formatted.insert(pos + color.second.length() + color.first.length(), "\033[0m");
                }
            }
        }
        
        return formatted;
    }
    
    void flushOutput() {
        lock_guard<mutex> lock(consoleMutex);
        
        // Loop - process all queued outputs
        while (!outputQueue.empty()) {
            string message = outputQueue.front();
            outputQueue.pop();
            
            // IO call - write to console
            cout << message;
            cout.flush();
        }
    }
    
    void updateResponseTime(double responseTime) {
        // Calculation - update average response time
        averageResponseTime = (averageResponseTime * (totalOutputs + totalInputs - 1) + responseTime) / 
                             (totalOutputs + totalInputs);
    }
    
    string padRight(const string& str, int width) {
        if (str.length() >= static_cast<size_t>(width)) {
            return str.substr(0, width);
        }
        
        return str + string(width - str.length(), ' ');
    }
    
    void bufferInput(const string& input) {
        // Decision making - add to input buffer
        if (enableInputBuffering && inputQueue.size() < maxQueueSize) {
            lock_guard<mutex> lock(consoleMutex);
            inputQueue.push(input);
        }
    }
    
    double calculateThroughput() {
        // Calculation - throughput based on total operations
        if (averageResponseTime > 0) {
            return (totalOutputs + totalInputs) / averageResponseTime;
        }
        return 0.0;
    }
    
    bool shouldFlush() {
        // Decision making - flush if queue is getting full
        return outputQueue.size() > maxQueueSize * 0.8;
    }
    
    void autoFlush() {
        if (shouldFlush()) {
            flushOutput();
        }
    }
};

// Main function to demonstrate ConsoleHandler
int main() {
    cout << "=== ConsoleHandler Demo ===" << endl;
    
    ConsoleHandler console;
    
    // Test basic output functionality
    cout << "\n--- Basic Output ---" << endl;
    console.write("This is a normal message");
    console.writeLine("This is a message with newline");
    console.writeError("This is an error message");
    console.writeWarning("This is a warning message");
    console.writeSuccess("This is a success message");
    
    // Test progress bar
    cout << "\n--- Progress Bar Demo ---" << endl;
    for (int i = 0; i <= 10; ++i) {
        console.displayProgressBar(i, 10, 30);
        this_thread::sleep_for(chrono::milliseconds(200));
    }
    
    // Test table display
    cout << "\n--- Table Display ---" << endl;
    vector<string> headers = {"Name", "Age", "City", "Score"};
    vector<vector<string>> data = {
        {"John Doe", "25", "New York", "85.5"},
        {"Jane Smith", "30", "Los Angeles", "92.3"},
        {"Bob Johnson", "28", "Chicago", "78.9"},
        {"Alice Brown", "35", "Houston", "88.1"}
    };
    console.displayTable(data, headers);
    
    // Test menu system
    cout << "\n--- Menu System ---" << endl;
    vector<string> menuOptions = {
        "Option 1: Process Data",
        "Option 2: View Statistics",
        "Option 3: Export Results",
        "Option 4: Exit"
    };
    
    console.displayMenu(menuOptions);
    cout << "Note: In a real application, this would wait for user input" << endl;
    
    // Test console settings
    cout << "\n--- Console Settings ---" << endl;
    console.setPrompt("DEMO> ");
    console.enableColorOutput(false);
    console.writeLine("Color output disabled - this should be plain text");
    console.enableColorOutput(true);
    console.writeLine("Color output enabled - this should have colors");
    
    // Test queue management
    cout << "\n--- Queue Management ---" << endl;
    console.setMaxQueueSize(5);
    for (int i = 1; i <= 10; ++i) {
        console.write("Message " + to_string(i) + " ");
    }
    console.flush();
    
    // Get console statistics
    cout << "\n--- Console Statistics ---" << endl;
    auto stats = console.getConsoleStats();
    for (const auto& stat : stats) {
        cout << stat.first << ": " << stat.second << endl;
    }
    
    // Test screen clearing
    cout << "\n--- Screen Clearing ---" << endl;
    console.writeLine("About to clear screen...");
    this_thread::sleep_for(chrono::milliseconds(1000));
    console.clearScreen();
    console.writeLine("Screen cleared!");
    
    // Test custom prompt
    cout << "\n--- Custom Prompt ---" << endl;
    string result = console.readLineWithPrompt("Enter your name: ");
    console.writeLine("Hello, " + result + "!");
    
    cout << "\nConsoleHandler demo completed successfully!" << endl;
    return 0;
}