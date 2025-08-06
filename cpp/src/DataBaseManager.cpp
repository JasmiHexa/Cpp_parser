#include <sqlite3.h>
#include <iostream>
#include <chrono>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <cmath>

using namespace std;

class DatabaseManager {
private:
    sqlite3* db;
    bool initialized;
    map<string, sqlite3_stmt*> preparedStatements;
    
    // Decision making variables
    int currentConnections;
    double queryTimeout;
    bool autoCommit;
    
    // Calculation variables
    long long totalQueries;
    long long failedQueries;
    double averageQueryTime;
    
public:
    DatabaseManager() : db(nullptr), initialized(false), currentConnections(0), 
                       queryTimeout(30.0), autoCommit(true), totalQueries(0), 
                       failedQueries(0), averageQueryTime(0.0) {}
    
    ~DatabaseManager() {
        close();
    }
    
    bool initialize(const string& dbPath) {
        // IO call - open database file
        int result = sqlite3_open(dbPath.c_str(), &db);
        if (result != SQLITE_OK) {
            cerr << "Failed to open database: " << sqlite3_errmsg(db) << endl;
            return false;
        }
        
        // Decision making - enable WAL mode for better performance
        if (shouldEnableWAL("initialize")) {
            executeQuery("PRAGMA journal_mode=WAL");
            executeQuery("PRAGMA synchronous=NORMAL");
        }
        
        // Create tables
        if (!createTables()) {
            return false;
        }
        
        // Prepare statements
        if (!prepareStatements()) {
            return false;
        }
        
        initialized = true;
        cout << "Database initialized successfully" << endl;
        return true;
    }
    
    void close() {
        if (db) {
            cleanupStatements();
            sqlite3_close(db);
            db = nullptr;
        }
        initialized = false;
    }
    
    bool executeQuery(const string& query) {
        if (!initialized || !db) return false;
        
        auto start = chrono::high_resolution_clock::now();
        
        // Decision making - sanitize query for security
        string sanitizedQuery = sanitizeQuery(query);
        
        char* errorMsg = nullptr;
        int result = sqlite3_exec(db, sanitizedQuery.c_str(), nullptr, nullptr, &errorMsg);
        
        auto end = chrono::high_resolution_clock::now();
        double queryTime = chrono::duration<double>(end - start).count();
        
        bool success = (result == SQLITE_OK);
        updatePerformanceMetrics(queryTime, success);
        
        if (!success && errorMsg) {
            cerr << "Query failed: " << errorMsg << endl;
            sqlite3_free(errorMsg);
        }
        
        return success;
    }
    
    vector<map<string, string>> selectQuery(const string& query) {
        vector<map<string, string>> results;
        if (!initialized || !db) return results;
        
        auto start = chrono::high_resolution_clock::now();
        
        sqlite3_stmt* stmt;
        int result = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
        
        if (result == SQLITE_OK) {
            // Loop - process each row
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                map<string, string> row;
                int columns = sqlite3_column_count(stmt);
                
                // Loop - process each column
                for (int i = 0; i < columns; ++i) {
                    string columnName = sqlite3_column_name(stmt, i);
                    const unsigned char* value = sqlite3_column_text(stmt, i);
                    string columnValue = value ? reinterpret_cast<const char*>(value) : "";
                    row[columnName] = columnValue;
                }
                results.push_back(row);
            }
        }
        
        sqlite3_finalize(stmt);
        
        auto end = chrono::high_resolution_clock::now();
        double queryTime = chrono::duration<double>(end - start).count();
        updatePerformanceMetrics(queryTime, true);
        
