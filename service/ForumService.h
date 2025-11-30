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
    static bool checkIfContain(const vector<int>& refIds, const int& targetId) ;
    static vector<ForumResponse> getAllResponses(const map<int, ForumResponse>& dict);
    static ForumResponse buildTree(int forumId, const map<int, Forum>& forumById);
    vector<Forum> createForum(const string& creatorId, const string& content, int parentForumId) const;
    static vector<ForumResponse> constructForums(const vector<Forum>& forums) ;
    static bool addForumReplyId(vector<Forum> forums, const int& mainForumId) ;
    static bool removeForumReplyId(vector<Forum> forums, const int& mainForumId, const int& forumId) ;
    
public:
    ForumService(); // New: Constructor to load Forums on startup

    void createForumAndSave(const string& creatorId, const string& content, int parentForumId = -1) const;
    void editForumContent(const string& requestUserId, const int& forumId, const string& newContent) const;
    void deleteForums(const string& requestUserId, const int& forumId) const;
    void replyToForum(const string& responseUserId, const string& content, const int& parentForumId) const;

    vector<ForumResponse> getAllForums() const;
    Forum getForumById(const int& forumId) const;
    vector<Forum> getAllForumsByUserId(const string& userId) const;
    vector<Forum> getAllForumsByTime(time_t timeStamp) const;

    ~ForumService();
};

#endif // FORUMSERVICE_H