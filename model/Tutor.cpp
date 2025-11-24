#include <iostream>
#include <sstream>
#include <fstream>
#include "Tutor.h"

using namespace std;
using namespace chrono;

Tutor::Tutor() {}

Tutor::Tutor(string id, const string& email, const string& password, const string& name)
    : User(id, email, password, name, 1) {
}

Tutor::~Tutor() {
}

// Serialize Tutor object to a stream
std::ostream& operator<<(std::ostream& os, const Tutor& tutor) {
    // Tutor data is User data + a placeholder major field for file format consistency
    os << static_cast<const User&>(tutor) << "|";
    os << "N/A"; // Placeholder for 'major' field to match Student format in file
    return os;
}