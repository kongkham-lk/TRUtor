#include "ForumService.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <filesystem>
#include <vector>
#include <map>

#include "../model/ForumResponse.h"
#include "../main.cpp"

using namespace std;
using namespace std::filesystem;

ForumService::ForumService() = default;

void ForumService::saveForums(const vector<Forum>& forums) const {
    // Check if the directory already exists
    if (!exists(dirPath) || !is_directory(dirPath)) {
        // Attempt to create the directory
        if (create_directory(dirPath)) {
            // cout << "Directory created successfully: " << dirPath << endl;
        } else {
            cerr << "Failed to create directory: " << dirPath << endl;
            return;
        }
    }

    ofstream file(filePath);
    for (const Forum& newForum : forums) {

        vector<int> replyForumsIds = newForum.getReplyForumsId();
        string replyForumsIdsStr = "_";
        for (const int replyForumsId : replyForumsIds)
            replyForumsIdsStr += to_string(replyForumsId) + "_";

        string content = newForum.getContent();
        ranges::replace(content, ' ', '_');

        file << newForum.getId() << " "
            << newForum.getCreatorId() << " "
            << content << " "
            << (long long) newForum.getCreatedAt() << " "
            << newForum.getParentForumId() << " "
            << replyForumsIdsStr << "\n";
    }
    file.close();
}

vector<Forum> ForumService::loadForums() const
{
    vector<Forum> forums;

    ifstream file(filePath);
    if (!file.is_open())
    {
        // cerr << "Failed to open file: " << filePath << endl;
        return {};
    }

    string line;
    while (getline(file, line)) {
        stringstream stream(line);
        int forumId;
        string creatorId;
        time_t timestamp;
        string content;
        int parentId;
        string replyForumsIdsStr;

        if (stream >> forumId >> creatorId >> content >> timestamp >> parentId >> replyForumsIdsStr)
        {
            ranges::replace(replyForumsIdsStr, '_', ' ');
            ranges::replace(content, '_', ' ');
            Forum newForum(forumId, creatorId, content, parentId, timestamp);

            int num;
            stringstream ss(replyForumsIdsStr);
            while (ss >> num) {
                // cout << "Push ReplyId: " << num << endl;
                newForum.addReplyForumId(int(num));
            }
            forums.push_back(newForum);
        }
    }
    file.close();

    // cout << endl << "Start Debugging...." << endl;
    // for (Forum newForum : forums)
    // {
    //     cout << newForum.getId() << endl;
    // }
    // cout << "End Debugging...." << endl;

    return forums;
}

bool ForumService::checkIfContain(const vector<int>& refIds, const int& targetId)
{
    return ranges::any_of(refIds, [&](int id){ return id == targetId; });
}

int ForumService::getLastId(const vector<Forum>& forums)
{
    int lastId = -1;
    for (const auto& f : forums)
        lastId = max(lastId, f.getId());
    return lastId;
}

vector<Forum> ForumService::createForum(const string& creatorId, const string& content, int parentForumId)
{
    // cout << endl << "Start creating new forum... " << endl;
    // cout << "Retrieving all Forum..." << endl;
    vector<Forum> forums = loadForums();

    int lastId = getLastId(forums);
    // cout << "Got lastId: " << lastId+1 << endl;

    // cout << "Creating new forum..." << endl;
    Forum newForum(lastId + 1, creatorId, content, parentForumId, 0);
    // cout << "new forum created, id: " << newForum.getId() << endl;
    forums.push_back(newForum);
    return forums;
}

void ForumService::createForumAndSave(const string& creatorId, const string& content, int parentForumId)
{
    vector<Forum> forums = createForum(creatorId, content, parentForumId);

    // cout << "Saving new forum..." << endl;
    saveForums(forums); // Save the new list to the file
}

void ForumService::editForumContent(const string& requestUserId, const int& forumId, const string& newContent) const
{
    if (vector<Forum> forums = getAllForumsByUserId(requestUserId); !forums.empty())
    {
        for (Forum& targetForum : forums)
        {
            if (targetForum.getId() == forumId)
            {
                targetForum.setContent(newContent);
                saveForums(forums);
                break;
            }
        }
    }
    else
        cout << "There is no forum to update!" << endl;
}

bool ForumService::addForumReplyId(vector<Forum>& forums, const int& parentForumId)
{
    // cout << endl << "Update replyId of ForumId: " << parentForumId << endl;
    if (!forums.empty())
    {
        int lastId = getLastId(forums);
        for (int i = 0; i < forums.size(); i++)
        {
            if (forums[i].getId() == parentForumId)
            {
                forums[i].addReplyForumId(lastId);

                // cout << endl << "Start Debugging...." << endl;
                // logForumForDebug(forums[i]);
                // cout << "End Debugging...." << endl;
                return true;
            }
        }
    }
    // cout << "Fail to Update! " << endl;
    return false;
}

