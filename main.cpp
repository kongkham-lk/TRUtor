#include <iostream>
#include <vector>
#include <string>
#include "model/User.h"
#include "model/Student.h"
#include "model/Tutor.h"
#include "service/AuthService.h"
#include "model/Course.h"
#include "service/CourseService.h"

using namespace std;

void showStudentMenu(const Student& student, AuthService& auth); //function to show student menu
void showTutorMenu(const Tutor& tutor, AuthService& auth); //function to show tutor menu

int main() {
    cout << "\n..........Application Starting...........\n" << endl;
    cout << "Testing User, Student, Tutor Classes:\n" << endl;
    // --- Testing User Logic ---
    
    Student s1("0", "jay@mytru.ca", "11111", "Jay", "CS");
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

     AuthService auth; // Authentication service instance

     cout << "\n------------------- Welcome to TRUtor -------------------\n\n";

    // Auto-login check
    if (auth.isLoggedIn()) {
        cout << "Auto-logged in!\n";

        if (auth.currentRole() == 0) { //check if the logged in user is student
            Student student = auth.currentStudent();
            cout << "Welcome back Student: " << student.getName()
                 << " (" << student.getMajor() << ")\n";
            showStudentMenu(student, auth); //call function to show student menu
        } else {
            Tutor tutor = auth.currentTutor();
            cout << "Welcome back Tutor: " << tutor.getName() << "\n";
            showTutorMenu(tutor, auth); //call function to show tutor menu
        }
    }

    // -------------------------------
    // Authentication Loop
    // -------------------------------
    while (true) {
        int option;
        string email, password, name, major;

        cout << "\n---------------------------- Authentication Menu ---------------------------\n";
        cout << "1. Sign Up (Student)\n";
        cout << "2. Sign Up (Tutor)\n";
        cout << "3. (DEV) Test Course / CourseService\n";
        cout << "4. Login\n";
        cout << "5. Exit\n";
        cout << "Choose one of the above options (1-5) : ";
        cin >> option;

        switch (option) {
        case 1: {
            cout << "\n-------------------- Student Sign Up --------------------\n";
            cout << "Enter Email: ";
            cin >> email;
            cout << "Enter Password: ";
            cin >> password;
            cout << "Full Name (mixed case with no spaces): ";
            cin >> name;
            cout << "Enter your major: ";
            cin >> major;

            if (auth.signUpStudent(email, password, name, major))
                cout << "Student account created!\n";
            else
                cout << "Error: Email already exists.\n";

            break;
        }

        case 2: {
            cout << "\n-------------------- Tutor Sign Up --------------------\n";
            cout << "Enter Email: ";
            cin >> email;
            cout << "Enter Password: ";
            cin >> password;
            cout << "Full Name (mixed case with no spaces): ";
            cin >> name;

            if (auth.signUpTutor(email, password, name))
                cout << "Tutor account created!\n";
            else
                cout << "\n\nERROR : Email already exists.\n\n";

            break;
        }

        case 3: {
            // ---------- Course / CourseService Tests ----------
            cout << "\n--- DEV: Running CourseService tests ---\n";

            CourseService cs;

            // Test createCourse
            Course c1("C101", "Intro to C++", "Basics of C++ programming.");
            cs.createCourse(c1);

            // Test getCourse
            Course fetched = cs.getCourse("C101");
            cout << "Fetched course: ID=" << fetched.getId()
                << ", Title=\"" << fetched.getTitle()
                << "\", Desc=\"" << fetched.getDescription() << "\"\n";

            // Test updateCourse
            fetched.setTitle("Intro to Modern C++");
            fetched.setDescription("Updated description: modern features and best practices.");
            cs.updateCourse(fetched);

            // Test getAllCourses
            vector<Course> all = cs.getAllCourses();
            cout << "\nAll courses returned by getAllCourses():\n";
            for (const auto& course : all) {
                cout << " - ID=" << course.getId()
                    << ", Title=\"" << course.getTitle() << "\"\n";
            }

            // Test deleteCourse
            cs.deleteCourse("C101");
            cout << "--- DEV: CourseService tests finished ---\n\n";

            break;
        }

        case 4: {
            cout << "\n-------------------- Login --------------------\n";
            cout << "Enter Email: ";
            cin >> email;
            cout << "Enter Password: ";
            cin >> password;

            if (!auth.login(email, password)) {
                cout << "\n\nERROR : Invalid email or password.\n\n";
                break;
            }

            cout << "Login successful!\n\n";

            if (auth.currentRole() == 0) {
                Student student = auth.currentStudent();
                showStudentMenu(student, auth);
            }
            else {
                Tutor tutor = auth.currentTutor();
                showTutorMenu(tutor, auth);
            }

            break;
        }

        case 5:
            cout << "Exiting TRUtor system.\n";
            return 0;

        default:
            cout << "Invalid option.\n";
        }
    }

    cout << "\n..........Application Ending...........\n" << endl;
    return 0;
}

