#ifndef FORUMSERVICE_H
#define FORUMSERVICE_H

#include <string>
#include <vector>
#include <map>
#include "../model/Forum.h"
#include "../model/ForumResponse.h"

using namespace std;

class ForumService {
private:
    const string dirPath = "../data";
    const string filePath = "../data/Forums.txt";

    void saveForums(const vector<Forum>& forums) const;
    vector<Forum> loadForums() const;
    vector<ForumResponse> getAllResponses(const map<int, ForumResponse>& dict);
    bool checkIfContain(const vector<int>& refIds, const int& targetId);
    int getLastId(const vector<Forum>& forums);
    ForumResponse buildTree(int forumId, const map<int, Forum>& forumById);
    vector<ForumResponse> constructForumResponse(const vector<Forum>& forums);
    vector<Forum> createForum(const string& creatorId, const string& content, int parentForumId);
    bool addForumReplyId(vector<Forum>& forums, const int& mainForumId) ;
    bool removeForumReplyId(vector<Forum> forums, const int& mainForumId, const int& forumId) ;
    
public:
    ForumService(); // New: Constructor to load Forums on startup

    void createForumAndSave(const string& creatorId, const string& content, int parentForumId = -1);
    void editForumContent(const string& requestUserId, const int& forumId, const string& newContent) const;
    void deleteForums(const string& requestUserId, const int& forumId);
    void replyToForum(const string& responseUserId, const string& content, const int& parentForumId);

    vector<ForumResponse> getAllForums();
    Forum getForumById(const int& forumId) const;
    vector<Forum> getAllForumsByUserId(const string& userId) const;
    vector<Forum> getAllForumsByTime(time_t timeStamp) const;

    ~ForumService();
};

#endif // FORUMSERVICE_H