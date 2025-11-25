#include <iostream>
#include <vector>
#include <string>
#include "model/User.h"
#include "model/Student.h"
#include "model/Tutor.h"
#include "model/Course.h"
#include "service/AuthService.h"
#include "service/CourseService.h"
#include "service/messageService.h"
#include "service/ForumService.h"

using namespace std;

bool getLoginHomePage(AuthService auth);
bool showUserMenu(const User& user, AuthService& auth);

int main() {
    cout << endl << "Application Starting ..." << endl;
    cout << endl << string(30, '=') << " Welcome to TRUtor " << string(30, '=') << endl;

    AuthService auth; // Authentication service instance
    bool running = true;
    // Auto-login check
    if (auth.isLoggedIn()) {
        cout << "Auto-logged in!" << endl;
        running = getLoginHomePage(auth);
    }

    // -------------------------------
    // Authentication Loop
    // -------------------------------
    while (running) {
        int option;
        string email, password, name, major;

        cout << endl << string(29, '-') << " Authentication Menu " << string(29, '-') << endl;
        cout << "1. Sign Up (Student)" << endl;
        cout << "2. Sign Up (Tutor)" << endl;
        cout << "3. Login" << endl;
        cout << "4. Exit" << endl;
        cout << "Choose one of the above options (1-4) : ";
        cin >> option;

        switch (option) {
        case 1: {
            cout << endl << string(29, '-') << " Student Sign Up " << string(29, '-') << endl;
            cout << "Enter Email: ";
            cin >> email;
            cout << "Enter Password: ";
            cin >> password;
            cout << "Full Name (mixed case with no spaces): ";
            cin >> name;
            cout << "Enter your major: ";
            cin >> major;

            if (auth.signUpStudent(email, password, name, major))
                cout << "Student account created!" << endl;
            else
                cout << "Error: Email already exists." << endl;

            break;
        }
        case 2: {
            cout << endl << string(30, '-') << " Tutor Sign Up " << string(30, '-') << endl;
            cout << "Enter Email: ";
            cin >> email;
            cout << "Enter Password: ";
            cin >> password;
            cout << "Full Name (mixed case with no spaces): ";
            cin >> name;

            if (auth.signUpTutor(email, password, name))
                cout << "Tutor account created!" << endl;
            else
                cout << endl <<"ERROR : Email already exists." << endl;

            break;
        }
        case 3: {
            cout << endl << string(36, '-')<< " Login " << string(36, '-') << endl;
            cout << "Enter Email: ";
            cin >> email;
            cout << "Enter Password: ";
            cin >> password;

            if (!auth.login(email, password)) {
                cout << endl << "ERROR : Invalid email or password." << endl;
                break;
            }

            cout << endl << "Login successful!" << endl;
            running = getLoginHomePage(auth);
            break;
        }
        case 4:
            running = false;
            break;
        default:
            cout << "Invalid option." << endl;
        }
    }

    cout << endl << "Application Ending ..." << endl;
    return 0;
}

bool getLoginHomePage(AuthService auth)
{
    if (auth.currentRole() == 0) { //check if the logged in user is student
        Student student = auth.currentStudent();
        cout << "Welcome back Student: " << student.getName()
             << " (" << student.getMajor() << ")" << endl;
        return showUserMenu(student, auth); //call function to show student menu
    } else {
        Tutor tutor = auth.currentTutor();
        cout << "Welcome back Tutor: " << tutor.getName() << endl;
        return showUserMenu(tutor, auth); //call function to show tutor menu
    }

}

int getChoiceFromUserMenu(const User& user)
{
    int choice;
    string headerType = dynamic_cast<const Student*>(&user) ? "Student" : "Tutor";

    cout << endl << string(34, '-') << " " + headerType + " Menu " << string(34, '-') << endl;
    cout << "Welcome, " << user.getName() << "!" << endl;
    cout << "1. View Profile" << endl;
    cout << "2. Messages" << endl;
    cout << "3. Tutoring Sessions" << endl;
    cout << "4. Forum" << endl;
    cout << "5. Logout" << endl;
    cout << "6. Exit" << endl;
    cout << "Choose an option (1-6): ";
    cin >> choice;

    return choice;
}

void getUserProfilePage(const User& user)
{
    cout << endl << string(33, '-') << " Profile Detail " << string(33, '-') << endl;
    cout << "ID: " << user.getId() << endl;
    cout << "Name: " << user.getName() << endl;
    cout << "Email: " << user.getEmail() << endl;

    const Student* student = dynamic_cast<const Student*>(&user);
    if (student)
        cout << "Major: " << student->getMajor() << endl;
}

