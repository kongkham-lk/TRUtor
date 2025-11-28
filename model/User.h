#ifndef USER_H
#define USER_H

#include <string>
#include <set>
#include <chrono>
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime> // Needed for time_t definition
#include "../interface/IUser.h"

using namespace std;
using namespace chrono;

// This class is used for DB mapping as well
class User : public IUser { // implement IUser interface
private:
    string id;
    string email;
    string password;
    string name;
    int role; // 0 if student, 1 if tutor
    set<string> subjects;
    // Internal storage uses the modern C++ type
    system_clock::time_point createdAt;

public:
    User(); // Default constructor for file loading
    // FIX C2511: Changed 'string id' to 'const string& id'
    User(const string& id, const string& email, const string& password, const string& name, int role); // Constructor

    // Override functions
    const string& getId() const override;

    const string& getEmail() const override;
    void setEmail(const string& email) override;

    const string& getPassword() const override;
    void setPassword(const string& password) override;

    const string& getName() const override;
    void setName(const string& name) override;

    // FIX: Public getter returns the requested legacy type (time_t)
    time_t getCreatedAt() const override;

    int getRole() const; // for checking what role is the user

    const set<string> getSubjects() const override;
    void addSubject(const string& subject) override;
    void removeSubject(const string& subject) override;

    // Stream operators for file persistence
    friend std::ostream& operator<<(std::ostream& os, const User& user);
    friend std::istream& operator>>(std::istream& is, User& user);

    ~User(); // Destructor
};

#endif // USER_H