#include "MessageService.h"
#include <iostream>
#include <ctime>
using namespace std;

int main() 
{
    MessageService msg;
    time_t t = time(0);

    msg.createMessage(1, 2, "Hello", t);
    msg.createMessage(2, 1, "Hi", t);

    auto msgs = msg.getAllMessagesByUserId(1, 2);
    for (const auto& m : msgs)
        cout << m.messageId << ": " << m.content << endl;

    int id = msg.fetchMessageIdFromDb(1, 2, t);
    cout << "Fetched message ID: " << id << endl;

    msg.deleteMessages(1, 2);
    cout << "Deleted messages between 1 and 2." << endl;

    return 0;
}