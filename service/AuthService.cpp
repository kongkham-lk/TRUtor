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

struct UserRecord {
    string id;
    string email;
    string password;
    string name;
    int role;
    string major; 
    time_point<system_clock> createdAt;
    bool isValid = false;
};


UserRecord parseUserLine(const string& line) {
    UserRecord record;
    if (line.empty()) return record;

    stringstream ss(line);
    string id, email, password, name, roleStr, timestampStr, major;
    long long timestamp_ll;
    string tempSeparator;

    //File format ID|EMAIL|PASSWORD|NAME|ROLE|TIMESTAMP||MAJOR
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
            //Conversion from time_t in long long to time_point
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

// Constructor
AuthService::AuthService()
    : loggedInUserId(""), loggedInUserRole(-1) {
    loadSession();
}

//Check if email already used
bool AuthService::userExistsByEmail(const string& email) const {
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

//ID generator
string AuthService::generateId(int role) const {
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
            }
        }
    }
    return string(1, prefix) + to_string(maxNum + 1);
}

bool AuthService::saveNewUser(const User& user, const string& major) {
    if (userExistsByEmail(user.getEmail())) {
        cerr << "Error: User with this email already exists." << endl;
        return false;
    }
    time_t createdAt_t = user.getCreatedAt();

    ofstream file("users.txt", ios::app);
    if (!file.is_open()) {
        cerr << "Error: Cannot open users.txt file for writing." << endl;
        return false;
    }
    file << user.getId() << "|"
        << user.getEmail() << "|"
        << user.getPassword() << "|"
        << user.getName() << "|"
        << user.getRole() << "|"
        << createdAt_t << "||"
        << major << "\n";

    file.close();

    loggedInUserId = user.getId();
    loggedInUserRole = user.getRole();
    saveSession();

    return true;
}


bool AuthService::signUpStudent(const string& email, const string& password, const string& name, const string& major) {
    string id = generateId(0);
    // Student constructor uses the fixed User base constructor
    Student newStudent(id, email, password, name, major);

    return saveNewUser(newStudent, major);
}

bool AuthService::signUpTutor(const string& email, const string& password, const string& name) {
    string id = generateId(1);
    //Tutor constructor uses the fixed User base constructor
    Tutor newTutor(id, email, password, name);

    return saveNewUser(newTutor, "N/A");
}

//Login definition
bool AuthService::login(const string& email, const string& password) {
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

//Logout definition
void AuthService::logout() {
    loggedInUserId = "";
    loggedInUserRole = -1;
    clearSession();
}

//Check if logged in
bool AuthService::isLoggedIn() const {
    return !loggedInUserId.empty();
}

//Current role
int AuthService::currentRole() const {
    return loggedInUserRole;
}

//Current user ID
string AuthService::currentUserId() const {
    return loggedInUserId;
}

//Student access
Student AuthService::currentStudent() const {
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

//Tutor access
Tutor AuthService::currentTutor() const {
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

std::vector<User> AuthService::getAllUsers() const {
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


//Load session from session.txt
void AuthService::loadSession() {
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

//Save session to session.txt
void AuthService::saveSession() const {
    ofstream file("session.txt");
    if (!file.is_open()) {
        cerr << "Error: Cannot open session.txt for writing." << endl;
        return;
    }
    file << loggedInUserId << "\n";
    file << loggedInUserRole << "\n";
    file.close();
}

//Clear session file
void AuthService::clearSession() const {
    ofstream file("session.txt", ios::trunc);
    if (!file.is_open()) {
        cerr << "Error: Cannot open session.txt for clearing." << endl;
    }
}