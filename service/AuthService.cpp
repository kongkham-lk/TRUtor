#include "AuthService.h"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

AuthService::AuthService()
    : loggedInUserId(""), loggedInUserRole(-1) {
    loadSession();
}

// Check if email already used
bool AuthService::userExistsByEmail(const string& email) const {
    ifstream file("users.txt");
    if (!file.is_open()) return false;

    string id, em, pw, name, major;
    int role;

    while (file >> id >> em >> pw >> name >> role >> major) {
        if (em == email) return true;
    }
    return false;
}

// Simple ID generator: S1, S2 or T1, T2...
string AuthService::generateId(int role) const {
    ifstream file("users.txt");
    char prefix = (role == 0 ? 'S' : 'T');
    int maxNum = 0;

    if (!file.is_open()) {
        return string(1, prefix) + "1";
    }

    string id, em, pw, name, major;
    int r;

    while (file >> id >> em >> pw >> name >> r >> major) {
        if (!id.empty() && id[0] == prefix) {
            try {
                int num = stoi(id.substr(1));
                if (num > maxNum) maxNum = num;
            } catch (...) {}
        }
    }

    return string(1, prefix) + to_string(maxNum + 1);
}

//Student signup logic
bool AuthService::signUpStudent(const string& email,const string& password,const string& name,const string& major) {
    if (userExistsByEmail(email)) return false; //if email exists

    string id = generateId(0);   // generate student ID

    ofstream file("users.txt", ios::app);
    if (!file.is_open()) return false; //if file cannot be opened

    file << id << " " << email << " " << password << " " << name << " " << 0 << " " << major << "\n"; //save to file

    return true;
}

// Tutor signup (major is "null")
bool AuthService::signUpTutor(const string& email,const string& password,const string& name) {
    if (userExistsByEmail(email)) return false; //if email exists already

    string id = generateId(1);  // generate tutor ID

    ofstream file("users.txt", ios::app);
    if (!file.is_open()) return false; //if file cannot be opened

    file << id << " " << email << " " << password << " " << name << " " << 1 << " " << "null" << "\n";   //save to file

    return true;
}

// Login logic
bool AuthService::login(const string& email,const string& password) {
    ifstream file("users.txt");
    if (!file.is_open()) return false; //if file cannot be opened

    string id, em, pw, name, major;  //major is unused (null) for tutor
    int role; // 0 = student, 1 = tutor

    while (file >> id >> em >> pw >> name >> role >> major) {
        if (em == email && pw == password) {
            //store current logged in user info
            loggedInUserId = id;
            loggedInUserRole = role;
            saveSession(); //save session
            return true;
        }
    }

    return false;
}

// Logout logic
void AuthService::logout() {
    loggedInUserId = "";   // clear current user info
    loggedInUserRole = -1; // reset role
    clearSession(); // clear session file
}

//check if logged in already
bool AuthService::isLoggedIn() const {
    return loggedInUserRole == 0 || loggedInUserRole == 1; // true if student or tutor logged in
}

// get current role
int AuthService::currentRole() const {
    return loggedInUserRole;
}

// get current user ID
string AuthService::currentUserId() const {
    return loggedInUserId;
}

// Load the current student
Student AuthService::currentStudent() const {
    if (!isLoggedIn() || loggedInUserRole != 0)
        throw runtime_error("It is not a student account");

    ifstream file("users.txt"); // open users file
    if (!file.is_open()) throw runtime_error("Cannot open users file.");

    string id, em, pw, name, major;
    int role;

    while (file >> id >> em >> pw >> name >> role >> major) {
        if (id == loggedInUserId && role == 0) { //if found the student
            return Student(id, em, pw, name, major); //return a student object with the data
        }
    }

    throw runtime_error("Student not found");
}

// Load the current tutor
Tutor AuthService::currentTutor() const {
    if (!isLoggedIn() || loggedInUserRole != 1)
        throw runtime_error("Not a tutor account");

    ifstream file("users.txt"); // open users file
    if (!file.is_open()) throw runtime_error("Cannot open users file.");

    string id, em, pw, name, major;
    int role;

    while (file >> id >> em >> pw >> name >> role >> major) {
        if (id == loggedInUserId && role == 1) {
            return Tutor(id, em, pw, name); //return a tutor object with the data, no major
        }
    }

    throw runtime_error("Tutor not found");
}

// load session from session.txt
void AuthService::loadSession() {
    ifstream file("session.txt");
    if (!file.is_open()) { //if file cannot be opened
        loggedInUserId = "";
        loggedInUserRole = -1;
        return;
    }

    string roleStrin; // string to hold role from file

    getline(file, loggedInUserId);
    getline(file, roleStrin);

    if (loggedInUserId.empty() || roleStrin.empty()) {
        loggedInUserId = "";
        loggedInUserRole = -1;
        return;
    }

    loggedInUserRole = stoi(roleStrin); //convert string role to int
}

// save session to session.txt
void AuthService::saveSession() const {
    ofstream file("session.txt");
    if (!file.is_open()) return;

    file << loggedInUserId << "\n" << loggedInUserRole << "\n"; // save ID and role
}

// clear session file
void AuthService::clearSession() const {
    ofstream file("session.txt", ios::trunc); // open session file in truncate mode to clear it
}
