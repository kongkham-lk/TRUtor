#ifndef MESSAGESERVICE_H
#define MESSAGESERVICE_H

#include <string>
#include <ctime>
#include <list>
#include <fstream> // For file I/O
#include <sstream> // For string stream parsing
#include <iostream> // For error logging

using namespace std;

// Updated struct to use string IDs and include a constructor
struct Message {
    int messageId;
    string senderId;   // CHANGED FROM int TO string
    string receiverId; // CHANGED FROM int TO string
    string content;
    time_t timestamp;

    // Default constructor
    Message() : messageId(0), timestamp(0) {}

    // Convenience constructor
    Message(int id, const string& sender, const string& receiver, const string& msg, time_t ts)
        : messageId(id), senderId(sender), receiverId(receiver), content(msg), timestamp(ts) {}
};

class MessageService {
public:
    MessageService(); // New: Constructor to load messages on startup

    // Updated function signatures to use string IDs
    void createMessage(const string& senderId, const string& receiverId, string content, time_t timeStamp);

    // Updated function signatures to use string IDs
    list<Message> getMessagesBetween(const string& userId1, const string& userId2);

    // New: Function to get all messages for checking conversations
    list<Message> getAllMessages();

    // The rest of the original functions (keeping them for now but not using)
    list<Message> getAllMessagesByTime(int senderId, int receiverId, time_t timeStamp);
    void deleteMessagesBetween(int senderId, int receiverId);
    int fetchMessageIdFromDb(int senderId, int receiverId, time_t timeStamp);

private:
    list<Message> messages;

    // New private helpers for persistence
    void loadMessages();
    void saveMessages() const;

    // Helper to escape/unescape content for file storage
    string escapeContent(const string& content) const;
    string unescapeContent(const string& content) const;
};

#endif // MESSAGESERVICE_H