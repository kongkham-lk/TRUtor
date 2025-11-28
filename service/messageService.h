#ifndef MESSAGESERVICE_H
#define MESSAGESERVICE_H

#include <string>
#include <ctime>
#include <list>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

//use string IDs and include a constructor
struct Message {
    int messageId;
    string senderId;  
    string receiverId; 
    string content;
    time_t timestamp;

    Message() : messageId(0), timestamp(0) {}

    Message(int id, const string& sender, const string& receiver, const string& msg, time_t ts)
        : messageId(id), senderId(sender), receiverId(receiver), content(msg), timestamp(ts) {}
};

class MessageService {
public:
    MessageService(); 

    void createMessage(const string& senderId, const string& receiverId, string content, time_t timeStamp);
    list<Message> getMessagesBetween(const string& userId1, const string& userId2);
    list<Message> getAllMessages();
    list<Message> getAllMessagesByTime(int senderId, int receiverId, time_t timeStamp);
    void deleteMessagesBetween(int senderId, int receiverId);
    int fetchMessageIdFromDb(int senderId, int receiverId, time_t timeStamp);

private:
    list<Message> messages;
    void loadMessages();
    void saveMessages() const;
    string escapeContent(const string& content) const;
    string unescapeContent(const string& content) const;
};

#endif // MESSAGESERVICE_H