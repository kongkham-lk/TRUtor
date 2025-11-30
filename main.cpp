#include <iostream>
#include <vector>
#include <string>
#include <list>
#include <algorithm>
#include <sstream>
#include <limits>
#include <ctime>
#include <set>
#include <map>

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

void getForumDetail(const Forum& forum, const string& creatorName, int indentSize = 0)
{

    cout << string(indentSize, ' ') << "Forum Id: " << forum.getId() << endl;
    cout << string(indentSize, ' ') << "CreateBy: " << creatorName << endl;
    cout << string(indentSize, ' ') << "CreateAt: " << forum.getCreatedAt() << endl;
    cout << string(indentSize, ' ') << "Content: " << forum.getContent() << endl;
    //
    // cout << endl << "Start Debugging...." << endl;
    // vector<int> replyForumsIds = forum.getReplyForumsId();
    // string replyForumsIdsStr = "_";
    // for (int i = 0; i < replyForumsIds.size(); i++)
    //     replyForumsIdsStr += to_string(replyForumsIds[i]) + "_";
    // cout << string(indentSize, ' ') << "replyIds: " << replyForumsIdsStr << endl;
    // cout << endl << "End Debugging...." << endl;

    cout << string(indentSize, ' ') << string(70-indentSize, '-') << endl;
}
//
// void printForumResponse(const ForumResponse& r, int indent = 0)
// {
//     printForum(r.getRoot(), indent);
//
//     for (const ForumResponse& child : r.getReplies())
//         printForumResponse(child, indent + 4);
// }

void getForumResponseDetail(const ForumResponse& forum, const map<string, string>& userIdByNameDict, int indent = 0)
{
    Forum mainForum = forum.getRoot();

    // // print original post
    // cout << "Lookup UserId (mainForum): " << mainForum.getId() << endl;
    if (mainForum.getId() == -1) return;

    getForumDetail(mainForum, userIdByNameDict.at(mainForum.getCreatorId()), indent);

    // print all its sub reply post
    if (vector<ForumResponse> replyForums = forum.getReplies(); !replyForums.empty())
    {
        cout << string(indent, ' ') << "Reply Post " << string(60-indent, '-') << endl;
        for (const ForumResponse& child : forum.getReplies())
            getForumResponseDetail(child, userIdByNameDict, indent + 4);
    }
    else
    {
        cout << string(indent, ' ') << "No Reply Post!" << endl;
        cout << string(70, '-') << endl;
    }
}

template <typename T>
bool checkIfForumsEmpty(const vector<T>& forums)
{
    if (forums.empty())
    {
        cout << "No Forum Found!" << endl;
        return true;
    }
    return false;
}

map<string, string> getAllUsers(const AuthService& auth)
{
    vector<User> users = auth.getAllUsers();
    map<string, string> userIdByNameDict;
    for (User user : users)
    {
        userIdByNameDict[user.getId()] = user.getName();
    }
    return userIdByNameDict;
}

bool printAllForumsResponse(const vector<ForumResponse>& forums, const AuthService& auth)
{
    if (checkIfForumsEmpty(forums)) return false;

    // need to refetch user every time require update on All created forum display
    map<string, string> userIdByNameDict = getAllUsers(auth);

    cout << endl << "All Forums:" << endl;
    cout << string(70, '-') << endl;

    for (const ForumResponse& forum : forums)
    {
        getForumResponseDetail(forum, userIdByNameDict);
    }
    return true;
}

bool printAllForums(const vector<Forum>& forums, const AuthService& auth)
{
    if (checkIfForumsEmpty(forums)) return false;
    map<string, string> userIdByNameDict = getAllUsers(auth); // need to refetch user every time require update on All created forum display

    cout << endl << "All Forums:" << endl;
    cout << string(70, '-') << endl;
    for (const Forum& forum : forums)
    {
        if (forum.getId() == -1) continue;

        // cout << "Lookup UserId (forum): " << forum.getCreatorId() << endl;
        getForumDetail(forum, userIdByNameDict.at(forum.getCreatorId()));
    }
    return true;
}

