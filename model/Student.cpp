#include <iostream>
#include "Student.h"

using namespace std;
using namespace chrono;

Student::Student(string id, const string& email, const string& password, const string& name)
    : User(id, email, password, name, 0) {
}

const string& Student::getMajor() const {
    return this->major;
}
void Student::setMajor(const string& major) {
    this->major = major;
}

Student::~Student() {
}