#ifndef AUTHSERVICE_H
#define AUTHSERVICE_H

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "../model/Student.h"
#include "../model/Tutor.h"
#include "../model/User.h" // Include User for internal helpers

using namespace std;

class AuthService {
public:
    AuthService();

    // Signup student
    bool signUpStudent(const string& email,
        const string& password,
        const string& name,
        const string& major);

    std::vector<User> getAllUsers() const;

    // Signup tutor
    bool signUpTutor(const string& email,
        const string& password,
        const string& name);

    // Login definition
    bool login(const string& email,
        const string& password);

    // Logout definition
    void logout();

    // check if logged in
    bool isLoggedIn() const;
    int currentRole() const;       // 0 = student, 1 = tutor, -1 = none
    string currentUserId() const;  // just the ID string of current user

    // Object access (reconstruct from file)
    Student currentStudent() const;
    Tutor currentTutor() const;

private:
    bool userExistsByEmail(const string& email) const; // check if email exists
    string generateId(int role) const;   // S1, S2... or T1, T2... using the role to decide id

    // NEW: Helper to save user data to file
    bool saveNewUser(const User& user, const string& major); // <-- ADDED THIS DECLARATION

    void loadSession();         // load session from file
    void saveSession() const;   // save session to file
    void clearSession() const;  // clear session file

    // The member variables that track the session state
    string loggedInUserId;
    int loggedInUserRole;       // 0 = student, 1 = tutor, -1 = none

};

#endif // AUTHSERVICE_H