void getUserMessagesPage(const User& user)
{
    cout << endl << string(33, '-') << " Your Messages " << string(33, '-') << endl;

    // MessageService msg;
    // time_t t = time(0);
    // msg.createMessage(1, 2, "Hello", t);
    // msg.createMessage(2, 1, "Hi", t);
    //
    // auto msgs = msg.getMessagesBetween(1, 2);
    // for (const auto& m : msgs)
    //     cout << m.messageId << ": " << m.content << endl;
    //
    // int id = msg.fetchMessageIdFromDb(1, 2, t);
    // cout << "Fetched message ID: " << id << endl;
    //
    // msg.deleteMessagesBetween(1, 2);
    // cout << "Deleted messages between 1 and 2." << endl;
}

void getTutoringSessionPage(const User& user)
{
    cout << endl << string(32, '-') << " Tutoring Sessions " << string(32, '-') << endl;
    // cout << "\nNo tutoring sessions implemented yet" << endl; // will be implemented later
}

void getForumDetail(const Forum& forum, const User& user)
{
    cout << "Forum Id: " << forum.getId() << endl;
    cout << "CreateBy: " << user.getName() << endl;
    cout << "CreateAt: " << forum.getCreatedAt() << endl;
    cout << "Content: " << forum.getContent() << endl;
    cout << string(70, '-') << endl;
}

void printAllForums(const ForumService& forumService, const User& user)
{
    vector<Forum> forums = forumService.getAllForums();
    if (forums.empty())
        cout << "No Forum Found!" << endl;
    else
    {
        cout << "All Forums:" << endl;
        cout << string(70, '-') << endl;
        for (const Forum& forum : forums)
            getForumDetail(forum, user);
    }
}

void getForumPage(const User& user)
{
    cout << endl << string(32, '-') << " Forum Dashboard " << string(32, '-') << endl;

    ForumService forumService;
    printAllForums(forumService, user);

    int choice = -1;
    do
    {
        cout << endl << "Choose an option to proceed:" << endl;
        cout << "1. Create a new Forum" << endl;
        cout << "2. Manage your forums" << endl;
        cout << "3. Back to main page" << endl;
        cout << "Please Enter: ";
        cin >> choice;

        if (choice > 0 && choice < 3)
        {
            if (choice == 1)
            {
                string newContent;
                cout << endl << "Please enter forum content:" << endl;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                getline(cin, newContent);
                forumService.createForum(user.getId(), newContent);
                cout << endl << "Your Forum Successfully Created..." << endl;
            }
            else if (choice == 2)
            {
                vector<Forum> forums = forumService.getAllForumsByUserId(user.getId());
                if (forums.empty())
                    cout << endl << "No Forum Found!" << endl;
                else
                {
                    cout << endl << "All Forums:" << endl;
                    for (Forum forum : forums)
                        getForumDetail(forum, user);

                    int targetForumId = -1;
                    string newContent;
                    cout << endl << "Target forum Id: ";
                    cin >> targetForumId;
                    cout << "Please enter new content: " << endl;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    getline(cin, newContent);
                    forumService.editForum(user.getId(), targetForumId, newContent);

                    cout << endl << "Here is your udpated forum detail:" << endl;
                    cout << string(70, '-') << endl;
                    Forum updatedForum = forumService.getForumById(targetForumId);
                    getForumDetail(updatedForum, user);
                }
            }
        }
        else if (choice == 3)
            break;
        else
            cout << endl << "Invalid Option, Please try again!" << endl;
        choice = -1;
    } while (choice == -1);

    cout << endl << "Exit Forum Page..." << endl;
}

bool showUserMenu(const User& user, AuthService& auth) {
    bool isLoggedIn = true;
    while (isLoggedIn) {
        int choice = getChoiceFromUserMenu(user);

        switch (choice) {
            case 1:
                getUserProfilePage(user);
                break;
            case 2:  // Messages
                getUserMessagesPage(user);
                break;  // remember the break inside the block
            case 3:
                getTutoringSessionPage(user);
                break;
            case 4:
                getForumPage(user);
                break;
            case 5:
                auth.logout();
                cout << "Logged out successfully." << endl;
                isLoggedIn = false;
                break; // return to main menu
            case 6:
                return false; // return to main menu
            default:
                cout << "Invalid option." << endl;
                break;
        }
    }
    return true;
}