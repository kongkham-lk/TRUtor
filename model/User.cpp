#include <iostream>
#include "User.h"
// #include "../uuid_v4/uuid_v4.h"
// #include "../uuid_v4/endianness.h"

using namespace std;
using namespace chrono;
// using namespace UUIDv4;

User::User(string id, const string& email, const string& password, const string& name)
    : id(id), email(email), password(password), name(name), createdAt(system_clock::now()) {
}

const string& User::getId() const {
    return this->id;
}

const string& User::getEmail() const {
    return this->email;
}

const string& User::getPassword() const {
    return this->password;
}

const string& User::getName() const {
    return this->name;
}

system_clock::time_point User::getCreatedAt() const {
    return this->createdAt;
}

void User::setEmail(const string& email) {
    this->email = email;
}

void User::setPassword(const string& password) {
    this->password = password;
}

void User::setName(const string& name) {
    this->name = name;
}

User::~User() {
}