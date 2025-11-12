#include <iostream>
#include "Tutor.h"

using namespace std;
using namespace chrono;

Tutor::Tutor(string id, const string& email, const string& password, const string& name)
    : User(id, email, password, name, 1) {
}

Tutor::~Tutor() {
}