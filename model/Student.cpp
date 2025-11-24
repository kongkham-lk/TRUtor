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

// Serialize Student object to a stream
std::ostream& operator<<(std::ostream& os, const Student& student) {
    // Student data is User data + major field
    os << static_cast<const User&>(student) << "|"; // Call base class operator and add delimiter
    os << student.major;
    return os;
}

// Deserialize Student object from a stream
std::istream& operator>>(std::istream& is, Student& student) {
    // Read the User base part first
    if (!(is >> static_cast<User&>(student))) {
        return is; // Error in reading base class
    }

    // Now process the Student-specific data (Major)
    string line;
    if (std::getline(is, line)) {
        // The User operator >> reads a whole line, so we need to re-read the last part of the line
        // We'll rely on AuthService to handle the full user/student distinction when loading from file.
        // For simplicity, we'll implement a custom read specific to AuthService structure:

        string majorSegment;

        // This relies on the previous User>> operator leaving the istream in a good state 
        // after reading the last segment of the original line which contains the subjects.
        // Since the User>> operator consumes the entire line, we cannot chain them easily.
        // Reverting to the simpler read logic in the AuthService which is better suited for the storage format.

        // **I will rely on AuthService to parse the line and reconstruct Student/Tutor correctly.**
        // **The custom stream operators are better suited for Course which has no inheritance.**
        // For Student, I'll keep the custom operator simple, only allowing output:

        // The original logic for `Student` and `Tutor` was simple inheritance. 
        // I'll keep the stream operators simple and rely on `AuthService` to handle the file format directly for the user hierarchy.

        // *Self-Correction: Stream operator for User with inheritance is tricky with custom delimiters. Reverting to basic User/Student constructors and direct `AuthService` file parsing for User objects to simplify the persistence layer, as the original `AuthService.cpp` already did this.*

        is.setstate(std::ios::failbit); // Indicate this operator is not intended for use
    }
    return is;
}