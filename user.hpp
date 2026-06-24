#include "Common.hpp"
#include "AVLTree.hpp"

struct User
{
    std::string username;
    int AssociatedVertex;
    TreeNode* postsTree;
    User(const std::string &name, int vertex) : username(name), AssociatedVertex(vertex),postsTree(nullptr) {} 
};