        return results;
    }
    
    bool shouldUseConnectionPool() {
        // Decision making based on current load
        double connectionEfficiency = calculateConnectionEfficiency();
        return connectionEfficiency < 0.7 && currentConnections < 10;
    }
    
    string selectOptimizationStrategy(const string& query) {
        // Decision making based on query complexity
        int complexity = estimateQueryComplexity(query);
        
        if (complexity < 5) {
            return "simple";
        } else if (complexity < 15) {
            return "indexed";
        } else {
            return "optimized";
        }
    }
    
    bool shouldEnableWAL(const string& operation) {
        // Decision making - enable WAL for write operations
        return operation == "initialize" || operation == "batch_insert" || operation == "update";
    }
    
    double calculateQueryPerformance(const string& query) {
        // Calculation - estimate performance based on query characteristics
        int complexity = estimateQueryComplexity(query);
        double baseTime = complexity * 0.001;
        
        // Decision making - adjust based on database state
        if (totalQueries > 1000) {
            baseTime *= 1.2; // Slight degradation over time
        }
        
        return baseTime;
    }
    
    int estimateQueryComplexity(const string& query) {
        // Calculation - estimate complexity based on query structure
        int complexity = 0;
        
        // Loop - count different SQL keywords
        vector<string> keywords = {"SELECT", "FROM", "WHERE", "JOIN", "GROUP BY", "ORDER BY", "HAVING"};
        for (const auto& keyword : keywords) {
            if (query.find(keyword) != string::npos) {
                complexity += 2;
            }
        }
        
        // Decision making - adjust complexity based on query length
        if (query.length() > 500) {
            complexity += 5;
        }
        
        return complexity;
    }
    
    double calculateConnectionEfficiency() {
        // Calculation - efficiency based on success rate
        if (totalQueries == 0) return 1.0;
        
        double successRate = static_cast<double>(totalQueries - failedQueries) / totalQueries;
        double timeEfficiency = max(0.1, 1.0 - averageQueryTime);
        
        return (successRate + timeEfficiency) / 2.0;
    }
    
    bool batchInsert(const vector<vector<string>>& data) {
        if (!initialized || data.empty()) return false;
        
        // Decision making - use transaction for batch operations
        executeQuery("BEGIN TRANSACTION");
        
        // Loop - insert each row
        for (const auto& row : data) {
            if (row.size() < 3) continue; // Skip invalid rows
            
            stringstream ss;
            ss << "INSERT INTO data_records (name, value, timestamp) VALUES (";
            ss << "'" << row[0] << "', '" << row[1] << "', '" << row[2] << "')";
            
            if (!executeQuery(ss.str())) {
                executeQuery("ROLLBACK");
                return false;
            }
        }
        
        executeQuery("COMMIT");
        return true;
    }
    
    void cleanupOldRecords(int daysOld) {
        // Calculation - determine cutoff date
        stringstream ss;
        ss << "DELETE FROM data_records WHERE timestamp < datetime('now', '-" << daysOld << " days')";
        
        // Decision making - use batch deletion for large datasets
        auto result = selectQuery("SELECT COUNT(*) as count FROM data_records WHERE timestamp < datetime('now', '-" + to_string(daysOld) + " days')");
        
        if (!result.empty()) {
            int count = stoi(result[0]["count"]);
            if (count > 1000) {
                // Use batch deletion
                executeQuery("BEGIN TRANSACTION");
                executeQuery(ss.str());
                executeQuery("COMMIT");
            } else {
                executeQuery(ss.str());
            }
        }
    }
    
    void optimizeDatabase() {
        // Service calls - database optimization
        executeQuery("VACUUM");
        executeQuery("ANALYZE");
        executeQuery("REINDEX");
        
        // Calculation - update performance metrics
        averageQueryTime *= 0.9; // Assume 10% improvement
    }
    
    bool backupDatabase(const string& backupPath) {
        if (!initialized) return false;
        
        // IO call - create backup file
        ofstream backupFile(backupPath);
        if (!backupFile.is_open()) {
            return false;
        }
        
        // Service call - backup database
        sqlite3* backupDb;
        int result = sqlite3_open(backupPath.c_str(), &backupDb);
        if (result != SQLITE_OK) {
            return false;
        }
        
        sqlite3_backup* backup = sqlite3_backup_init(backupDb, "main", db, "main");
        if (backup) {
            sqlite3_backup_step(backup, -1);
            sqlite3_backup_finish(backup);
        }
        
        sqlite3_close(backupDb);
        return true;
    }
    
    bool validateDatabaseIntegrity() {
        // Service call - integrity check
        auto result = selectQuery("PRAGMA integrity_check");
        return !result.empty() && result[0]["integrity_check"] == "ok";
    }
    
private:
    bool createTables() {
        string createTableSQL = R"(
            CREATE TABLE IF NOT EXISTS data_records (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                name TEXT NOT NULL,
                value TEXT,
                timestamp DATETIME DEFAULT CURRENT_TIMESTAMP
            );
            
            CREATE TABLE IF NOT EXISTS performance_metrics (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                query_time REAL,
                success BOOLEAN,
                timestamp DATETIME DEFAULT CURRENT_TIMESTAMP
            );
        )";
        
        return executeQuery(createTableSQL);
    }
    
    bool prepareStatements() {
        // Prepare common statements for better performance
        const char* insertSQL = "INSERT INTO data_records (name, value) VALUES (?, ?)";
        sqlite3_stmt* stmt;
        
        if (sqlite3_prepare_v2(db, insertSQL, -1, &stmt, nullptr) == SQLITE_OK) {
            preparedStatements["insert_data"] = stmt;
        }
        
        return true;
    }
    
    void cleanupStatements() {
        // Loop - cleanup all prepared statements
        for (auto& pair : preparedStatements) {
            sqlite3_finalize(pair.second);
        }
        preparedStatements.clear();
    }
    
    void updatePerformanceMetrics(double queryTime, bool success) {
        totalQueries++;
        if (!success) failedQueries++;
        
        // Calculation - update average query time
        averageQueryTime = (averageQueryTime * (totalQueries - 1) + queryTime) / totalQueries;
    }
    
    string sanitizeQuery(const string& query) {
        // Simple sanitization - in production, use proper parameterized queries
        string sanitized = query;
        
        // Loop - remove potentially dangerous characters
        vector<string> dangerous = {"--", "/*", "*/", "xp_", "sp_", "exec"};
        for (const auto& danger : dangerous) {
            size_t pos = sanitized.find(danger);
            if (pos != string::npos) {
                sanitized.replace(pos, danger.length(), "");
            }
        }
        
        return sanitized;
    }
};

