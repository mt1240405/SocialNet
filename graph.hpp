#pragma once
#include "Common.hpp"
#include "User.hpp"

struct PairHash
{
    size_t operator()(
        const pair<int,int>& p) const
    {
        return hash<int>()(p.first)
             ^ (hash<int>()(p.second) << 1);
    }
};

class Graph
{
private:
    vector<User *> NodeList;
    vector<vector<int>> AdjacencyList;
    unordered_map<string, int> usernameToId;
    unordered_set <pair<int,int>,PairHash> edgeList;
public:
    void AddUser(int userId, const string &username);       // add a User
    void AddFriendship(int userId1, int userId2);             // add friends
    void SortedFriendsList(int userId) const;         // for all friends lst
    void SuggestMutualFriends(int userId, int friendCount) const;        // single layer bfs/dfs for mutal friends
    int shortestPathLength(int userId1, int userId2) const;      // find the DOS
    User *getUser(int id) const;                          // get User pointer by id
    int getUserIndex(const string &username) const; // find vertex index by username
    void Clear();
};
