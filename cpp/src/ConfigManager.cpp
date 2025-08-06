#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <sstream>
#include <algorithm>
#include <regex>

using namespace std;

class ConfigManager {
private:
    map<string, string> config;
    string configFile;
    bool loaded;
    
    // Decision making variables
    bool enableValidation;
    bool enableDefaults;
    bool autoSave;
    
public:
    ConfigManager() : loaded(false), enableValidation(true), 
                     enableDefaults(true), autoSave(true) {}
    
    bool loadConfig(const string& filename) {
        configFile = filename;
        
        // IO call - read configuration file
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Failed to open config file: " << filename << endl;
            
            // Decision making - create default config if file doesn't exist
            if (enableDefaults) {
                createDefaultConfig();
                return saveConfig();
            }
            return false;
        }
        
        // Loop - read each line
        string line;
        while (getline(file, line)) {
            parseConfigLine(line);
        }
        
        file.close();
        loaded = true;
        
        // Decision making - validate configuration
        if (enableValidation && !validateConfig()) {
            cerr << "Configuration validation failed" << endl;
            return false;
        }
        
        cout << "Configuration loaded successfully" << endl;
        return true;
    }
    
    bool saveConfig() {
        if (configFile.empty()) return false;
        
        // IO call - write configuration file
        ofstream file(configFile);
        if (!file.is_open()) {
            cerr << "Failed to create config file: " << configFile << endl;
            return false;
        }
        
        // Loop - write each configuration item
        for (const auto& pair : config) {
            file << pair.first << "=" << pair.second << endl;
        }
        
        file.close();
        return true;
    }
    
    string getString(const string& key, const string& defaultValue = "") {
        auto it = config.find(key);
        if (it != config.end()) {
            return it->second;
        }
        
        // Decision making - return default if not found
        if (enableDefaults) {
            config[key] = defaultValue;
            if (autoSave) saveConfig();
            return defaultValue;
        }
        
        return defaultValue;
    }
    
    int getInt(const string& key, int defaultValue = 0) {
        string value = getString(key, to_string(defaultValue));
        
        // Decision making - validate integer conversion
        try {
            return stoi(value);
        } catch (const exception& e) {
            cerr << "Invalid integer value for key " << key << ": " << value << endl;
            return defaultValue;
        }
    }
    
    double getDouble(const string& key, double defaultValue = 0.0) {
        string value = getString(key, to_string(defaultValue));
        
        // Decision making - validate double conversion
        try {
            return stod(value);
        } catch (const exception& e) {
            cerr << "Invalid double value for key " << key << ": " << value << endl;
            return defaultValue;
        }
    }
    
    bool getBool(const string& key, bool defaultValue = false) {
        string value = getString(key, defaultValue ? "true" : "false");
        
        // Decision making - parse boolean value
        transform(value.begin(), value.end(), value.begin(), ::tolower);
        return value == "true" || value == "1" || value == "yes";
    }
    
    void setString(const string& key, const string& value) {
        // Decision making - validate key format
        if (isValidKey(key)) {
            config[key] = value;
            if (autoSave) saveConfig();
        } else {
            cerr << "Invalid configuration key: " << key << endl;
        }
    }
    
    void setInt(const string& key, int value) {
        setString(key, to_string(value));
    }
    
    void setDouble(const string& key, double value) {
        setString(key, to_string(value));
    }
    
    void setBool(const string& key, bool value) {
        setString(key, value ? "true" : "false");
    }
    
    bool hasKey(const string& key) {
        return config.find(key) != config.end();
    }
    
    void removeKey(const string& key) {
        auto it = config.find(key);
        if (it != config.end()) {
            config.erase(it);
            if (autoSave) saveConfig();
        }
    }
    
    vector<string> getKeys() {
        vector<string> keys;
        
        // Loop - extract all keys
        for (const auto& pair : config) {
            keys.push_back(pair.first);
        }
        
        return keys;
    }
    
    void clear() {
        config.clear();
        if (autoSave) saveConfig();
    }
    
    bool reload() {
        if (configFile.empty()) return false;
        return loadConfig(configFile);
    }
    
