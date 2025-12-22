#pragma once
#include <string>
#include "avl.hpp"
struct user
{
    std::string username;
    int associated_vertex;
    tree_node* posts_tree;
    user(const std::string &name, int vertex) : username(name), associated_vertex(vertex),posts_tree(nullptr) {} 
};
