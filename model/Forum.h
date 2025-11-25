#ifndef FORUM_H
#define FORUM_H

#include <string>
#include <chrono>

using namespace std;
using namespace chrono;

class Forum {
private:
    int id;
    string userId;   // The user ID that create forum post
    string content;
    time_t createdAt;

public:
    Forum(int id, const string& userId, const string& msg, time_t timestamp = system_clock::to_time_t(system_clock::now())); // Constructor

    const int getId() const;

    const string& getUserId() const;

    const string& getContent() const;
    void setContent(const string& content);

    time_t getCreatedAt() const;
    bool operator==(const Forum& other) const;

    ~Forum(); // Destructor

};

#endif // FORUM_H