#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <curl/curl.h>
#include <grpcpp/grpcpp.h>
#include <sqlite3.h>
#include <boost/beast/http.hpp>
#include <poco/net/httpclient.h>

using namespace std;

// Service class examples that the parser detects
class UserService {
public:
    string getUserDetails(int userId) {
        // Service call pattern: getUserDetails
        return "User details for ID: " + to_string(userId);
    }
    
    bool authenticateUser(string username, string password) {
        // Service call pattern: authenticateUser
        return true;
    }
    
    void createUser(string userData) {
        // Service call pattern: createUser
        cout << "Creating user: " << userData << endl;
    }
};

class DatabaseService {
public:
    bool executeQuery(string query) {
        // Service call pattern: executeQuery
        return true;
    }
    
    void connectDatabase(string connectionString) {
        // Service call pattern: connectDatabase
        cout << "Connecting to database: " << connectionString << endl;
    }
};

class HttpService {
public:
    string sendRequest(string url, string data) {
        // Service call pattern: sendRequest
        return "Response from: " + url;
    }
    
    void receiveResponse(string response) {
        // Service call pattern: receiveResponse
        cout << "Received: " << response << endl;
    }
};

class MessageQueueService {
public:
    void publishMessage(string topic, string message) {
        // Service call pattern: publishMessage
        cout << "Publishing to topic: " << topic << endl;
    }
    
    string subscribeToTopic(string topic) {
        // Service call pattern: subscribeToTopic
        return "Subscribed to: " + topic;
    }
};

// Main application class
class Application {
private:
    UserService userService;
    DatabaseService dbService;
    HttpService httpService;
    MessageQueueService mqService;
    
public:
    void processUserLogin(string username, string password) {
        // Service call detection: userService.authenticateUser()
        bool isAuthenticated = userService.authenticateUser(username, password);
        
        if (isAuthenticated) {
            // Service call detection: userService.getUserDetails()
            string userDetails = userService.getUserDetails(123);
            
            // Service call detection: dbService.executeQuery()
            dbService.executeQuery("SELECT * FROM users WHERE id = 123");
            
            // Service call detection: httpService.sendRequest()
            string response = httpService.sendRequest("https://api.example.com/user", userDetails);
            
            // Service call detection: mqService.publishMessage()
            mqService.publishMessage("user.login", "User logged in successfully");
        }
    }
    
    void createNewUser(string userData) {
        // Service call detection: userService.createUser()
        userService.createUser(userData);
        
        // Service call detection: dbService.connectDatabase()
        dbService.connectDatabase("mysql://localhost:3306/users");
        
        // Service call detection: httpService.receiveResponse()
        httpService.receiveResponse("User created successfully");
    }
    
    void handleMessageQueue() {
        // Service call detection: mqService.subscribeToTopic()
        string subscription = mqService.subscribeToTopic("notifications");
        
        // Service call detection: mqService.publishMessage()
        mqService.publishMessage("system.status", "Service is running");
    }
    
    // HTTP client using cURL (detected by parser patterns)
    void makeHttpCall() {
        CURL* curl = curl_easy_init();
        if (curl) {
            // Service call detection: curl_easy_setopt
            curl_easy_setopt(curl, CURLOPT_URL, "https://api.example.com/data");
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
            
            // Service call detection: curl_easy_perform
            CURLcode res = curl_easy_perform(curl);
            
            curl_easy_cleanup(curl);
        }
    }
    
    // gRPC client example (detected by parser patterns)
    void makeGrpcCall() {
        // Service call detection: CreateChannel
        auto channel = grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials());
        
        // Service call detection: ClientContext::stub
        // auto stub = MyService::NewStub(channel);
    }
    
    // Database operations (detected by parser patterns)
    void performDatabaseOperations() {
        sqlite3* db;
        
        // Service call detection: sqlite3_open
        int rc = sqlite3_open("test.db", &db);
        
        if (rc == SQLITE_OK) {
            // Service call detection: sqlite3_exec
            sqlite3_exec(db, "CREATE TABLE users (id INTEGER PRIMARY KEY, name TEXT);", nullptr, nullptr, nullptr);
            sqlite3_close(db);
        }
    }
    
    // Boost.Beast HTTP example (detected by parser patterns)
    void makeBoostHttpCall() {
        // Service call detection: http::write, http::read
        // boost::beast::http::request<boost::beast::http::string_body> req;
        // boost::beast::http::write(stream, req);
        // boost::beast::http::read(stream, buffer, res);
    }
    
    // Poco HTTP client example (detected by parser patterns)
    void makePocoHttpCall() {
        // Service call detection: sendRequest, receiveResponse
        // Poco::Net::HTTPClientSession session("example.com");
        // session.sendRequest(request);
        // session.receiveResponse(response);
    }
};

// Global service functions (detected by parser patterns)
void logServiceCall(string serviceName, string operation) {
    cout << "Service call: " << serviceName << " -> " << operation << endl;
}

void auditServiceAccess(string userId, string serviceName) {
    cout << "Audit: User " << userId << " accessed " << serviceName << endl;
}

// Main function demonstrating service calls
int main() {
    Application app;
    
    // Service call detection: app.processUserLogin()
    app.processUserLogin("john_doe", "password123");
    
    // Service call detection: app.createNewUser()
    app.createNewUser("{\"name\": \"Jane Doe\", \"email\": \"jane@example.com\"}");
    
    // Service call detection: app.handleMessageQueue()
    app.handleMessageQueue();
    
    // Service call detection: app.makeHttpCall()
    app.makeHttpCall();
    
    // Service call detection: app.makeGrpcCall()
    app.makeGrpcCall();
    
    // Service call detection: app.performDatabaseOperations()
    app.performDatabaseOperations();
    
    // Global service function calls
    logServiceCall("UserService", "authenticateUser");
    auditServiceAccess("john_doe", "DatabaseService");
    
    return 0;
} 