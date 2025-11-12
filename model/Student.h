#ifndef STUDENT_H
#define STUDENT_H

#include <string>
#include "User.h"

using namespace std;
using namespace chrono;

// This class is used for DB mapping as well
class Student : public User { // extend User class
private:
    string major;

public:
    Student(string id, const string& email, const string& password, const string& name); // Constructor, invoke when signup

    const string& getMajor() const;
    void setMajor(const string& major);

    ~Student(); // Destructor
};

#endif // STUDENT_H
