#pragma once
#include "Common.hpp"
#include "Graph.hpp"
#include "AVLTree.hpp"
#include "User.hpp"

class SocialNetwork
{
private:
    Graph graph;
    unordered_map<string, int> UserList;
    unordered_map<string, AVLTree> UserContentList;
public: //As per given in requirements
    void AddUser(const string &username);
    void AddFriend(const string &user1, const string &user2);
    void AddPost(const string &username, const string &content);
    void ListFriends(const string &username) const;
    void SuggestFriends(const string &username, int friendCount) const;
    void DegreesOfSeparation(const string &user1, const string &user2) const;
    void OutputPosts(const string &username, int postCount) const;
    void Clear();
    bool isReplay;
};
