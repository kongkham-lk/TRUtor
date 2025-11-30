#include "Forum.h"
#include <string>
#include <utility>

using namespace std;

Forum::Forum() : id(-1) // flag to check if forum object is null
{

}

Forum::Forum(int id, string  userId, string  msg, int parentForumId, time_t timestamp)
    : id(id),
    creatorId(std::move(userId)),
    content(std::move(msg)),
    parentForumId(parentForumId),
    createdAt(timestamp == 0 ? system_clock::to_time_t(system_clock::now()) : timestamp) {
}

const int Forum::getId() const {
    return this->id;
}

const string& Forum::getCreatorId() const {
    return this->creatorId;
}

const string& Forum::getContent() const {
    return this->content;
}

void Forum::setContent(const string& content) {
    this->content = content;
}

time_t Forum::getCreatedAt() const {
    return this->createdAt;
}

vector<int> Forum::getReplyForumsId() const
{
    return this->replyForumsId;
}

void Forum::addReplyForumId(const int& replyForumId)
{
    this->replyForumsId.push_back(replyForumId);
}

void Forum::removeReplyForumId(const int& replyForumId)
{
    this->replyForumsId.erase(remove(this->replyForumsId.begin(), this->replyForumsId.end(), replyForumId), this->replyForumsId.end());
}


const int Forum::getParentForumId() const
{
    return this->parentForumId;
}

void Forum::setParentForumId(const int& newParentForumId)
{
    this->parentForumId = newParentForumId;
}

// Define operator== for comparison
bool Forum::operator==(const Forum& other) const
{
    return this->id == other.id;
}

Forum::~Forum() {
}