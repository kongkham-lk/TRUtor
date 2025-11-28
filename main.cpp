#include <iostream>
#include <vector>
#include <string>
#include <list>
#include <algorithm>
#include <sstream>
#include <limits>
#include <ctime>
#include <set>

#include "model/User.h"
#include "model/Student.h"
#include "model/Tutor.h"
#include "model/Course.h"
#include "service/AuthService.h"
#include "service/CourseService.h"
#include "service/MessageService.h" // Includes MessageService

using namespace std;

// --- Helper Functions (From second file) ---

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
    cout << "Choose a user to message (1-" << recipients.size() << "): ";
    if (!(cin >> choice) || choice < 1 || choice > recipients.size()) {
        cout << "Invalid selection. Returning to menu." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    User selectedRecipient = recipients[choice - 1];

    // 2. Start conversation loop
    viewConversation(currentUserId, selectedRecipient.getId(), auth, msgService);
}

// Consolidated function to view conversation and handle continuous reply
void viewConversation(const string& currentUserId, const string& partnerId, AuthService& auth, MessageService& msgService) {
    while (true) {
        list<Message> conversation = msgService.getMessagesBetween(currentUserId, partnerId);

        // Convert list to vector for sorting
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
                // Determine which name to show (current user or partner)
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
            cin.clear(); // Clear any state that may have been set
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
        // Optionally add a path to send a new message here, but the main menu handles it.
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

// --- Forward Declarations and Service Functions (Combined and Forum removed) ---

// Forward declarations using the new structure
bool getLoginHomePage(AuthService& auth, MessageService& msgService); // Added msgService
bool showUserMenu(const User& user, AuthService& auth, MessageService& msgService); // Added msgService
void showStudentMenu(const Student& student, AuthService& auth, MessageService& msgService);
void showTutorMenu(const Tutor& tutor, AuthService& auth, MessageService& msgService);

// --- User Menu Display (Modified from first file) ---

int getChoiceFromUserMenu(const User& user)
{
    int choice;
    string headerType = dynamic_cast<const Student*>(&user) ? "Student" : "Tutor";

    cout << endl << string(34, '-') << " " + headerType + " Menu " << string(34, '-') << endl;
    cout << "Welcome, " << user.getName() << "!" << endl;
    cout << "1. View Profile" << endl;
    cout << "2. View Messages" << endl;       // Option changed
    cout << "3. Send New Message" << endl;    // New option
    cout << "4. Tutoring Sessions" << endl;   // Index moved from 3 to 4
    cout << "5. Logout" << endl;
    cout << "6. Exit" << endl;
    cout << "Choose an option (1-6): ";
    cin >> choice;

    return choice;
}

// --- Page Implementations (Modified from first file) ---

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

void getTutoringSessionPage(const User& user)
{
    cout << endl << string(32, '-') << " Tutoring Sessions " << string(32, '-') << endl;
    cout << "\n(No tutoring sessions implemented yet)" << endl;
}

// --- Main User Menu Logic (Modified from first file) ---

bool showUserMenu(const User& user, AuthService& auth, MessageService& msgService) {
    bool isLoggedIn = true;
    while (isLoggedIn) {
        int choice = getChoiceFromUserMenu(user);

        if (cin.fail()) {
            cout << "Invalid input. Please enter a number." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        string currentUserId = user.getId();

        switch (choice) {
        case 1:
            getUserProfilePage(user);
            break;
        case 2: // View Messages (Active Conversations)
            viewAllMessages(currentUserId, auth, msgService);
            break;
        case 3: // Send New Message
            sendMessageToUser(currentUserId, auth, msgService);
            break;
        case 4: // Tutoring Sessions
            getTutoringSessionPage(user);
            break;
        case 5: // Logout
            auth.logout();
            cout << "Logged out successfully." << endl;
            isLoggedIn = false;
            break; // return to main menu
        case 6: // Exit Application
            return false;
        default:
            cout << "Invalid option." << endl;
            break;
        }
    }
    return true; // continue running main app loop
}

// --- Entry Point after Login (Modified from first file) ---

bool getLoginHomePage(AuthService& auth, MessageService& msgService)
{
    if (auth.currentRole() == 0) { // Student
        Student student = auth.currentStudent();
        cout << "Welcome back Student: " << student.getName()
            << " (" << student.getMajor() << ")" << endl;
        return showUserMenu(student, auth, msgService);
    }
    else { // Tutor
        Tutor tutor = auth.currentTutor();
        cout << "Welcome back Tutor: " << tutor.getName() << endl;
        return showUserMenu(tutor, auth, msgService);
    }
}

// --- Main Function (Combined from both files, prioritizing first file's structure) ---

int main() {
    cout << endl << "Application Starting ..." << endl;
    cout << endl << string(30, '=') << " Welcome to TRUtor " << string(30, '=') << endl;

    // Instantiate all services
    AuthService auth;
    CourseService courseService; // Kept to satisfy Course.h include
    MessageService msgService; // New: Instantiate the message service

    bool running = true;

    // --- Initial Test/Setup Logic from second file (Kept for consistency) ---
    Student s1("S1", "jay@mytru.ca", "11111", "Jay", "CS");
    Tutor t1("T1", "Kevin@mytru.ca", "22222", "Kevin");
    Course c1("1", "CMPT 1250", "Intro to Programming");

    // NOTE: The user vector logic from the second file is removed as it was not part of the main application flow
    // NOTE: The main() from the second file contained duplicated logic that is best handled by the first file's structure

    // Auto-login check (from first file)
    if (auth.isLoggedIn()) {
        cout << "Auto-logged in!" << endl;
        running = getLoginHomePage(auth, msgService);
    }

    // -------------------------------
    // Authentication Loop (from first file)
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

        // Input handling for robustness
        if (!(cin >> option)) {
            cout << "Invalid input. Please enter a number." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (option) {
        case 1: {
            cout << endl << string(29, '-') << " Student Sign Up " << string(29, '-') << endl;
            cout << "Enter Email: ";
            cin >> email;
            cout << "Enter password: ";
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
            cout << "Enter password: ";
            cin >> password;
            cout << "Full Name (mixed case with no spaces): ";
            cin >> name;

            if (auth.signUpTutor(email, password, name))
                cout << "Tutor account created!" << endl;
            else
                cout << endl << "ERROR : Email already exists." << endl;

            break;
        }
        case 3: {
            cout << endl << string(36, '-') << " Login " << string(36, '-') << endl;
            cout << "Enter Email: ";
            cin >> email;
            cout << "Enter password: ";
            cin >> password;

            if (!auth.login(email, password)) {
                cout << endl << "ERROR : Invalid email or password." << endl;
                break;
            }

            cout << endl << "Login successful!" << endl;
            running = getLoginHomePage(auth, msgService);
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