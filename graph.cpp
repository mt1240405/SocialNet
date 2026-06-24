#include "Graph.hpp"

void Graph::AddUser(int UserId, const string &username)
{
    if (usernameToId.find(username) != usernameToId.end()) {
        cout << "Username already exists\n";
        return;
    }
    User *user = new User(username, UserId);
    NodeList.push_back(user);
    AdjacencyList.push_back({});
    usernameToId[username] = UserId;
}

void Graph::AddFriendship(int userId1, int userId2)
{
    if(userId1 > userId2)
    {
        swap(userId1,userId2);
    }
    if(edgeList.count({userId1,userId2}))
    {
        cout << "Friendship exists!" <<endl;
        return;
    }
    edgeList.insert({userId1,userId2});
    AdjacencyList[userId1].push_back(userId2);
    AdjacencyList[userId2].push_back(userId1);
}

void Graph::SortedFriendsList(int userId) const
{
    vector<string> friendList;
    for (int friendId : AdjacencyList[userId])
        friendList.push_back(NodeList[friendId]->username);

    sort(friendList.begin(), friendList.end());

    for (const auto &name : friendList)
        cout << name << '\n';
}

void Graph::SuggestMutualFriends(int userId, int friendCount) const
{
    if(friendCount <= 0)
        return;
    vector<bool> visited(NodeList.size(), false);
    visited[userId] = true;
    vector<pair<int, int>> mutualCount(NodeList.size(), {0, -1});
    for (int friendId : AdjacencyList[userId])
        visited[friendId] = true;
    visited[userId] = true;

    for (int friendId : AdjacencyList[userId])
    {
        for (int friendOfFriend : AdjacencyList[friendId])
        {
            if (visited[friendOfFriend])
                continue;
            mutualCount[friendOfFriend].first++;
            mutualCount[friendOfFriend].second = friendOfFriend;
        }
    }

    sort(mutualCount.begin(), mutualCount.end(),
              [this](const pair<int, int> &p1, const pair<int, int> &p2)
              {
                  if (p1.second == -1)
                      return false;
                  if (p2.second == -1)
                      return true;
                  if (p1.first != p2.first)
                      return p1.first > p2.first;
                  return NodeList[p1.second]->username < NodeList[p2.second]->username;
              });

    int count = 0;
    for (auto &recommendation : mutualCount) //first is mutual count, second is friend id
    {
        if (recommendation.first == 0 || recommendation.second == -1)
            continue;
        cout << NodeList[recommendation.second]->username << "\n";
        count++;
        if (count == friendCount)
            break;
    }
}

int Graph::shortestPathLength(int userId1, int userId2) const
{
    if (userId1 == userId2)
        return 0;
    vector<bool> visited(NodeList.size(), false);
    visited[userId1] = true;

    queue<int> paths;
    paths.push(userId1);

    int sz = 0;
    bool flag = false;
    int depth = 0;

    while (!paths.empty())
    {
        sz = paths.size();
        while (sz--)
        {
            int cur = paths.front();
            paths.pop();

            if (cur == userId2)
            {
                flag = true;
                break;
            }

            for (int friendId : AdjacencyList[cur])
            {
                if (visited[friendId])
                    continue;
                visited[friendId] = true;
                paths.push(friendId);
            }
        }
        if (flag)
            break;
        depth++;
    }
    if (flag)
        return depth;
    else
        return -1;
}

User *Graph::getUser(int id) const //valid already checked in getUserIndex
{
    return NodeList[id];
}
int Graph::getUserIndex(const string &username) const
{
    auto it = usernameToId.find(username);
    if (it != usernameToId.end())
        return it->second;
    cout << "Invalid Username\n";
    return -1;
}

