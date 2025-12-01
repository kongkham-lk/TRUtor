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
    cout << "\n----------------------- Select Recipient ------------------------\n";
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
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
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
        cout << " \n";
        cout << "1. Create a new Forum" << endl;
        cout << "2. Manage your forums" << endl;
        cout << "3. Back to main page" << endl;
        cout << "--------------------------------------------------------\n";
        cout << "Please choose an option (1-3) : ";
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


bool isValidTimeFormat(const string& time) {
    // Must be exactly 5 characters: HH:MM
    if (time.size() != 5) return false;

    // Check colon
    if (time[2] != ':') return false;

    // Check digits
    if (!isdigit(time[0]) || !isdigit(time[1]) ||
        !isdigit(time[3]) || !isdigit(time[4])) return false;
    // Convert
    int hour = stoi(time.substr(0, 2));
    int min  = stoi(time.substr(3, 2));

    // Valid ranges
    if (hour < 0 || hour > 23) return false;
    if (min < 0 || min > 59) return false;

    return true;
}


void showStudentMenu(
    const Student& student,
    AuthService& auth,
    MessageService& msgService,
 //   const vector<User*>& users,
    FeedbackService& feedbackService,
    CourseService& courseService  // used for the course management
);

void showTutorMenu(
    const Tutor& tutor,
    AuthService& auth,
    MessageService& msgService,
 //   const vector<User*>& users,
    FeedbackService& feedbackService,
    CourseService& courseService  // used for the course management
);

