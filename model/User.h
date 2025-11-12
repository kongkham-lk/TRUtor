#ifndef USER_H
#define USER_H

#include <string>
#include <chrono>
#include "../interface/IUser.h"

using namespace std;
using namespace chrono;

// This class is used for DB mapping as well
class User : public IUser { // implement IUser interface
public:
    User(string id, const string& email, const string& password, const string& name); // Constructor

    // Overrided functions
    const string& getId() const override;
    const string& getEmail() const override;
    const string& getPassword() const override;
    const string& getName() const override;
    system_clock::time_point getCreatedAt() const override;

    void setEmail(const string& email) override;
    void setPassword(const string& password) override;
    void setName(const string& name) override;

    // bool signUp(const string& email, const string& password) override;
    // bool login(const string& email, const string& password) override;
    // void logout() override;

    // const Course getCourseDetail(const string& subject) const override;
    // const vector<string> getAllSubjects() const override;
    // void addSubject(const string& subject) override;
    // void removeSubject(const string& subject) override;
    // void removeAllSubjects() override;

    // void sendMessage(int recipientId) override;
    // void readMessage(int recipientId) override;
    // void viewAllMessageRecipent() override;

    ~User(); // Destructor

private:
    string id;
    string email;
    string password;
    string name;
    system_clock::time_point createdAt;
    bool isLoggedIn;
};

#endif // USER_H
