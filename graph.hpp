#pragma once
#include "user.hpp"
#include <vector>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <queue>

class Graph
{
private:
    std::vector<user *> node_list;
    std::vector<std::vector<int>> adjacency_list;
    std::unordered_map<std::string, int> username_to_id;
public:
    void insert(int n, const std::string &username);       // add a user
    void add_edge(int user1_id, int user2_id);             // add friends
    void sorted_adjacency_list(int user_id) const;         // for all friends lst
    void edge_of_edges(int user_id, int numf) const;        // single layer bfs/dfs for mutal friends
    int path_length(int user_id1, int user_id2) const;      // find the DOS
    user *get_user(int id) const;                          // get user pointer by id
    int get_user_index(const std::string &username) const; // find vertex index by username
};
