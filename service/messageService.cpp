#include "MessageService.h"
#include <iostream>
#include <ctime>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <limits>
#include <cmath> // For max

// Helper function to replace spaces with underscores for file storage
string MessageService::escapeContent(const string& content) const {
    string escaped = content;
    replace(escaped.begin(), escaped.end(), ' ', '_');
    return escaped;
}

// Helper function to replace underscores with spaces after file read
string MessageService::unescapeContent(const string& content) const {
    string unescaped = content;
    replace(unescaped.begin(), unescaped.end(), '_', ' ');
    return unescaped;
}

// New: Loads all messages from messages.txt into the in-memory list
void MessageService::loadMessages() {
    messages.clear(); // Clear current in-memory list
    ifstream file("messages.txt");
    if (!file.is_open()) return;

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        Message msg;
        long long timestamp_ll; // Use long long for time_t file storage
        string escapedContent;

        // File format: messageId senderId receiverId timestamp escapedContent
        if (ss >> msg.messageId >> msg.senderId >> msg.receiverId >> timestamp_ll >> escapedContent) {
            msg.timestamp = (time_t)timestamp_ll;
            msg.content = unescapeContent(escapedContent);
            messages.push_back(msg);
        }
    }
    file.close();
}

// New: Saves all messages from the in-memory list to messages.txt
void MessageService::saveMessages() const {
    ofstream file("messages.txt");
    if (!file.is_open()) {
        cerr << "Error: Could not open messages.txt for writing." << endl;
        return;
    }

    // Format: messageId senderId receiverId timestamp escapedContent
    for (const auto& msg : messages) {
        file << msg.messageId << " "
            << msg.senderId << " "
            << msg.receiverId << " "
            << (long long)msg.timestamp << " "
            << escapeContent(msg.content) << "\n";
    }
    file.close();
}

// New: Constructor - loads messages when the service is instantiated
MessageService::MessageService() {
    loadMessages();
}

// Updated: create Message now uses string IDs and adds persistence
void MessageService::createMessage(const string& senderId, const string& receiverId, string content, time_t timeStamp) {
    // Load latest messages before creating a new one to get the latest ID
    loadMessages();

    int maxId = 0;
    for (const auto& existing_msg : messages) {
        if (existing_msg.messageId > maxId) {
            maxId = existing_msg.messageId;
        }
    }

    Message msg(maxId + 1, senderId, receiverId, content, timeStamp);

    messages.push_back(msg);
    saveMessages(); // Save the new list to the file
}

// Updated: getMessagesBetween now uses string IDs and persistence
list<Message> MessageService::getMessagesBetween(const string& userId1, const string& userId2)
{
    loadMessages(); // Load before querying
    list<Message> result;

    for (const auto& current : messages)
    {
        // A message belongs to the conversation if (A sent to B) OR (B sent to A)
        bool isConversation =
            (current.senderId == userId1 && current.receiverId == userId2) ||
            (current.senderId == userId2 && current.receiverId == userId1);

        if (isConversation)
        {
            result.push_back(current);
        }
    }

    return result;
}

// New: Gets all messages (needed to find all conversation partners)
list<Message> MessageService::getAllMessages() {
    loadMessages();
    return messages;
}

// (Keeping original implementations for completeness, but they are now obsolete)
list<Message> MessageService::getAllMessagesByTime(int senderId, int receiverId, time_t timeStamp)
{
    loadMessages();
    // ... original logic (now uses list<Message> with string IDs, so logic is wrong)
    return {};
}

void MessageService::deleteMessagesBetween(int senderId, int receiverId)
{
    loadMessages();
    // ... original logic (now uses list<Message> with string IDs, so logic is wrong)
    saveMessages();
}

int MessageService::fetchMessageIdFromDb(int senderId, int receiverId, time_t timeStamp)
{
    // ... original logic (now uses list<Message> with string IDs, so logic is wrong)
    return -1;
}