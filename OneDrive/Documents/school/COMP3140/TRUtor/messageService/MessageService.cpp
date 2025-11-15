#include "MessageService.h"
#include <iostream>
#include <ctime>

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

list<Message> MessageService::getAllMessagesByUserId(int senderId, int receiverId)
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

void MessageService::deleteMessages(int senderId, int receiverId)
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