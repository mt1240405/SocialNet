#pragma once
#include <string>
#include <vector>
#include <ctime>
struct TreeNode
{
    string PostContent;
    TreeNode *left;
    TreeNode *right;
    int height;
    time_t timestamp;
    TreeNode(string const &content, time_t newTime) : PostContent(content), left(nullptr), right(nullptr), height(0), timestamp(newTime) {}
};

class AVLTree
{
private:
    TreeNode *root;

public:
    AVLTree() : root(nullptr) {}
    TreeNode *InsertHelper(TreeNode *node, string const &PostContent, time_t newTime); // Insert a new post
    TreeNode *LeftRotateHelper(TreeNode *x);                                          // left rotation avl helper
    TreeNode *RightRotateHelper(TreeNode *y);                                         // right rotation avl helper
    void GetRecentPosts(int &postCount, vector<string> &contentList, TreeNode *node) const;   // searches first postCount queries by time
    void GetAllPosts(vector<string> &contentList, TreeNode *node) const;
    int height(TreeNode *node);
    int GetBalance(TreeNode *node);
    TreeNode *GetRoot() const;
    void Insert(string const &PostContent, time_t newTime);
};
