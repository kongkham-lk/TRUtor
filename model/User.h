#ifndef USER_H
#define USER_H

#include <string>
#include <set>
#include <chrono>
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include "../interface/IUser.h"

using namespace std;
using namespace chrono;

class User : public IUser {
private:
    string id;
    string email;
    string password;
    string name;
    int role; 
    set<string> subjects;
    system_clock::time_point createdAt;

public:
    User(); 
    User(const string& id, const string& email, const string& password, const string& name, int role); 

    //Override functions
    const string& getId() const override;
    const string& getEmail() const override;
    void setEmail(const string& email) override;
    const string& getPassword() const override;
    void setPassword(const string& password) override;
    const string& getName() const override;
    void setName(const string& name) override;
    time_t getCreatedAt() const override;
    int getRole() const; 

    const set<string> getSubjects() const override;
    void addSubject(const string& subject) override;
    void removeSubject(const string& subject) override;

    //Stream operators for file persistence
    friend std::ostream& operator<<(std::ostream& os, const User& user);
    friend std::istream& operator>>(std::istream& is, User& user);

    ~User(); // Destructor
};

#endif // USER_H