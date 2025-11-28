#ifndef TUTOR_H
#define TUTOR_H

#include <string>
#include <sstream>
#include <fstream>
#include "User.h"

using namespace std;
using namespace chrono;

class Tutor : public User { 
public:
    Tutor(); 
    Tutor(string id, const string& email, const string& password, const string& name); 

    //Stream operator 
    friend std::ostream& operator<<(std::ostream& os, const Tutor& tutor);

    ~Tutor();
};

#endif // TUTOR_H