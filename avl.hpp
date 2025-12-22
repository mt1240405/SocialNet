#pragma once
#include <string>
#include <vector>
#include <ctime>
struct tree_node
{
    std::string post_content;
    tree_node *left;
    tree_node *right;
    int height;
    time_t timestamp;
    tree_node(std::string const &content, time_t new_time) : post_content(content), left(nullptr), right(nullptr), height(0), timestamp(new_time) {}
};

class AVL_tree
{
private:
    tree_node *root;

public:
    AVL_tree() : root(nullptr) {}
    tree_node *insert(tree_node *node, std::string const &post_content, time_t new_time); // insert a new post
    tree_node *left_rotate_helper(tree_node *x);                                          // left rotation avl helper
    tree_node *right_rotate_helper(tree_node *y);                                         // right rotation avl helper
    void search(int &k, std::vector<std::string> &content_list, tree_node *node) const;   // searches first k queries by time
    void search_all(std::vector<std::string> &content_list, tree_node *node) const;
    int height(tree_node *node);
    int getBalance(tree_node *node);
    tree_node *get_root() const;
    void real_insert(std::string const &post_content, time_t new_time);
};
