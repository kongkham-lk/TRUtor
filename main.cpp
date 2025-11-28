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
#include "model/Forum.h" 
#include "service/AuthService.h"
#include "service/CourseService.h"
#include "service/MessageService.h"
#include "service/ForumService.h" 
#include "service/FeedbackService.h"

using namespace std;

//Time function
string timeToString(time_t time) {
    tm* ltm = localtime(&time);
    stringstream ss;
    ss << 1900 + ltm->tm_year << "-"
        << 1 + ltm->tm_mon << "-"
        << ltm->tm_mday << " "
        << ltm->tm_hour << ":"
        << ltm->tm_min; 
    return ss.str();
}


//Looks up a user's name based on their ID from the list of all users
string getUserNameById(const string& userId, const vector<User>& allUsers) {
    for (const auto& user : allUsers) {
        if (user.getId() == userId) {
            return user.getName();
        }
    }
    return "Unknown User";
}

void viewConversation(const string& currentUserId, const string& partnerId, AuthService& auth, MessageService& msgService);

//Function to handle sending a new message to a selected user
void sendMessageToUser(const string& currentUserId, AuthService& auth, MessageService& msgService) {
    vector<User> allUsers = auth.getAllUsers();
    vector<User> recipients;
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
    viewConversation(currentUserId, selectedRecipient.getId(), auth, msgService);
}

//Function to view conversation and handle continuous reply
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
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        getline(cin, replyContent);

        if (replyContent == "!back") {
            cin.clear();
            return;
        }
        if (replyContent.empty()) continue;
        time_t now = time(0);
        msgService.createMessage(currentUserId, partnerId, replyContent, now);
        cout << "\nMessage sent to " << partnerName << "." << endl;
    }
}

//Function to view existing conversations
void viewAllMessages(const string& currentUserId, AuthService& auth, MessageService& msgService) {
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
    viewConversation(currentUserId, selectedPartnerId, auth, msgService);
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
            printAllForums(forumService, user);
            choice = -1; 
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
                forumService.editForum(user.getId(), targetForumId, newContent);

                cout << endl << "Here is your updated forum detail:" << endl;
                cout << string(70, '-') << endl;
                Forum updatedForum = forumService.getForumById(targetForumId);
                getForumDetail(updatedForum, user);
            }
            choice = -1;
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

void showStudentMenu(
    const Student& student,
    AuthService& auth,
    MessageService& msgService,
 //   const vector<User*>& users,
    FeedbackService& feedbackService
);

void showTutorMenu(
    const Tutor& tutor,
    AuthService& auth,
    MessageService& msgService,
 //   const vector<User*>& users,
    FeedbackService& feedbackService
);

int main() {
    cout << "\n..........Application Starting...........\\n" << endl;

    // Instantiate all services
    AuthService auth;
    CourseService courseService;
    MessageService msgService;
    FeedbackService feedbackService("feedback.txt");
   
    Course c1("1", "CMPT 1250", "Intro to Programming");
    courseService.createCourse(c1);
    courseService.updateCourse(c1);
    courseService.getCourse("1");
    courseService.getAllCourses();

    while (true) {
        int choice;

        if (auth.isLoggedIn()) {
            if (auth.currentRole() == 0) {
                showStudentMenu(auth.currentStudent(), auth, msgService, feedbackService);
            }
            else if (auth.currentRole() == 1) {
                showTutorMenu(auth.currentTutor(), auth, msgService, feedbackService);
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

//Student Menu Function
void showStudentMenu(
    const Student& student,
    AuthService& auth,
    MessageService& msgService,
    FeedbackService& feedbackService
) {
    while (true) {
        int choice;

        cout << "\n--------------------- Student Menu --------------------\n";
        cout << "Welcome, " << student.getName() << "!\n";
        cout << "1. View Profile\n";
        cout << "2. View Messages\n";
        cout << "3. Send New Message\n";
        cout << "4. Tutoring Sessions\n";
        cout << "5. Forum\n";
        cout << "6. Leave Feedback\n";
        cout << "7. Logout\n";
        cout << "Choose an option (1-7): ";

        if (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
        case 1:
            cout << "\n------------------- Student Profile -------------------\n";
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
            cout << "\n(No session viewing implemented yet)\n";
            break;

        case 5:
            getForumPage(student);
            break;

        case 6: // Feedback
        {
            vector<User> allUsers = auth.getAllUsers();
            vector<User> tutors;
            for (const User& u : allUsers) {
                if (u.getRole() == 1) { // 1 is the role for Tutor
                    tutors.push_back(u);
                }
            }

            if (tutors.empty()) {
                cout << "No tutors found to give feedback.\n";
                break;
            }

            cout << "Select a tutor to give feedback:\n";
            for (size_t i = 0; i < tutors.size(); ++i)
                cout << i + 1 << ". " << tutors[i].getName() << endl;

            int tutorChoice;
            cin >> tutorChoice;
            if (tutorChoice < 1 || tutorChoice > tutors.size()) {
                cout << "Invalid choice.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            }

            int studentIdInt = stoi(student.getId().substr(1));
            int tutorIdInt = stoi(tutors[tutorChoice - 1].getId().substr(1));

            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clear leftover newline

            string feedbackContent;
            cout << "Enter your feedback: ";
            getline(cin, feedbackContent);

            feedbackService.addFeedback(
                studentIdInt,
                tutorIdInt,
                feedbackContent
            );

            cout << "Feedback submitted.\n";
            break;
        }

        case 7:
            auth.logout();
            cout << "Logged out successfully." << endl;
            return;

        default:
            cout << "Invalid option.\n";
        }
    }
}

void showTutorMenu(
    const Tutor& tutor,
    AuthService& auth,
    MessageService& msgService,
    FeedbackService& feedbackService
) {
    while (true) {
        int choice;

        cout << "\n---------------------- Tutor Menu ---------------------\n";
        cout << "Welcome, " << tutor.getName() << "!\n";
        cout << "1. View Profile\n";
        cout << "2. View Messages\n";
        cout << "3. Send New Message\n";
        cout << "4. Tutoring Sessions\n";
        cout << "5. Forum\n";
        cout << "6. View Feedback\n";
        cout << "7. Logout\n";
        cout << "Choose an option (1-7): ";

        if (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
        case 1:
            cout << "\n------------------- Tutor Profile -------------------\n";
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
            cout << "\n(No session management implemented yet)\n";
            break;

        case 5:
            getForumPage(tutor);
            break;

        case 6:
        {
            vector<Feedback> tutorFeedback = feedbackService.getFeedbackForTutor(stoi(tutor.getId().substr(1)));
            if (tutorFeedback.empty()) {
                cout << "No feedback yet.\n";
            }
            else {
                cout << "Feedback received:\n";
                for (const auto& f : tutorFeedback)
                    cout << "Student ID " << f.studentId << ": " << f.content << endl;
            }
            break;
        }

        case 7:
            auth.logout();
            cout << "Logged out successfully." << endl;
            return;

        default:
            cout << "Invalid option.\n";
        }
    }
}
