#include <string>
#include <ctime>
#include <list>

using namespace std;

struct Message {
	int messageId;
	int senderId;
	int receiverId;
	string content;
	time_t timestamp;
};

class MessageService {
public:

	void createMessage(int senderId, int receiverId, string content, time_t timeStamp);

	list<Message> getMessagesBetween(int senderId, int receiverId);

	list<Message> getAllMessagesByTime(int senderId, int receiverId, time_t timeStamp);

	void deleteMessagesBetween(int senderId, int receiverId);

	int fetchMessageIdFromDb(int senderId, int receiverId, time_t timeStamp);


private:
	list<Message> messages;

};