//function to show student menu, takes student object and auth service reference as parameters
void showStudentMenu(const Student& student, AuthService& auth) {
    while (true) {
        int choice;

        cout << "\n------------------------- Student Menu -------------------------\n";
        cout << "Welcome, " << student.getName() << "!\n";
        cout << "1. View Profile\n";
        cout << "2. Messages\n";
        cout << "3. Tutoring Sessions\n";
        cout << "4. Logout\n";
        cout << "Choose an option (1-4): ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "\n------------------- Student Profile -------------------\n";
                cout << "ID: " << student.getId() << "\n";
                cout << "Name: " << student.getName() << "\n";
                cout << "Email: " << student.getEmail() << "\n";
                cout << "Major: " << student.getMajor() << "\n";
                break;

                // Inside showStudentMenu or when testing student messages
            case 2:  // Messages
                cout << "\n---------------------Your Messages---------------------\n";

                // Your MessageService test
                MessageService msg;
                time_t t = time(0);
                msg.createMessage(1, 2, "Hello", t);
                msg.createMessage(2, 1, "Hi", t);

                auto msgs = msg.getMessagesBetween(1, 2);
                for (const auto& m : msgs)
                    cout << m.messageId << ": " << m.content << endl;

                int id = msg.fetchMessageIdFromDb(1, 2, t);
                cout << "Fetched message ID: " << id << endl;

                msg.deleteMessagesBetween(1, 2);
                cout << "Deleted messages between 1 and 2." << endl;

                break;

            case 3:
                cout << "\n------------------- Tutoring Sessions -------------------\n";
                cout << "\nNo tutoring sessions implemented yet\n"; // will be implemented later
                break;

            case 4:
                auth.logout();
                cout << "Logged out successfully.\n";
                return; // return to main menu

            default:
                cout << "Invalid option.\n";
        }
    }
}

void showTutorMenu(const Tutor& tutor, AuthService& auth) {
    while (true) {
        int choice;

        cout << "\n---------------------- Tutor Menu ---------------------\n";
        cout << "Welcome, " << tutor.getName() << "!\n";
        cout << "1. View Profile\n";
        cout << "2. Messages\n";
        cout << "3. Tutoring Sessions\n";
        cout << "4. Logout\n";
        cout << "Choose an option (1-4): ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "\n------------------- Tutor Profile -------------------\n";
                cout << "ID: " << tutor.getId() << "\n";
                cout << "Name: " << tutor.getName() << "\n";
                cout << "Email: " << tutor.getEmail() << "\n";
                break;

            case 2:
                cout << "\n---------------------Your Messages---------------------\n";
                cout << "\n(No messages implemented yet)\n";
                break;

            case 3:
                cout << "\n------------------- Mannage Tutoring Sessions -------------------\n";
                cout << "\n(No session management implemented yet)\n";
                break;

            case 4:
                auth.logout();
                cout << "Logged out successfully.\n";
                return; // return to main menu

            default:
                cout << "Invalid option.\n";
        }
    }
}