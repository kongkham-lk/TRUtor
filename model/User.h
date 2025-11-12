#ifndef USER_H
#define USER_H

#include <string>
#include <set>
#include <chrono>
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
    int role; // 0 if student, 1 if tutor, (OPTIONAL) 2 if both student and tutor
    set<string> subjects;
    system_clock::time_point createdAt;

public:
    User(string id, const string& email, const string& password, const string& name, int role); // Constructor

    // Overrided functions
    const string& getId() const override;

    const string& getEmail() const override;
    void setEmail(const string& email) override;

    const string& getPassword() const override;
    void setPassword(const string& password) override;

    const string& getName() const override;
    void setName(const string& name) override;

    system_clock::time_point getCreatedAt() const override;

    int getRole() const; // for checking what role is the user

    const set<string> getSubjects() const override;
    void addSubject(const string& subject) override;
    void removeSubject(const string& subject) override;

    ~User(); // Destructor
};

#endif // USER_H
