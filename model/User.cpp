#include <iostream>
#include "User.h"
// #include "../uuid_v4/uuid_v4.h"
// #include "../uuid_v4/endianness.h"

using namespace std;
using namespace chrono;
// using namespace UUIDv4;

User::User(const string& id, const string& email, const string& password, const string& name, int role)
    : id(id), email(email), password(password), name(name), role(role), createdAt(system_clock::to_time_t(system_clock::now())) {
}

const string& User::getId() const {
    return this->id;
}

const string& User::getEmail() const {
    return this->email;
}
void User::setEmail(const string& email) {
    this->email = email;
}

const string& User::getPassword() const {
    return this->password;
}
void User::setPassword(const string& password) {
    this->password = password;
}

const string& User::getName() const {
    return this->name;
}
void User::setName(const string& name) {
    this->name = name;
}

time_t User::getCreatedAt() const {
    return this->createdAt;
}

int User::getRole() const {
    return this->role;
}

const set<string> User::getSubjects() const {
    return this->subjects;
}
void User::addSubject(const string& subject) {
    this->subjects.insert(subject);
}
void User::removeSubject(const string& subjectToRemove) {
    subjects.erase(subjectToRemove);
}

User::~User() {
}