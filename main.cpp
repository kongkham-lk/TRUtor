#include <iostream>
#include "model/User.h"
#include "model/Student.h"
#include "model/Tutor.h"

using namespace std;

int main() {
    cout << "\nApplication Starting...\n" << endl;

    // --- Testing User Logic ---
    
    Student s1("0", "jay@mytru.ca", "11111", "Jay");
    Tutor t1("1", "Kevin@mytru.ca", "22222", "Kevin");
    
    // Use pointers to avoid object slicing when storing derived classes in base container
    vector<User*> users;
    users.push_back(&s1);
    users.push_back(&t1);
    
    for (User* user : users) {
        if (user->getRole() == 0) {
            Student* s = dynamic_cast<Student*>(user);
            if (s) {
                cout << "This user is Student." << endl;
            } else {
                cout << "Downcast failed\n";
            }
        } else if (user->getRole() == 1) {
            Tutor* t = dynamic_cast<Tutor*>(user); 
            if (t) {
                cout << "This user is Tutor." << endl;
            } else {
                cout << "Downcast failed\n";
            }
        }
    }

    cout << "\nApplication Ending...\n" << endl;
    return 0;
}