#pragma once
#include "graph.hpp"
#include "avl.hpp"
#include "user.hpp"
#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>

class socialnet
{
private:
    Graph g;
    std::unordered_map<std::string, int> user_list;
    std::unordered_map<std::string, AVL_tree> user_content_list;

public: //As per given in requirements
    void add_user(const std::string &username);
    void add_friend(const std::string &u1, const std::string &u2);
    void add_post(const std::string &username, const std::string &content);
    void list_friends(const std::string &username) const;
    void suggest_friends(const std::string &username, int n) const;
    void degrees_of_separation(const std::string &u1, const std::string &u2) const;
    void output_posts(const std::string &username, int n) const;
};
