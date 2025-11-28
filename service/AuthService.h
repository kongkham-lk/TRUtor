#ifndef AUTHSERVICE_H
#define AUTHSERVICE_H

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "../model/Student.h"
#include "../model/Tutor.h"
#include "../model/User.h"

using namespace std;

class AuthService {
public:
    AuthService();

    //Signup student
    bool signUpStudent(const string& email,
        const string& password,
        const string& name,
        const string& major);

    std::vector<User> getAllUsers() const;

    //Signup tutor
    bool signUpTutor(const string& email,
        const string& password,
        const string& name);

    //Login definition
    bool login(const string& email,
        const string& password);

    //Logout definition
    void logout();

    //Check if logged in
    bool isLoggedIn() const;
    int currentRole() const;       
    string currentUserId() const;  

    Student currentStudent() const;
    Tutor currentTutor() const;

private:
    bool userExistsByEmail(const string& email) const; 
    string generateId(int role) const;   

    //Helper to save user data to file
    bool saveNewUser(const User& user, const string& major); 

    void loadSession();         
    void saveSession() const;   
    void clearSession() const;  

    //The member variables that track the session state
    string loggedInUserId;
    int loggedInUserRole;       

};

#endif // AUTHSERVICE_H