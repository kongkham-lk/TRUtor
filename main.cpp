#include <iostream>
#include <vector>
#include <string>
#include <list>
#include <algorithm>
#include <sstream>
#include <limits>
#include <ctime>
#include <set>

// Include necessary model/service headers
#include "model/User.h"
#include "model/Student.h"
#include "model/Tutor.h"
#include "model/Course.h"
#include "model/Forum.h" // ADDED FORUM MODEL
#include "service/AuthService.h"
#include "service/CourseService.h"
#include "service/MessageService.h"
#include "service/ForumService.h" // ADDED FORUM SERVICE

using namespace std;

// --- Time Helper ---
// Converts time_t to a readable string format
string timeToString(time_t time) {
    tm* ltm = localtime(&time);
    stringstream ss;
    ss << 1900 + ltm->tm_year << "-"
        << 1 + ltm->tm_mon << "-"
        << ltm->tm_mday << " "
        << ltm->tm_hour << ":"
        << ltm->tm_min; // Simplified time format
    return ss.str();
}

// --- Message/User Lookup Helpers (From main2.cpp) ---

// Looks up a user's name based on their ID from the list of all users
string getUserNameById(const string& userId, const vector<User>& allUsers) {
    for (const auto& user : allUsers) {
        if (user.getId() == userId) {
            return user.getName();
        }
    }
    return "Unknown User";
}

// Displays the message history and allows the user to send a reply
void viewConversation(const string& currentUserId, const string& partnerId, AuthService& auth, MessageService& msgService);

// Function to handle sending a new message to a selected user
void sendMessageToUser(const string& currentUserId, AuthService& auth, MessageService& msgService) {
    vector<User> allUsers = auth.getAllUsers();
    vector<User> recipients;

    // 1. Filter out the current user and list potential recipients
    cout << "\n------------------ Select Recipient -----------------\n";
    int index = 1;
    for (const auto& user : allUsers) {
        if (user.getId() != currentUserId) {
            recipients.push_back(user);
            cout << index++ << ". " << user.getName() << " (" << user.getId() << ") ";
            cout << (user.getRole() == 0 ? "(Student)" : "(Tutor)") << endl;
        }
    }

    if (recipients.empty()) {
        cout << "No other users available to message." << endl;
        return;
    }

    int choice;
    cout << "Choose a user to message (1-" << recipients.size() << ", 0 to cancel): ";
    if (!(cin >> choice) || choice < 0 || choice > recipients.size()) {
        cout << "Invalid selection. Returning to menu." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }
    if (choice == 0) return;

    User selectedRecipient = recipients[choice - 1];

    // 2. Start conversation loop
    viewConversation(currentUserId, selectedRecipient.getId(), auth, msgService);
}

// Consolidated function to view conversation and handle continuous reply
void viewConversation(const string& currentUserId, const string& partnerId, AuthService& auth, MessageService& msgService) {
    while (true) {
        list<Message> conversation = msgService.getMessagesBetween(currentUserId, partnerId);
        vector<Message> sortedConversation(conversation.begin(), conversation.end());

        // Sort messages by timestamp
        sort(sortedConversation.begin(), sortedConversation.end(), [](const Message& a, const Message& b) {
            return a.timestamp < b.timestamp;
            });

        string partnerName = getUserNameById(partnerId, auth.getAllUsers());
        string currentUserName = getUserNameById(currentUserId, auth.getAllUsers());

        cout << "\n------------- Conversation with " << partnerName << " (" << partnerId << ") -------------\n";

        if (sortedConversation.empty()) {
            cout << "No messages in this conversation yet. Start a new one below." << endl;
        }
        else {
            for (const auto& msg : sortedConversation) {
                string senderName = (msg.senderId == currentUserId) ? currentUserName : partnerName;

                cout << "[" << timeToString(msg.timestamp) << "] ";
                cout << senderName << ": " << msg.content << endl;
            }
        }
        cout << "----------------------------------------------------------------\n";

        string replyContent;
        cout << "\nSend a message (Type !back to exit): \n";

        // Consume the rest of the line from the previous cin
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        getline(cin, replyContent);

        if (replyContent == "!back") {
            cin.clear();
            return;
        }

        if (replyContent.empty()) continue; // Skip if message is empty

        // Send the message
        time_t now = time(0);
        msgService.createMessage(currentUserId, partnerId, replyContent, now);
        cout << "\nMessage sent to " << partnerName << "." << endl;
    }
}

