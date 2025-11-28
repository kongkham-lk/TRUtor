#ifndef STUDENT_H
#define STUDENT_H

#include <string>
#include <sstream>
#include <fstream>
#include "User.h"

using namespace std;
using namespace chrono;

// This class is used for DB mapping as well
class Student : public User { // extend User class
private:
    string major;

public:
    Student(); // Default constructor for file loading
    Student(string id, const string& email, const string& password, const string& name, const string& major); // Constructor, invoke when signup

    const string& getMajor() const;
    void setMajor(const string& major);

    // Stream operators for file persistence
    friend std::ostream& operator<<(std::ostream& os, const Student& student);
    friend std::istream& operator>>(std::istream& is, Student& student);

    ~Student(); // Destructor
};

#endif // STUDENT_H