#ifndef IUSER_H
#define IUSER_H

#include <string>
#include <chrono>

using namespace std;
using namespace chrono;

/**
 * @class IUser
 * @brief This interface will be used when implementing both student and tutor
 */
class IUser {
public:
    virtual const string& getId() const = 0;

    virtual const string& getEmail() const = 0;
    virtual void setEmail(const string& email) = 0;

    virtual const string& getPassword() const = 0;
    virtual void setPassword(const string& password) = 0;

    virtual const string& getName() const = 0;
    virtual void setName(const string& name) = 0;

    virtual time_t getCreatedAt() const = 0;


    // virtual bool signUp(const string& email, const string& password, const ) = 0;
    // virtual bool login(const string& email, const string& password) = 0;
    // virtual void logout() = 0;

    // virtual const Course getCourseDetail(const string& subject) const = 0;
    virtual const set<string> getSubjects() const = 0;
    virtual void addSubject(const string& subject) = 0;
    virtual void removeSubject(const string& subject) = 0;
    // virtual void removeAllSubjects() = 0;

    // virtual void sendMessage(int recipientId) = 0;
    // virtual void readMessage(int recipientId) = 0;
    // virtual void viewAllMessageRecipent() = 0;

    virtual ~IUser(){}; // IUser deconstructor
};

#endif // IUSER_H
