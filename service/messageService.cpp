#include "MessageService.h"
#include <iostream>
#include <ctime>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <limits>
#include <cmath> 

MessageService::~MessageService() {}

//Replace spaces with underscores for file storage
string MessageService::escapeContent(const string& content) const {
    string escaped = content;
    replace(escaped.begin(), escaped.end(), ' ', '_');
    return escaped;
}

//Replace underscores with spaces after file read
string MessageService::unescapeContent(const string& content) const {
    string unescaped = content;
    replace(unescaped.begin(), unescaped.end(), '_', ' ');
    return unescaped;
}

//Loads all messages from messages.txt into the in-memory list
void MessageService::loadMessages() {
    messages.clear();
    ifstream file("messages.txt");
    if (!file.is_open()) return;

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        Message msg;
        long long timestamp_ll;
        string escapedContent;

        //File format messageId senderId receiverId timestamp escapedContent
        if (ss >> msg.messageId >> msg.senderId >> msg.receiverId >> timestamp_ll >> escapedContent) {
            msg.timestamp = (time_t)timestamp_ll;
            msg.content = unescapeContent(escapedContent);
            messages.push_back(msg);
        }
    }
    file.close();
}

//Saves all messages from the in-memory list to messages.txt
void MessageService::saveMessages() const {
    ofstream file("messages.txt");
    if (!file.is_open()) {
        cerr << "Error: Could not open messages.txt for writing." << endl;
        return;
    }
    for (const auto& msg : messages) {
        file << msg.messageId << " "
            << msg.senderId << " "
            << msg.receiverId << " "
            << (long long)msg.timestamp << " "
            << escapeContent(msg.content) << "\n";
    }
    file.close();
}

MessageService::MessageService() {
    loadMessages();
}

void MessageService::createMessage(const string& senderId, const string& receiverId, string content, time_t timeStamp) {
    loadMessages();

    int maxId = 0;
    for (const auto& existing_msg : messages) {
        if (existing_msg.messageId > maxId) {
            maxId = existing_msg.messageId;
        }
    }
    Message msg(maxId + 1, senderId, receiverId, content, timeStamp);
    messages.push_back(msg);
    saveMessages();
}

list<Message> MessageService::getMessagesBetween(const string& userId1, const string& userId2)
{
    loadMessages(); 
    list<Message> result;

    for (const auto& current : messages)
    {
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

list<Message> MessageService::getAllMessages() {
    loadMessages();
    return messages;
}

list<Message> MessageService::getAllMessagesByTime(int senderId, int receiverId, time_t timeStamp)
{
    loadMessages();
    return {};
}

void MessageService::deleteMessagesBetween(int senderId, int receiverId)
{
    loadMessages();
    saveMessages();
}

int MessageService::fetchMessageIdFromDb(int senderId, int receiverId, time_t timeStamp)
{
    return -1;
}