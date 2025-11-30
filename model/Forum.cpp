#include "Forum.h"
#include <string>

using namespace std;

Forum::Forum()
{

}

Forum::Forum(int id, const string& userId, const string& msg, time_t timestamp)
    : id(id), userId(userId), content(msg), createdAt(timestamp) {
}

const int Forum::getId() const {
    return this->id;
}

const string& Forum::getCreatorrId() const {
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

// Define operator== for comparison
bool Forum::operator==(const Forum& other) const
{
    return this->id == other.id;
}

Forum::~Forum() {
}