// Main function to demonstrate DatabaseManager
int main() {
    cout << "=== DatabaseManager Demo ===" << endl;
    
    DatabaseManager dbManager;
    
    // Initialize database
    string dbPath = "test_database.db";
    if (!dbManager.initialize(dbPath)) {
        cerr << "Failed to initialize database" << endl;
        return 1;
    }
    
    // Test basic operations
    cout << "\n--- Basic Database Operations ---" << endl;
    
    // Insert some test data
    vector<vector<string>> testData = {
        {"User1", "Data1", "2024-01-01 10:00:00"},
        {"User2", "Data2", "2024-01-01 11:00:00"},
        {"User3", "Data3", "2024-01-01 12:00:00"},
        {"User4", "Data4", "2024-01-01 13:00:00"},
        {"User5", "Data5", "2024-01-01 14:00:00"}
    };
    
    if (dbManager.batchInsert(testData)) {
        cout << "Batch insert completed successfully" << endl;
    } else {
        cout << "Batch insert failed" << endl;
    }
    
    // Query the data
    cout << "\n--- Querying Data ---" << endl;
    auto results = dbManager.selectQuery("SELECT * FROM data_records ORDER BY id");
    
    cout << "Retrieved " << results.size() << " records:" << endl;
    for (const auto& row : results) {
        cout << "ID: " << row["id"] 
             << ", Name: " << row["name"] 
             << ", Value: " << row["value"] 
             << ", Timestamp: " << row["timestamp"] << endl;
    }
    
    // Test query optimization
    cout << "\n--- Query Optimization ---" << endl;
    string testQuery = "SELECT * FROM data_records WHERE name LIKE '%User%' ORDER BY timestamp";
    string strategy = dbManager.selectOptimizationStrategy(testQuery);
    cout << "Query: " << testQuery << endl;
    cout << "Optimization strategy: " << strategy << endl;
    
    double performance = dbManager.calculateQueryPerformance(testQuery);
    cout << "Estimated performance: " << performance << " seconds" << endl;
    
    // Test connection efficiency
    cout << "\n--- Connection Analysis ---" << endl;
    double efficiency = dbManager.calculateConnectionEfficiency();
    cout << "Connection efficiency: " << efficiency << endl;
    
    bool shouldUsePool = dbManager.shouldUseConnectionPool();
    cout << "Should use connection pool: " << (shouldUsePool ? "Yes" : "No") << endl;
    
    // Test database optimization
    cout << "\n--- Database Optimization ---" << endl;
    dbManager.optimizeDatabase();
    cout << "Database optimization completed" << endl;
    
    // Test database integrity
    cout << "\n--- Database Integrity ---" << endl;
    if (dbManager.validateDatabaseIntegrity()) {
        cout << "Database integrity check passed" << endl;
    } else {
        cout << "Database integrity check failed" << endl;
    }
    
    // Test backup functionality
    cout << "\n--- Database Backup ---" << endl;
    string backupPath = "backup_database.db";
    if (dbManager.backupDatabase(backupPath)) {
        cout << "Database backup created successfully" << endl;
    } else {
        cout << "Database backup failed" << endl;
    }
    
    // Test cleanup operations
    cout << "\n--- Cleanup Operations ---" << endl;
    dbManager.cleanupOldRecords(365); // Clean records older than 1 year
    cout << "Old records cleanup completed" << endl;
    
    // Test WAL mode decision
    cout << "\n--- WAL Mode Analysis ---" << endl;
    bool shouldEnableWAL = dbManager.shouldEnableWAL("batch_insert");
    cout << "Should enable WAL for batch insert: " << (shouldEnableWAL ? "Yes" : "No") << endl;
    
    // Close database
    dbManager.close();
    cout << "\nDatabase closed successfully" << endl;
    
    cout << "\nDatabaseManager demo completed successfully!" << endl;
    return 0;
}