// Function to view existing conversations
void viewAllMessages(const string& currentUserId, AuthService& auth, MessageService& msgService) {
    // 1. Get all messages and find unique conversation partners
    list<Message> allMessages = msgService.getAllMessages();
    set<string> uniquePartnerIds;

    for (const auto& msg : allMessages) {
        if (msg.senderId == currentUserId) {
            uniquePartnerIds.insert(msg.receiverId);
        }
        else if (msg.receiverId == currentUserId) {
            uniquePartnerIds.insert(msg.senderId);
        }
    }

    // 2. Display list of active conversations
    cout << "\n--------------------- Your Conversations ----------------------\n";
    vector<string> conversationPartners(uniquePartnerIds.begin(), uniquePartnerIds.end());

    if (conversationPartners.empty()) {
        cout << "You have no active conversations. Choose 'Send New Message' to start one." << endl;
        return;
    }

    int index = 1;
    vector<User> allUsers = auth.getAllUsers();
    for (const string& partnerId : conversationPartners) {
        cout << index++ << ". Conversation with " << getUserNameById(partnerId, allUsers) << " (" << partnerId << ")" << endl;
    }

    int choice;
    cout << "Enter the number of the conversation to open (1-" << conversationPartners.size() << ", 0 to cancel): ";
    if (!(cin >> choice) || choice < 0 || choice > conversationPartners.size()) {
        cout << "Invalid selection. Returning to menu." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    if (choice == 0) return;

    string selectedPartnerId = conversationPartners[choice - 1];

    // 3. Open selected conversation
    viewConversation(currentUserId, selectedPartnerId, auth, msgService);
}

// --- Forum Functions (From main1.cpp) ---

void getForumDetail(const Forum& forum, const User& user)
{
    cout << "Forum Id: " << forum.getId() << endl;
    cout << "CreateBy: " << user.getName() << endl;
    // Note: The getCreatedAt() function in Forum likely returns a time_t/string, assuming Forum has a matching getter
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
        if (!(cin >> choice)) {
            cout << endl << "Invalid Option, Please try again!" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            choice = -1;
            continue;
        }


        if (choice == 1)
        {
            string newContent;
            cout << endl << "Please enter forum content:" << endl;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            getline(cin, newContent);
            forumService.createForum(user.getId(), newContent);
            cout << endl << "Your Forum Successfully Created..." << endl;
            printAllForums(forumService, user); // Refresh view
            choice = -1; // Keep menu open
        }
        else if (choice == 2)
        {
            vector<Forum> forums = forumService.getAllForumsByUserId(user.getId());
            if (forums.empty())
                cout << endl << "No Forum Found!" << endl;
            else
            {
                cout << endl << "Your Forums:" << endl;
                for (Forum forum : forums)
                    getForumDetail(forum, user);

                int targetForumId = -1;
                string newContent;
                cout << endl << "Enter the ID of the forum to edit: ";
                if (!(cin >> targetForumId)) {
                    cout << "Invalid ID. Returning to forum menu." << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    choice = -1;
                    continue;
                }

                cout << "Please enter new content: " << endl;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                getline(cin, newContent);

                // Note: The signature for editForum is assumed to take (userId, forumId, content)
                forumService.editForum(user.getId(), targetForumId, newContent);

                cout << endl << "Here is your updated forum detail:" << endl;
                cout << string(70, '-') << endl;
                Forum updatedForum = forumService.getForumById(targetForumId);
                getForumDetail(updatedForum, user);
            }
            choice = -1; // Keep menu open
        }
        else if (choice == 3)
            break;
        else {
            cout << endl << "Invalid Option, Please try again!" << endl;
            choice = -1;
        }
    } while (choice == -1);

    cout << endl << "Exit Forum Page..." << endl;
}

// --- Menu Functions (Modified to include Forum and MessageService) ---

void showStudentMenu(const Student& student, AuthService& auth, MessageService& msgService);
void showTutorMenu(const Tutor& tutor, AuthService& auth, MessageService& msgService);

