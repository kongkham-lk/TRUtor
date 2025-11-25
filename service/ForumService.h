#ifndef FORUMSERVICE_H
#define FORUMSERVICE_H

#include <string>
#include <list>
#include "../model/Forum.h"

using namespace std;

class ForumService {
private:
    const string dirPath = "../data";
    const string filePath = "../data/Forums.txt";

    void saveForums(const vector<Forum>& forums) const;
    
public:
    ForumService(); // New: Constructor to load Forums on startup

    void createForum(const string& creatorId, const string& content) const;
    void editForum(const string& requestUserId, int forumId, const string& newContent) const;
    void deleteForums(const string& requestUserId, int forumId) const;

    vector<Forum> getAllForums() const;
    vector<Forum> getAllForumsByUserId(const string& userId) const;
    vector<Forum> getAllForumsByTime(time_t timeStamp) const;

    ~ForumService();
};

#endif // FORUMSERVICE_H