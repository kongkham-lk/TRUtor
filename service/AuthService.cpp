#include "AuthService.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <limits>
#include "../model/User.h"
#include "../model/Student.h"
#include "../model/Tutor.h"

using namespace std;
using namespace chrono;

// --- Private Helper Struct for Consistent Reading ---
// NOTE: This is a freestanding struct, so it does not need AuthService:: scope.
struct UserRecord {
    string id;
    string email;
    string password;
    string name;
    int role;
    string major; // for Student, 'N/A' for Tutor
    time_point<system_clock> createdAt;
    bool isValid = false;
};

/**
 * @brief Parses a single line from users.txt (pipe-delimited) into a UserRecord.
 */
UserRecord parseUserLine(const string& line) {
    UserRecord record;
    if (line.empty()) return record;

    stringstream ss(line);
    string id, email, password, name, roleStr, timestampStr, major;
    long long timestamp_ll;
    string tempSeparator;

    // File format: ID|EMAIL|PASSWORD|NAME|ROLE|TIMESTAMP||MAJOR
    if (getline(ss, id, '|') &&
        getline(ss, email, '|') &&
        getline(ss, password, '|') &&
        getline(ss, name, '|') &&
        getline(ss, roleStr, '|') &&
        getline(ss, timestampStr, '|') &&
        getline(ss, tempSeparator, '|') &&
        getline(ss, major))
    {
        try {
            record.role = stoi(roleStr);
        }
        catch (...) {
            cerr << "Error parsing role from file: " << roleStr << endl;
            return record;
        }

        try {
            timestamp_ll = stoll(timestampStr);
            record.createdAt = system_clock::from_time_t((time_t)timestamp_ll);
        }
        catch (...) {
            record.createdAt = system_clock::now();
        }

        record.id = id;
        record.email = email;
        record.password = password;
        record.name = name;
        record.major = major;
        record.isValid = true;
    }
    return record;
}


// --- AuthService Implementations (Scope Added) ---

// Constructor
AuthService::AuthService()
    : loggedInUserId(""), loggedInUserRole(-1) {
    loadSession();
}

// Check if email already used
bool AuthService::userExistsByEmail(const string& email) const { // FIX: Added AuthService::
    ifstream file("users.txt");
    if (!file.is_open()) return false;

    string line;
    while (getline(file, line)) {
        UserRecord record = parseUserLine(line);
        if (record.isValid && record.email == email) {
            return true;
        }
    }
    return false;
}

// Simple ID generator
string AuthService::generateId(int role) const { // FIX: Added AuthService::
    ifstream file("users.txt");
    char prefix = (role == 0 ? 'S' : 'T');
    int maxNum = 0;

    if (!file.is_open()) {
        return string(1, prefix) + "1";
    }

    string line;
    while (getline(file, line)) {
        UserRecord record = parseUserLine(line);
        if (record.isValid && !record.id.empty() && record.id[0] == prefix) {
            try {
                int num = stoi(record.id.substr(1));
                if (num > maxNum) {
                    maxNum = num;
                }
            }
            catch (...) {
                // Ignore malformed IDs
            }
        }
    }
    return string(1, prefix) + to_string(maxNum + 1);
}

// New Helper function
bool AuthService::saveNewUser(const User& user, const string& major) { // FIX: Added AuthService::
    if (userExistsByEmail(user.getEmail())) {
        cerr << "Error: User with this email already exists." << endl;
        return false;
    }

    time_t createdAt_t = system_clock::to_time_t(user.getCreatedAt());

    ofstream file("users.txt", ios::app);
    if (!file.is_open()) {
        cerr << "Error: Cannot open users.txt file for writing." << endl;
        return false;
    }

    // New format: ID|EMAIL|PASSWORD|NAME|ROLE|TIMESTAMP||MAJOR
    file << user.getId() << "|"
        << user.getEmail() << "|"
        << user.getPassword() << "|"
        << user.getName() << "|"
        << user.getRole() << "|"
        << createdAt_t << "||"
        << major << "\n";

    file.close();

    // Member variables are now correctly accessible
    loggedInUserId = user.getId();
    loggedInUserRole = user.getRole();
    saveSession();

    return true;
}


bool AuthService::signUpStudent(const string& email, const string& password, const string& name, const string& major) { // FIX: Added AuthService::
    string id = generateId(0);
    Student newStudent(id, email, password, name, major);

    return saveNewUser(newStudent, major);
}

