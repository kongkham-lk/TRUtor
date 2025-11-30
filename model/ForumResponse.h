#ifndef FORUMRESPONSE_H
#define FORUMRESPONSE_H

#include <string>
#include <chrono>
#include <vector>
#include "Forum.h"

using namespace std;
using namespace chrono;

class ForumResponse {
private:
    Forum root;
    vector<ForumResponse> replies;

public:
    ForumResponse();
    ForumResponse(const Forum& root); // Constructor

    Forum getRoot() const;

    vector<ForumResponse> getReplies() const;
    void addReply(const ForumResponse& replyForum);

    ~ForumResponse(); // Destructor

};

#endif // FORUMRESPONSE_H