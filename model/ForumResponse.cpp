#include "ForumResponse.h"
#include "Forum.h"
#include <string>

using namespace std;

ForumResponse::ForumResponse()
{

}

ForumResponse::ForumResponse(const Forum& root)
    : root(root) {
}

Forum ForumResponse::getRoot() const
{
    return this->root;
}

vector<ForumResponse> ForumResponse::getReplies() const
{
    return this->replies;
}

void ForumResponse::addReply(const ForumResponse& replyForum)
{
    this->replies.push_back(replyForum);
}


ForumResponse::~ForumResponse() {
}