int main() {
    cout << "\n..........Application Starting...........\\n" << endl;

    // Instantiate all services
    AuthService auth;
    CourseService courseService;
    MessageService msgService;
    // ForumService forumService; // Declared inside getForumPage where it is used

    // --- Testing User Logic (From main2.cpp) ---

    Student s1("S1", "jay@mytru.ca", "11111", "Jay", "CS");
    Tutor t1("T1", "Kevin@mytru.ca", "22222", "Kevin");

    vector<User*> users;
    users.push_back(&s1);
    users.push_back(&t1);

    for (User* user : users) {
        if (user->getRole() == 0) {
            Student* s = dynamic_cast<Student*>(user);
            if (s) {
                cout << "Student: " << s->getName() << ", Major: " << s->getMajor() << endl;
            }
        }
        else {
            Tutor* t = dynamic_cast<Tutor*>(user);
            if (t) {
                cout << "Tutor: " << t->getName() << endl;
            }
        }
    }

    // --- Testing Course Logic (from main2.cpp) ---
    Course c1("1", "CMPT 1250", "Intro to Programming");
    courseService.createCourse(c1);
    courseService.updateCourse(c1);
    courseService.getCourse("1");
    courseService.getAllCourses();

    // The main application loop (From main2.cpp)
    while (true) {
        int choice;

        if (auth.isLoggedIn()) {
            if (auth.currentRole() == 0) {
                showStudentMenu(auth.currentStudent(), auth, msgService);
            }
            else if (auth.currentRole() == 1) {
                showTutorMenu(auth.currentTutor(), auth, msgService);
            }
        }

        cout << "\n---------------------- Main Menu ----------------------\n";
        cout << "1. Login\n";
        cout << "2. Signup Student\n";
        cout << "3. Signup Tutor\n";
        cout << "4. Exit\n";
        cout << "Choose an option (1-4): ";

        if (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        string email, password, name, major;

        switch (choice) {
        case 1:
            cout << "Enter email: ";
            cin >> email;
            cout << "Enter password: ";
            cin >> password;
            if (auth.login(email, password)) {
                cout << "Login successful." << endl;
            }
            else {
                cout << "Login failed. Check email and password." << endl;
            }
            break;

        case 2:
            cout << "Enter name: ";
            cin >> name;
            cout << "Enter email: ";
            cin >> email;
            cout << "Enter password: ";
            cin >> password;
            cout << "Enter major: ";
            cin >> major;
            if (auth.signUpStudent(email, password, name, major)) {
                cout << "Student account created successfully! You are now logged in." << endl;
            }
            break;

        case 3:
            cout << "Enter name: ";
            cin >> name;
            cout << "Enter email: ";
            cin >> email;
            cout << "Enter password: ";
            cin >> password;
            if (auth.signUpTutor(email, password, name)) {
                cout << "Tutor account created successfully! You are now logged in." << endl;
            }
            break;

        case 4:
            cout << "Exiting application. Goodbye!" << endl;
            return 0;

        default:
            cout << "Invalid option." << endl;
        }
    }
}

// Student Menu Function (Modified for Forum option)
void showStudentMenu(const Student& student, AuthService& auth, MessageService& msgService) {
    while (true) {
        int choice;

        cout << "\n--------------------- Student Menu --------------------\\n";
        cout << "Welcome, " << student.getName() << "!\n";
        cout << "1. View Profile\n";
        cout << "2. View Messages\n";
        cout << "3. Send New Message\n";
        cout << "4. Tutoring Sessions\n";
        cout << "5. Forum\n"; // ADDED FORUM
        cout << "6. Logout\n"; // RE-INDEXED
        cout << "Choose an option (1-6): ";

        if (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
        case 1:
            cout << "\n------------------- Student Profile -------------------\\n";
            cout << "ID: " << student.getId() << "\n";
            cout << "Name: " << student.getName() << "\n";
            cout << "Email: " << student.getEmail() << "\n";
            cout << "Major: " << student.getMajor() << "\n";
            break;

        case 2:
            viewAllMessages(student.getId(), auth, msgService);
            break;

        case 3:
            sendMessageToUser(student.getId(), auth, msgService);
            break;

        case 4:
            cout << "\n------------------- View Tutoring Sessions -------------------\\n";
            cout << "\n(No session viewing implemented yet)\\n";
            break;

        case 5: // NEW FORUM OPTION
            getForumPage(student);
            break;

        case 6: // LOGOUT
            auth.logout();
            cout << "Logged out successfully." << endl;
            return;

        default:
            cout << "Invalid option.\n";
        }
    }
}

// Tutor Menu Function (Modified for Forum option)
void showTutorMenu(const Tutor& tutor, AuthService& auth, MessageService& msgService) {
    while (true) {
        int choice;

        cout << "\n---------------------- Tutor Menu ---------------------\\n";
        cout << "Welcome, " << tutor.getName() << "!\n";
        cout << "1. View Profile\n";
        cout << "2. View Messages\n";
        cout << "3. Send New Message\n";
        cout << "4. Tutoring Sessions\n";
        cout << "5. Forum\n"; // ADDED FORUM
        cout << "6. Logout\n"; // RE-INDEXED
        cout << "Choose an option (1-6): ";

        if (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
        case 1:
            cout << "\n------------------- Tutor Profile -------------------\\n";
            cout << "ID: " << tutor.getId() << "\n";
            cout << "Name: " << tutor.getName() << "\n";
            cout << "Email: " << tutor.getEmail() << "\n";
            break;

        case 2:
            viewAllMessages(tutor.getId(), auth, msgService);
            break;

        case 3:
            sendMessageToUser(tutor.getId(), auth, msgService);
            break;

        case 4:
            cout << "\n------------------- Manage Tutoring Sessions -------------------\\n";
            cout << "\n(No session management implemented yet)\\n";
            break;

        case 5: // NEW FORUM OPTION
            getForumPage(tutor);
            break;

        case 6: // LOGOUT
            auth.logout();
            cout << "Logged out successfully." << endl;
            return;

        default:
            cout << "Invalid option.\n";
        }
    }
}