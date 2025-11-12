#include <iostream>
// #include "controller/ApplicationController.h"
#include "model/User.h"

using namespace std;

int main() {
    cout << "Application Starting..." << endl;

    // // All the logic will bedefine under appController instead
    // ApplicationController appController;
    // appController.run();

    // Pass string id instead of integer 0. Passing 0 would convert to a null
    // const char* and std::string(nullptr) is undefined behavior causing a crash.
    User user("0", "jay@gmail.com", "12345", "Jay");
    cout << user.getName() << endl;

    cout << "Application Ending..." << endl;
    return 0;
}