int main() {
    cout << "\n.....................Application Starting.......................\\n" << endl;

    // Instantiate all services
    AuthService auth;
    CourseService courseService;
    MessageService msgService;
    FeedbackService feedbackService("feedback.txt");
   

    while (true) {
        int choice;

        if (auth.isLoggedIn()) {
            if (auth.currentRole() == 0) {
                showStudentMenu(auth.currentStudent(), auth, msgService, feedbackService, courseService);   //note
            }
            else if (auth.currentRole() == 1) {
                showTutorMenu(auth.currentTutor(), auth, msgService, feedbackService, courseService);   //note
            }
        }


    

        cout << "\n---------------------- Main Menu ----------------------\n";
        cout << "1. Login\n";
        cout << "2. Signup Student\n";
        cout << "3. Signup Tutor\n";
        cout << "4. Exit\n";
        cout << "--------------------------------------------------------\n";
        cout << "Please choose an option (1-4) : ";

        if (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        string email, password, name, major;

        switch (choice) {
        case 1:
            cout << "Enter email : ";
            cin >> email;
            cout << "Enter password : ";
            cin >> password;
            if (auth.login(email, password)) {
                cout << "Login successful." << endl;
            }
            else {
                cout << "Login failed. Check email or password." << endl;
            }
            break;

        case 2:
            cout << "Enter name : ";
            cin >> name;
            cout << "Enter email : ";
            cin >> email;
            cout << "Enter password : ";
            cin >> password;
            cout << "Enter major : ";
            cin >> major;
            if (auth.signUpStudent(email, password, name, major)) {
                cout << "Student account created successfully! You are now logged in." << endl;
            }
            break;

        case 3:
            cout << "Enter name : "; cin.ignore(); 
            getline(cin, name);
    
            cout << "Enter email : ";
            cin >> email;
            cout << "Enter password : ";
            cin >> password;

            if (auth.signUpTutor(email, password, name)) {
                cout << " ";
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
    FeedbackService& feedbackService,
    CourseService& courseService  // used for the course management
) {
    while (true) {
        int choice;

        cout << "\n--------------------- Student Menu --------------------\n";
        cout << "Welcome, " << student.getName() << "!\n";
        cout << "-----------------------------------------------\n";
        cout << "1. View Profile\n";
        cout << "2. View Messages\n";
        cout << "3. Send New Message\n";
        cout << "4. Tutoring Sessions\n";
        cout << "5. Forum\n";
        cout << "6. Leave Feedback\n";
        cout << "7. Logout\n";
        cout << "-----------------------------------------------\n";
        cout << "Please choose an option (1-7) : ";

        if (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
        case 1:
            cout << "\n------------------- Student Profile -------------------\n";
            cout << "ID : " << student.getId() << "\n";
            cout << "Name : " << student.getName() << "\n";
            cout << "Email : " << student.getEmail() << "\n";
            cout << "Major : " << student.getMajor() << "\n";
            break;

        case 2:
            viewAllMessages(student.getId(), auth, msgService);
            break;

        case 3:
            sendMessageToUser(student.getId(), auth, msgService);
            break;

        case 4:
        {
            while (true) {
            cout << "\n------------------------ Tutoring Sessions ---------------------\n";

            int option;
            cout << "1. Browse All Courses\n";
            cout << "2. Enroll in a Course\n";
            cout << "3. Withdraw from a Course\n";
            cout << "4. Leave Waitlist\n";
            cout << "5. View My Enrolled Courses\n";
            cout << "6. View My Waitlisted Courses\n";
            cout << "7. Back to Student Menu\n";
            cout << "---------------------------------------------------------------\n";
            cout << "Please choose an option (1-7) : ";
            cin >> option;

            string cid;

            switch (option) {
                case 1:
                    courseService.listAllCourses();
                    break;

                case 2:
                {
                    cout << "Enter Course ID : ";
                    cin >> cid;

                    Course c = courseService.getCourse(cid);
                    if (c.getId().empty()) {
                        cout << "Course does not exist.\n";
                        break;
                    }

                    if (courseService.enrollStudent(student.getId(), cid))
                        cout << "\n Successfully Enrolled!\n";
                    else {
                        cout << "Course " << cid << " is full. \n";
                        cout << "Do you want to join the waitlist? (y/n): ";
                        char waitlistChoice;
                        cin >> waitlistChoice;
                        if (waitlistChoice == 'y' || waitlistChoice == 'Y') {
                            courseService.joinWaitlist(student.getId(), cid);
                            cout << "Added to waitlist.\n";
                        }
                        else {
                            cout << "Not added to waitlist.\n";
                        }
                    }
                    break;
                }
                case 3:
                    cout << "Enter Course ID: ";
                    cin >> cid;
                    if (courseService.removeStudent(student.getId(), cid))
                        cout << "\nWithdrawn from course " << cid << ".\n";
                    else
                        cout << "Not enrolled.\n";
                    break;

                case 4:
                    cout << "Enter Course ID: ";
                    cin >> cid;
                    if (courseService.leaveWaitlist(student.getId(), cid))
                        cout << "Removed from waitlist.\n";
                    else
                        cout << "Not in waitlist.\n";
                    break;

                case 5:
                    courseService.listStudentCourses(student.getId());
                    break;

                case 6:
                    courseService.listStudentWaitlisted(student.getId());
                    break;
                case 7:
                    break;
                default:
                    cout << "Invalid option.\n";
            }

            }

            break;
        }
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

            cout << "Select a tutor to give feedback :\n";
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
            cout << "Please enter your feedback : ";
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
    FeedbackService& feedbackService,
    CourseService& courseService  // used for the course management
) {
    while (true) {
        int choice;

        cout << "\n---------------------- Tutor Menu ---------------------\n";
        cout << "Welcome, " << tutor.getName() << "!\n";
        cout << "--------------------------------------------------------\n";
        cout << "1. View Profile\n";
        cout << "2. View Messages\n";
        cout << "3. Send New Message\n";
        cout << "4. Manage Tutoring Sessions\n";
        cout << "5. Forum\n";
        cout << "6. View Feedback\n";
        cout << "7. Logout\n";
        cout << "--------------------------------------------------------\n";
        cout << "Please choose an option (1-7): ";

        if (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
        case 1:
            cout << "\n------------------- Tutor Profile -------------------\n";
            cout << "ID : " << tutor.getId() << "\n";
            cout << "Name : " << tutor.getName() << "\n";
            cout << "Email : " << tutor.getEmail() << "\n";
            break;

        case 2:
            viewAllMessages(tutor.getId(), auth, msgService);
            break;

        case 3:
            sendMessageToUser(tutor.getId(), auth, msgService);
            break;

        case 4:
        {
            while (true) {
                    cout << "\n---------------------- Tutoring Sessions Management -----------------------\n";

            int option;
            cout << "1. Create a Course\n";
            cout << "2. Edit an existing Course\n";
            cout << "3. Delete a Course\n";
            cout << "4. View My Courses\n";
            cout << "5. Back to Tutor Menu\n";
            cout << "-------------------------------------------------------------------------\n";
            cout << "Please choose an option (1-5) : ";
            cin >> option;

            string cid;

            switch (option) {
                case 1: {
                    bool continueAdding;
                    
                    do
                    {
                        continueAdding = false;
                        string code, title, desc, day, start, end;
                        int cap;
                        int dayOption;
                        cout << "\nPlease enter the required course details.\n";
                        cout << "\n";
                        cout << "Code : "; cin >> code;
                        cout << "Title : "; cin.ignore(); getline(cin, title);
                        cout << "Description : "; getline(cin, desc);
                        cout << "Capacity : "; cin >> cap;
                        cout << "Day : "; 
                        cout << "\t1. Monday\n\t2. Tuesday\n\t3. Wednesday\n\t4. Thursday\n\t5. Friday\n\t6. Saturday\n\t7. Sunday\n";
                        cin >> dayOption;
                        cout << "Please choose a day (1-7) : ";
                        switch (dayOption) {
                            case 1: day = "MON"; break;
                            case 2: day = "TUE"; break;
                            case 3: day = "WED"; break;
                            case 4: day = "THU"; break;
                            case 5: day = "FRI"; break;
                            case 6: day = "SAT"; break;
                            case 7: day = "SUN"; break;
                            default: 
                                cout << "Invalid day selection!\n";
                                continueAdding = true;
                        }
                        cout << "Start Time (HH:MM) : "; cin >> start;
                        cout << "End Time (HH:MM) : "; cin >> end;
                        cout << "\n";

                        if (cap <= 0) {
                            cout << "Invalid capacity.\n";
                            continueAdding = true;
                        }
                        
                        if (day == "Invalid") {
                            cout << "Invalid day selection.\n";
                            continueAdding = true;
                        }
                        if (!isValidTimeFormat(start) || !isValidTimeFormat(end)) {
                            cout << "Invalid time format. Please use 24 hour HH:MM format (e.g., 14:30).\n";
                            continueAdding = true;
                        }
                        else {
                            // Only compare if format valid
                            if (start >= end) {
                                cout << "Start time must be before end time.\n";
                                continueAdding = true;
                            }
                        }

                        if (!continueAdding) {
                            Course c("TEMP", code, title, desc, cap,
                            tutor.getId(), day, start, end);
                            courseService.createCourse(c);
                            cout << "\nCourse Created Successfully...\n";
                            cout << "-------------------------------------------------------\n";
                        }
                        else {
                            cout << "Course Creation Aborted.\n";
                            cout << "-------------------------------------------------------\n";
                        }
                        
                    } while (continueAdding);
                    
                    break;
                }

                case 2:
                {
                    string cid;
                    cout << "Enter Course ID to edit : ";
                    cin >> cid;

                    Course course = courseService.getCourse(cid);

                    if (course.getId() == "") {
                        cout << "Course not found.\n";
                        return;
                    }

                    int opt;
                    do {
                        cout << "\n------------------- Edit Course (" << course.getId() << ") -------------------\n";
                        cout << "1. Edit Title\n";
                        cout << "2. Edit Description\n";
                        cout << "3. Edit Capacity\n";
                        cout << "4. Edit Day\n";
                        cout << "5. Edit Start Time\n";
                        cout << "6. Edit End Time\n";
                        cout << "7. Save & Exit\n";
                        cout << "Choose: ";
                        cin >> opt;

                        cin.ignore(numeric_limits<streamsize>::max(), '\n');

                        if (opt == 1) {
                            string t;
                            cout << "New Title: ";
                            getline(cin, t);
                            course.setTitle(t);
                        }
                        else if (opt == 2) {
                            string d;
                            cout << "New Description: ";
                            getline(cin, d);
                            course.setDescription(d);
                        }
                        else if (opt == 3) {
                            int cap;
                            cout << "New Capacity: ";
                            cin >> cap;

                            if (cap <= 0) {
                                cout << " Invalid capacity.\n";
                            } else {
                                course.setCapacity(cap);
                            }
                        }
                        else if (opt == 4) {
                            int dayOpt;
                            cout << "Choose New Day (1-7)\n"
                                    "1 MON\n2 TUE\n3 WED\n4 THU\n5 FRI\n6 SAT\n7 SUN\n";
                            cin >> dayOpt;

                            switch (dayOpt) {
                                case 1: course.setDay("MON"); break;
                                case 2: course.setDay("TUE"); break;
                                case 3: course.setDay("WED"); break;
                                case 4: course.setDay("THU"); break;
                                case 5: course.setDay("FRI"); break;
                                case 6: course.setDay("SAT"); break;
                                case 7: course.setDay("SUN"); break;
                                default: cout << "Invalid option.\n"; break;
                            }
                        }
                        else if (opt == 5) {
                            string s;
                            cout << "New Start Time (HH:MM) : ";
                            cin >> s;

                            if (!isValidTimeFormat(s)) {
                                cout << " Invalid time format.\n";
                            } else if (s >= course.getEndTime()) {
                                cout << " Start time must be before end time.\n";
                            } else {
                                course.setStartTime(s);
                            }
                        }
                        else if (opt == 6) {
                            string e;
                            cout << "New End Time (HH:MM) : ";
                            cin >> e;

                            if (!isValidTimeFormat(e)) {
                                cout << " Invalid time format.\n";
                            } else if (e <= course.getStartTime()) {
                                cout << "End time must be after start time.\n";
                            } else {
                                course.setEndTime(e);
                            }
                        }

                    } while (opt != 7);

                    courseService.updateCourse(course);
                    cout << "\n Course updated successfully.\n";
                    cout << "-------------------------------------------------------\n";
                    break;
                }

                case 3:
                    cout << "Enter Course ID to delete : ";
                    cin >> cid;
                    if (courseService.deleteCourse(tutor.getId(), cid))
                        cout << "Course deleted Successfully.\n";
                    break;

                case 4:
                    courseService.listTutorCourses(tutor.getId());
                    break;

                case 5:
                    break;

                default:
                    cout << "Invalid option.\n";
            }
        }

            break;
        }
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
