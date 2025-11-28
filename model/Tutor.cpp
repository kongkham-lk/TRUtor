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

//Serialize Tutor object to a stream
std::ostream& operator<<(std::ostream& os, const Tutor& tutor) {
    os << static_cast<const User&>(tutor) << "|";
    os << "N/A"; 
    return os;
}