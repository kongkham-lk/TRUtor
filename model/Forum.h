#ifndef FORUM_H
#define FORUM_H

#include <string>
#include <chrono>
#include <vector>

using namespace std;
using namespace chrono;

class Forum {
private:
    int id;
    string creatorId;   // The user ID that create forum post
    string content;
    time_t createdAt{}; // default time
    vector<int> replyForumsId;
    int parentForumId = -1; // determine if the forum is the original post or the reply one (-1 is original post)

public:
    Forum();
    Forum(int id, string  userId, string  msg, int parentForumId = -1, time_t timestamp = 0); // Constructor

    const int getId() const;

    const string& getCreatorId() const;

    const string& getContent() const;
    void setContent(const string& content);

    time_t getCreatedAt() const;
    bool operator==(const Forum& other) const;

    vector<int> getReplyForumsId() const;
    void addReplyForumId(const int& replyForumId);
    void removeReplyForumId(const int& replyForumId);

    const int getParentForumId() const;
    void setParentForumId(const int& newParentForumId);

    ~Forum(); // Destructor

};

#endif // FORUM_H