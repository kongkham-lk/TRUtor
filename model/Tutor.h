#ifndef TUTOR_H
#define TUTOR_H

#include <string>
#include <sstream>
#include <fstream>
#include "User.h"

using namespace std;
using namespace chrono;

// This class is used for DB mapping as well
class Tutor : public User { // extend User class
public:
    Tutor(); // Default constructor for file loading
    Tutor(string id, const string& email, const string& password, const string& name); // Constructor, invoke when signup

    // Stream operator (only output needed, input handled by AuthService)
    friend std::ostream& operator<<(std::ostream& os, const Tutor& tutor);

    ~Tutor(); // Destructor
};

#endif // TUTOR_H