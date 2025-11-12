#ifndef TUTOR_H
#define TUTOR_H

#include <string>
#include "User.h"

using namespace std;
using namespace chrono;

// This class is used for DB mapping as well
class Tutor : public User { // extend User class
public:
    Tutor(string id, const string& email, const string& password, const string& name); // Constructor, invoke when signup

    ~Tutor(); // Destructor
};

#endif // TUTOR_H
