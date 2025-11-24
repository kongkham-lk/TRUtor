#include <iostream>
#include <sstream>
#include <algorithm>
#include <fstream>
#include "User.h"

using namespace std;
using namespace chrono;

User::User()
    : id(""), email(""), password(""), name(""), role(-1), createdAt(system_clock::now()) {
}

User::User(string id, const string& email, const string& password, const string& name, int role)
    : id(id), email(email), password(password), name(name), role(role), createdAt(system_clock::now()) {
}

// ... (Other getters and setters remain the same) ...
const string& User::getId() const { return this->id; }
const string& User::getEmail() const { return this->email; }
void User::setEmail(const string& email) { this->email = email; }
const string& User::getPassword() const { return this->password; }
void User::setPassword(const string& password) { this->password = password; }
const string& User::getName() const { return this->name; }
void User::setName(const string& name) { this->name = name; }
system_clock::time_point User::getCreatedAt() const { return this->createdAt; }
int User::getRole() const { return this->role; }
const set<string> User::getSubjects() const { return this->subjects; }
void User::addSubject(const string& subject) { this->subjects.insert(subject); }
void User::removeSubject(const string& subject) { this->subjects.erase(subject); }
User::~User() {}

// Serialize User object to a stream
std::ostream& operator<<(std::ostream& os, const User& user) {
    // ID|Email|Password|Name|Role|CreatedAt(time_t)|Subjects(comma-separated)

    // 1. Convert time_point to time_t
    time_t timeStamp = system_clock::to_time_t(user.createdAt);

    os << user.id << "|"
        << user.email << "|"
        << user.password << "|"
        << user.name << "|"
        << user.role << "|"
        << timeStamp << "|";

    // 2. Serialize subjects set
    bool first = true;
    for (const string& subject : user.subjects) {
        if (!first) os << ",";
        os << subject;
        first = false;
    }
    return os;
}

// Deserialize User object from a stream
std::istream& operator>>(std::istream& is, User& user) {
    string line;
    if (std::getline(is, line)) {
        stringstream ss(line);
        string segment;
        vector<string> parts;

        // Simple split logic based on the '|' delimiter
        size_t start = 0;
        size_t end = line.find('|');
        while (end != string::npos) {
            parts.push_back(line.substr(start, end - start));
            start = end + 1;
            end = line.find('|', start);
        }
        parts.push_back(line.substr(start)); // The last segment

        if (parts.size() >= 6) {
            user.id = parts[0];
            user.email = parts[1];
            user.password = parts[2];
            user.name = parts[3];
            try {
                user.role = stoi(parts[4]);

                // 5. Deserialize time_t to time_point
                time_t timeStamp = stoll(parts[5]);
                user.createdAt = system_clock::from_time_t(timeStamp);

                // 6. Deserialize subjects (parts.size() == 7 for Student/Tutor + Major)
                if (parts.size() >= 7) {
                    string subjectsString = parts.back(); // Last part is always subjects
                    user.subjects.clear();
                    stringstream subject_ss(subjectsString);
                    string subject;
                    while (getline(subject_ss, subject, ',')) {
                        if (!subject.empty()) {
                            user.subjects.insert(subject);
                        }
                    }
                }
            }
            catch (const std::exception& e) {
                is.setstate(std::ios::failbit);
                cerr << "Error parsing User fields: " << e.what() << endl;
            }
        }
        else {
            is.setstate(std::ios::failbit); // Indicate failure if line format is wrong
        }
    }
    return is;
}