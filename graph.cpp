#include "header.hpp"


void Graph::insert(int n, const std::string &username)
{
    if (username_to_id.find(username) != username_to_id.end()) {
        std::cout << "Username already exists\n";
        return;
    }
    user *k = new user(username, n);
    node_list.push_back(k);
    adjacency_list.push_back({});
    username_to_id[username] = n;
}

void Graph::add_edge(int user_id1, int user_id2)
{
    adjacency_list[user_id1].push_back(user_id2);
    adjacency_list[user_id2].push_back(user_id1);
}

void Graph::sorted_adjacency_list(int user_id) const
{
    std::vector<std::string> friend_list;
    for (int friend_id : adjacency_list[user_id])
        friend_list.push_back(node_list[friend_id]->username);
    std::sort(friend_list.begin(), friend_list.end());
    for (const auto &name : friend_list)
        std::cout << name << '\n';
}

void Graph::edge_of_edges(int user_id, int numf) const
{
    if(numf <= 0)
        return;
    std::vector<bool> visited(node_list.size(), false);
    visited[user_id] = true;
    std::vector<std::pair<int, int>> mutual_count(node_list.size(), {0, -1});
    for (int friend_id : adjacency_list[user_id])
        visited[friend_id] = true;
    visited[user_id] = true;

    for (int friend_id : adjacency_list[user_id])
    {
        for (int friend_of_friend : adjacency_list[friend_id])
        {
            if (visited[friend_of_friend])
                continue;
            mutual_count[friend_of_friend].first++;
            mutual_count[friend_of_friend].second = friend_of_friend;
        }
    }

    std::sort(mutual_count.begin(), mutual_count.end(),
              [this](const std::pair<int, int> &p1, const std::pair<int, int> &p2)
              {
                  if (p1.second == -1)
                      return false;
                  if (p2.second == -1)
                      return true;
                  if (p1.first != p2.first)
                      return p1.first > p2.first;
                  return node_list[p1.second]->username < node_list[p2.second]->username;
              });

    int count = 0;
    for (auto &p : mutual_count)
    {
        if (p.first == 0 || p.second == -1)
            continue;
        std::cout << node_list[p.second]->username << "\n";
        count++;
        if (count == numf)
            break;
    }
}

int Graph::path_length(int user_id1, int user_id2) const
{
    if (user_id1 == user_id2)
        return 0;
    std::vector<bool> visited(node_list.size(), false);
    visited[user_id1] = true;
    std::queue<int> paths;
    paths.push(user_id1);
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
            if (cur == user_id2)
            {
                flag = true;
                break;
            }
            for (int friend_id : adjacency_list[cur])
            {
                if (visited[friend_id])
                    continue;
                visited[friend_id] = true;
                paths.push(friend_id);
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

user *Graph::get_user(int id) const
{
    return node_list[id];
}
int Graph::get_user_index(const std::string &username) const
{
    auto it = username_to_id.find(username);
    if (it != username_to_id.end())
        return it->second;
    std::cout << "Invalid Username\n";
    return -1;
}
