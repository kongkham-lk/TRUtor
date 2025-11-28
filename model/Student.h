#ifndef STUDENT_H
#define STUDENT_H

#include <string>
#include <sstream>
#include <fstream>
#include "User.h"

using namespace std;
using namespace chrono;

class Student : public User { 
private:
    string major;

public:
    Student(); 
    Student(string id, const string& email, const string& password, const string& name, const string& major); 

    const string& getMajor() const;
    void setMajor(const string& major);

    //Stream operators for file persistence
    friend std::ostream& operator<<(std::ostream& os, const Student& student);
    friend std::istream& operator>>(std::istream& is, Student& student);

    ~Student();
};

#endif // STUDENT_H