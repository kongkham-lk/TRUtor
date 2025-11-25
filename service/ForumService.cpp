#include "ForumService.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <filesystem>

using namespace std;
using namespace std::filesystem;

ForumService::ForumService()
{

}

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
        string content = newForum.getContent();
        ranges::replace(content, ' ', '_');

        file << newForum.getId() << " "
            << newForum.getUserId() << " "
            << (long long) newForum.getCreatedAt() << " "
            << content << "\n";
    }
    file.close();
}

void ForumService::createForum(const string& creatorId, const string& content) const
{
    // cout << "Start creating new forum... " << endl;
    // cout << "Retrieving all Forum..." << endl;
    vector<Forum> forums = getAllForums();

    // cout << "Get lastId..." << endl;
    int lastId = forums.empty() ? 0 : forums[forums.size()-1].getId();

    // cout << "Creating new forum..." << endl;
    const Forum newForum(lastId + 1, creatorId, content);
    forums.push_back(newForum);

    // cout << "Saving new forum..." << endl;
    saveForums(forums); // Save the new list to the file
}

void ForumService::editForum(const string& requestUserId, int forumId, const string& newContent) const
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
}

void ForumService::deleteForums(const string& requestUserId, int forumId) const
{
    if (vector<Forum> forums = getAllForumsByUserId(requestUserId); !forums.empty())
    {
        for (const auto targetForum = forums.begin(); targetForum != forums.end();)
        {
            if (targetForum->getId() == forumId)
            {
                forums.erase(targetForum);
                saveForums(forums);
                break;
            }
        }
    }
}

vector<Forum> ForumService::getAllForums() const
{
    vector<Forum> forums;

    ifstream file(filePath);
    if (!file.is_open())
    {
        // cerr << "Failed to open file: " << filePath << endl;
        return forums;
    }

    string line;
    while (getline(file, line)) {
        stringstream stream(line);
        int forumId;
        string userId;
        time_t timestamp;
        string content;

        if (stream >> forumId >> userId >> timestamp >> content) {
            ranges::replace(content, '_', ' ');
            Forum newForum(forumId, userId, content, timestamp);
            forums.push_back(newForum);
        }
    }
    file.close();

    return forums;
}

Forum ForumService::getForumById(const int& forumId) const
{
    if (vector<Forum> forums = getAllForums(); !forums.empty())
        for (const Forum& forum : forums)
            if (forum.getId() == forumId)
                return forum;
    else
        return Forum();
}

vector<Forum> ForumService::getAllForumsByUserId(const string& userId) const
{
    if (vector<Forum> forums = getAllForums(); !forums.empty())
    {
        vector<Forum> result;
        for (const auto& forum : forums)
            if (forum.getUserId() == userId)
                result.push_back(forum);
        return result;
    }
    else
        return forums;
}

vector<Forum> ForumService::getAllForumsByTime(time_t timeStamp) const
{
    if (vector<Forum> forums = getAllForums(); !forums.empty())
    {
        vector<Forum> result;
        for (const auto& forum : forums)
            if (forum.getCreatedAt() == timeStamp)
                result.push_back(forum);
        return result;
    }
    else
        return forums;
}

ForumService::~ForumService()
{

}