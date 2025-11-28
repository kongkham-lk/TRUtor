#include <iostream>
#include <sstream>
#include <fstream>
#include "Student.h"

using namespace std;
using namespace chrono;

Student::Student() : major("") {}

Student::Student(string id, const string& email, const string& password, const string& name, const string& major)
    : User(id, email, password, name, 0) {
    this->major = major;
}

const string& Student::getMajor() const {
    return this->major;
}
void Student::setMajor(const string& major) {
    this->major = major;
}

Student::~Student() {
}

//Serialize Student object to a stream
std::ostream& operator<<(std::ostream& os, const Student& student) {
    os << static_cast<const User&>(student) << "|";
    os << student.major;
    return os;
}

//Deserialize Student object from a stream
std::istream& operator>>(std::istream& is, Student& student) {
    // Read the User base part first
    if (!(is >> static_cast<User&>(student))) {
        return is; 
    }
    string line;
    if (std::getline(is, line)) {
        string majorSegment;
        is.setstate(std::ios::failbit); 
    }
    return is;
}