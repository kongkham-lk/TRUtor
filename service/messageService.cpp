#include "MessageService.h"
#include <iostream>
#include <ctime>

// create Message with constructor variables
void MessageService::createMessage(int senderId, int receiverId, string content, time_t timeStamp)
{
    Message msg;

    msg.messageId = messages.size() + 1;
    msg.senderId = senderId;
    msg.receiverId = receiverId;
    msg.content = content;
    msg.timestamp = timeStamp;


    messages.push_back(msg);
}

// iterator that goes through all the messages between sender and receiver
list<Message> MessageService::getMessagesBetween(int senderId, int receiverId)
{
    list<Message> result;

    list<Message>::iterator it;

    for (it = messages.begin(); it != messages.end(); ++it)
    {
        Message current = *it;

        if ((current.senderId == senderId && current.receiverId == receiverId) || (current.senderId == receiverId && current.receiverId == senderId))
        {
            result.push_back(current);
        }
    }

    return result;
}

//iterator that read messages that is sent at the time that it is called
list<Message> MessageService::getAllMessagesByTime(int senderId, int receiverId, time_t timeStamp)
{
    list<Message> result;

    list<Message>::iterator it;

    for (it = messages.begin(); it != messages.end(); ++it)
    {
        Message current = *it;

        if (current.senderId == senderId && current.receiverId == receiverId && current.timestamp == timeStamp)
        {
            result.push_back(current);
        }
    }

    return result;
}

//delete all messages between sender and receiver
void MessageService::deleteMessagesBetween(int senderId, int receiverId)
{

    list<Message>::iterator it = messages.begin();

    while (it != messages.end())
    {
        Message current = *it;

        bool match = (current.senderId == senderId && current.receiverId == receiverId) || (current.senderId == receiverId && current.receiverId == senderId);

        if (match)
        {

            it = messages.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

//fetching message from database -- will be functional currently has a temporary placeholder
int MessageService::fetchMessageIdFromDb(int senderId, int receiverId, time_t timeStamp)
{
    list<Message>::iterator it;
    for (it = messages.begin(); it != messages.end(); ++it)
    {
        Message current = *it;

        if (current.senderId == senderId && current.receiverId == receiverId && current.timestamp == timeStamp)
        {
            return current.messageId;
        }
    }

    return -1;
}