void ForumService::removeForumReplyId(vector<Forum>& forums, const Forum& targetForum, const vector<int>& toRemoveIds)
{
    int parentId = targetForum.getParentForumId();
    if (parentId != -1) {
        for (auto& f : forums) {
            if (f.getId() == parentId) {
                for (int id : toRemoveIds)
                    f.removeReplyForumId(id);
            }
        }
    }
}

void ForumService::collectAllDescendants(int forumId, const map<int, Forum>& forumById, vector<int>& toRemove)
{
    toRemove.push_back(forumId);

    const Forum& f = forumById.at(forumId);
    for (int childId : f.getReplyForumsId()) {
        if (forumById.contains(childId)) {
            collectAllDescendants(childId, forumById, toRemove);
        }
    }
}

void ForumService::deleteForums(const string& requestUserId, const int& forumId)
{
    if (Forum targetForum = getForumById(forumId); targetForum.getId() == -1)
        cerr << "Fail to delete, the request forum with ID does not exist!" << endl;
    else if (targetForum.getCreatorId() != requestUserId)
        cerr << "Fail to delete, the forum is you created by you!" << endl;
    else
    {
        if (vector<Forum> forums = loadForums(); !forums.empty())
        {
            // Build dictionary
            map<int, Forum> forumById;
            for (const auto& f : forums)
                forumById[f.getId()] = f;

            // Get all descendant reply forum, recursively
            vector<int> toRemoveIds;
            collectAllDescendants(forumId, forumById, toRemoveIds);

            // Remove all descendant reply forum from the list
            forums.erase(
                remove_if(
                    forums.begin(),
                    forums.end(),
                    [&](const Forum& f) {
                        return checkIfContain(toRemoveIds, f.getId());
                    }),
                forums.end()
            );

            // Remove forumId from parent's replyForumsId
            removeForumReplyId(forums, targetForum, toRemoveIds);
            saveForums(forums);
        }
        else
            cout << "Currently there is no forum!" << endl;
    }
}

void ForumService::replyToForum(const string& responseUserId, const string& content, const int& parentForumId)
{
    // create a new reply forum and push to list (Not save to file yet!)
    vector<Forum> forums = createForum(responseUserId, content, parentForumId);

    // update its parentForum's replyForumId list
    if (addForumReplyId(forums, parentForumId))
    {
        // logAllForumForDebug(forums); // for debugging
        saveForums(forums);
    }
    else
        cerr << "The target forum that you attempt to reply is Invalid!" << endl;
}

vector<ForumResponse> ForumService::getAllResponses(const map<int, ForumResponse>& forumResponseDict) {
    vector<ForumResponse> result;
    result.reserve(forumResponseDict.size());
    for (const auto& forumResponse : forumResponseDict)
        result.push_back(forumResponse.second); // only use the value of the dict

    return result;
}


ForumResponse ForumService::buildTree(const int forumId, const map<int, Forum>& forumById)
{
    // cout << "Lookup ForumById: " << forumId << endl;
    const Forum& rootForum = forumById.at(forumId);
    // cout << "Found: " << rootForum.getId() << endl;

    ForumResponse resp(rootForum); // root node
    const vector<int>& children = rootForum.getReplyForumsId();

    // cout << "roorID: " << rootForum.getId();

    for (int childId : children) {
        if (forumById.contains(childId)) {
            // cout << " -> " << childId << endl;
            resp.addReply(buildTree(childId, forumById));  // recursion
        }
    }
    return resp;
}


vector<ForumResponse> ForumService::constructForumResponse(const vector<Forum>& forums)
{
    // cout << "constructForumResponse..." << endl;
    map<int, Forum> forumById;
    for (const Forum& f : forums)
        forumById[f.getId()] = f;

    vector<ForumResponse> roots;

    // cout << "buildTree..." << endl;
    for (const Forum& f : forums) {
        if (f.getParentForumId() == -1) {  // root forum
            roots.push_back(buildTree(f.getId(), forumById));
        }
    }

    return roots;
}

vector<ForumResponse> ForumService::getAllForums()
{
    // cout << "getAllForums..." << endl;
    return constructForumResponse(loadForums());
}

Forum ForumService::getForumById(const int& forumId) const
{
    if (vector<Forum> forums = loadForums(); !forums.empty())
        for (const Forum& forum : forums)
            if (forum.getId() == forumId)
                return forum;
    return {};
}

vector<Forum> ForumService::getAllForumsByUserId(const string& userId) const
{
    if (vector<Forum> forums = loadForums(); !forums.empty())
    {
        vector<Forum> result;
        for (const auto& forum : forums)
            if (forum.getCreatorId() == userId)
                result.push_back(forum);
        return result;
    }
    else
        return {};
}

vector<Forum> ForumService::getAllForumsByTime(time_t timeStamp) const
{
    if (vector<Forum> forums = loadForums(); !forums.empty())
    {
        vector<Forum> result;
        for (const auto& forum : forums)
            if (forum.getCreatedAt() == timeStamp)
                result.push_back(forum);
        return result;
    }
    else
        return {};
}

ForumService::~ForumService() = default;