bool AuthService::signUpTutor(const string& email, const string& password, const string& name) { // FIX: Added AuthService::
    string id = generateId(1);
    Tutor newTutor(id, email, password, name);

    return saveNewUser(newTutor, "N/A");
}

// Login definition
bool AuthService::login(const string& email, const string& password) { // FIX: Added AuthService::
    ifstream file("users.txt");
    if (!file.is_open()) return false;

    string line;
    while (getline(file, line)) {
        UserRecord record = parseUserLine(line);
        if (record.isValid && record.email == email && record.password == password) {
            loggedInUserId = record.id;
            loggedInUserRole = record.role;
            saveSession();
            return true;
        }
    }
    return false;
}

// Logout definition
void AuthService::logout() { // FIX: Added AuthService::
    loggedInUserId = "";
    loggedInUserRole = -1;
    clearSession();
}

// check if logged in
bool AuthService::isLoggedIn() const { // FIX: Added AuthService::
    return !loggedInUserId.empty();
}

// current role
int AuthService::currentRole() const { // FIX: Added AuthService::
    return loggedInUserRole;
}

// current user ID
string AuthService::currentUserId() const { // FIX: Added AuthService::
    return loggedInUserId;
}

// Object access - Student
Student AuthService::currentStudent() const { // FIX: Added AuthService::
    if (loggedInUserId.empty() || loggedInUserRole != 0)
        throw runtime_error("Not a student account or not logged in");

    ifstream file("users.txt");
    if (!file.is_open()) throw runtime_error("Cannot open users file.");

    string line;
    while (getline(file, line)) {
        UserRecord record = parseUserLine(line);
        if (record.isValid && record.id == loggedInUserId && record.role == 0) {
            return Student(record.id, record.email, record.password, record.name, record.major);
        }
    }
    throw runtime_error("Student not found");
}

// Object access - Tutor
Tutor AuthService::currentTutor() const { // FIX: Added AuthService::
    if (loggedInUserId.empty() || loggedInUserRole != 1)
        throw runtime_error("Not a tutor account or not logged in");

    ifstream file("users.txt");
    if (!file.is_open()) throw runtime_error("Cannot open users file.");

    string line;
    while (getline(file, line)) {
        UserRecord record = parseUserLine(line);
        if (record.isValid && record.id == loggedInUserId && record.role == 1) {
            return Tutor(record.id, record.email, record.password, record.name);
        }
    }
    throw runtime_error("Tutor not found");
}


/**
 * @brief Reads all user data from users.txt and returns them as a vector of User objects.
 */
std::vector<User> AuthService::getAllUsers() const { // FIX: Added AuthService::
    std::vector<User> allUsers;
    std::ifstream file("users.txt");
    if (!file.is_open()) {
        std::cerr << "Warning: Cannot open users.txt file." << std::endl;
        return allUsers;
    }

    std::string line;
    while (getline(file, line)) {
        UserRecord record = parseUserLine(line);
        if (record.isValid) {
            allUsers.push_back(User(record.id, record.email, record.password, record.name, record.role));
        }
    }
    file.close();
    return allUsers;
}


// load session from session.txt
void AuthService::loadSession() { // FIX: Added AuthService::
    ifstream file("session.txt");
    if (!file.is_open()) {
        loggedInUserId = "";
        loggedInUserRole = -1;
        return;
    }

    string roleStrin;

    getline(file, loggedInUserId);
    getline(file, roleStrin);

    if (loggedInUserId.empty() || roleStrin.empty()) {
        loggedInUserId = "";
        loggedInUserRole = -1;
        return;
    }

    try {
        loggedInUserRole = stoi(roleStrin);
    }
    catch (...) {
        loggedInUserId = "";
        loggedInUserRole = -1;
    }
}

// save session to session.txt
void AuthService::saveSession() const { // FIX: Added AuthService::
    ofstream file("session.txt");
    if (!file.is_open()) {
        cerr << "Error: Cannot open session.txt for writing." << endl;
        return;
    }
    file << loggedInUserId << "\n";
    file << loggedInUserRole << "\n";
    file.close();
}

// clear session file
void AuthService::clearSession() const { // FIX: Added AuthService:: and const
    ofstream file("session.txt", ios::trunc);
    if (!file.is_open()) {
        cerr << "Error: Cannot open session.txt for clearing." << endl;
    }
}