private:
    void parseConfigLine(const string& line) {
        // Decision making - skip comments and empty lines
        string trimmed = trim(line);
        if (trimmed.empty() || trimmed[0] == '#') {
            return;
        }
        
        // Decision making - parse key-value pair
        size_t pos = trimmed.find('=');
        if (pos != string::npos) {
            string key = trim(trimmed.substr(0, pos));
            string value = trim(trimmed.substr(pos + 1));
            
            if (!key.empty()) {
                config[key] = value;
            }
        }
    }
    
    string trim(const string& str) {
        size_t start = str.find_first_not_of(" \t\r\n");
        if (start == string::npos) return "";
        
        size_t end = str.find_last_not_of(" \t\r\n");
        return str.substr(start, end - start + 1);
    }
    
    bool isValidKey(const string& key) {
        // Decision making - validate key format
        if (key.empty()) return false;
        
        // Loop - check each character
        for (char c : key) {
            if (!isalnum(c) && c != '_' && c != '.') {
                return false;
            }
        }
        
        return true;
    }
    
    bool validateConfig() {
        // Decision making - validate required keys
        vector<string> requiredKeys = {"operation_mode", "max_threads", "batch_size"};
        
        // Loop - check each required key
        for (const auto& key : requiredKeys) {
            if (!hasKey(key)) {
                cerr << "Missing required configuration key: " << key << endl;
                return false;
            }
        }
        
        // Decision making - validate value ranges
        int maxThreads = getInt("max_threads", 4);
        if (maxThreads < 1 || maxThreads > 32) {
            cerr << "Invalid max_threads value: " << maxThreads << endl;
            return false;
        }
        
        int batchSize = getInt("batch_size", 100);
        if (batchSize < 1 || batchSize > 10000) {
            cerr << "Invalid batch_size value: " << batchSize << endl;
            return false;
        }
        
        return true;
    }
    
    void createDefaultConfig() {
        // Decision making - set default values
        config["operation_mode"] = "normal";
        config["max_threads"] = "4";
        config["batch_size"] = "100";
        config["processing_threshold"] = "0.8";
        config["enable_logging"] = "true";
        config["log_level"] = "INFO";
        config["database_path"] = "data.db";
        config["network_timeout"] = "30";
        config["retry_count"] = "3";
        config["compression_enabled"] = "true";
        
        cout << "Default configuration created" << endl;
    }
    
    map<string, string> getConfigSnapshot() {
        return config;
    }
    
    void restoreFromSnapshot(const map<string, string>& snapshot) {
        config = snapshot;
        if (autoSave) saveConfig();
    }
};

// Main function to demonstrate ConfigManager
int main() {
    cout << "=== ConfigManager Demo ===" << endl;
    
    ConfigManager configManager;
    
    // Test loading configuration
    string configFile = "test_config.txt";
    if (!configManager.loadConfig(configFile)) {
        cout << "Creating default configuration..." << endl;
    }
    
    // Test getting configuration values
    cout << "\n--- Configuration Values ---" << endl;
    cout << "Operation Mode: " << configManager.getString("operation_mode") << endl;
    cout << "Max Threads: " << configManager.getInt("max_threads") << endl;
    cout << "Batch Size: " << configManager.getInt("batch_size") << endl;
    cout << "Processing Threshold: " << configManager.getDouble("processing_threshold") << endl;
    cout << "Enable Logging: " << (configManager.getBool("enable_logging") ? "true" : "false") << endl;
    cout << "Log Level: " << configManager.getString("log_level") << endl;
    
    // Test setting new values
    cout << "\n--- Setting New Values ---" << endl;
    configManager.setString("custom_setting", "test_value");
    configManager.setInt("timeout_seconds", 60);
    configManager.setDouble("accuracy_threshold", 0.95);
    configManager.setBool("debug_mode", true);
    
    cout << "Custom Setting: " << configManager.getString("custom_setting") << endl;
    cout << "Timeout: " << configManager.getInt("timeout_seconds") << " seconds" << endl;
    cout << "Accuracy Threshold: " << configManager.getDouble("accuracy_threshold") << endl;
    cout << "Debug Mode: " << (configManager.getBool("debug_mode") ? "true" : "false") << endl;
    
    // Test key operations
    cout << "\n--- Key Operations ---" << endl;
    cout << "Has 'operation_mode' key: " << (configManager.hasKey("operation_mode") ? "true" : "false") << endl;
    cout << "Has 'non_existent_key' key: " << (configManager.hasKey("non_existent_key") ? "true" : "false") << endl;
    
    // Get all keys
    vector<string> keys = configManager.getKeys();
    cout << "All configuration keys:" << endl;
    for (const auto& key : keys) {
        cout << "  - " << key << endl;
    }
    
    // Test removing a key
    cout << "\n--- Removing Key ---" << endl;
    configManager.removeKey("custom_setting");
    cout << "Has 'custom_setting' after removal: " << (configManager.hasKey("custom_setting") ? "true" : "false") << endl;
    
    // Test saving configuration
    cout << "\n--- Saving Configuration ---" << endl;
    if (configManager.saveConfig()) {
        cout << "Configuration saved successfully" << endl;
    } else {
        cout << "Failed to save configuration" << endl;
    }
    
    // Test reloading configuration
    cout << "\n--- Reloading Configuration ---" << endl;
    if (configManager.reload()) {
        cout << "Configuration reloaded successfully" << endl;
    } else {
        cout << "Failed to reload configuration" << endl;
    }
    
    // Test clearing configuration
    cout << "\n--- Clearing Configuration ---" << endl;
    configManager.clear();
    cout << "Configuration cleared. Key count: " << configManager.getKeys().size() << endl;
    
    cout << "\nConfigManager demo completed successfully!" << endl;
    return 0;
}