int getForumIdFromUser()
{
    int targetForumId = -1;
    cout << endl << "Enter the ID of the forum: ";
    if (!(cin >> targetForumId)) {
        cout << "Invalid ID. Returning to forum menu." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return -1;
    }
    return targetForumId;
}

string getForumContentFromUser()
{
    string newContent;
    cout << "Please enter new content: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, newContent);
    return newContent;
}

void showForumUpdateResult(const User& user, Forum updatedForum)
{
    cout << endl << "Here is your updated forum detail:" << endl;
    cout << string(70, '-') << endl;
    getForumDetail(updatedForum, user.getName());

}

void getForumPage(const User& user, const AuthService& auth)
{
    cout << endl << string(32, '-') << " Forum Dashboard " << string(32, '-') << endl;

    ForumService forumService;
    vector<ForumResponse> forums = forumService.getAllForums();
    printAllForumsResponse(forums, auth);

    int choice = -1;
    do
    {
        cout << endl << "Choose an option to proceed:" << endl;
        cout << "1. Create a new forum" << endl;
        cout << "2. Reply to a specific forum" << endl;
        cout << "3. Manage your forums" << endl;
        cout << "4. Back to main page" << endl;
        cout << "Please Enter: ";

        if (!(cin >> choice)) {
            cout << endl << "Invalid Option, Please try again!" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            choice = -1;
        }
        else if (choice == 1) // Create Forum
        {
            string newContent = getForumContentFromUser();
            forumService.createForumAndSave(user.getId(), newContent);

            cout << endl << "Forum Creation Successfully..." << endl;
            vector<ForumResponse> forums = forumService.getAllForums();
            printAllForumsResponse(forums, auth);
            choice = -1; 
        }
        else if (choice == 2) // Reply Forum
        {
            if (int targetForumId = getForumIdFromUser(); targetForumId != -1)
            {
                string newContent = getForumContentFromUser();
                forumService.replyToForum(user.getId(), newContent, targetForumId);

                vector<ForumResponse> updatedForums = forumService.getAllForums();
                map<string, string> userIdByNameDict = getAllUsers(auth);
                for (ForumResponse updatedForum : updatedForums)
                {
                    if (updatedForum.getRoot().getId() == targetForumId)
                    {
                        cout << endl << "Found Forum Updated..." << endl;
                        getForumResponseDetail(updatedForum, userIdByNameDict);
                        break;
                    }
                }
            }
            choice = -1;
        }
        else if (choice == 3) // Manage Forum
        {
            if (vector<Forum> forums = forumService.getAllForumsByUserId(user.getId()); printAllForums(forums, auth))
            {
                if (int targetForumId = getForumIdFromUser(); targetForumId != -1)
                {
                    int subChoice = -1;
                    cout << endl << "Choose an option to proceed:" << endl;
                    cout << "1. Update Forum Content" << endl;
                    cout << "2. Delete Forum" << endl;
                    cout << "3. Go Back" << endl;
                    cout << "Please Enter: ";

                    if (!(cin >> subChoice)) {
                        cout << endl << "Invalid Option, Please try again!" << endl;
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    }
                    else if (subChoice == 1) // Update Forum Content
                    {
                        string newContent = getForumContentFromUser();
                        forumService.editForumContent(user.getId(), targetForumId, newContent);

                        Forum updatedForum = forumService.getForumById(targetForumId);
                        showForumUpdateResult(user, updatedForum);
                    }
                    else if (subChoice == 2) // Delete the form
                    {
                        forumService.deleteForums(user.getId(), targetForumId);

                        cout << endl << "Forum Deletion Successfully..." << endl;
                        vector<ForumResponse> forums = forumService.getAllForums();
                        printAllForumsResponse(forums, auth);
                    }
                }
            }
            choice = -1;
        }
        else if (choice == 4) // Back to Main Menu
            break;
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
            getForumPage(student, auth);
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
            getForumPage(tutor, auth);
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
