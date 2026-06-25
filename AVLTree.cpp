#include "Common.hpp"
#include "AVLTree.hpp"

int AVLTree::height(TreeNode *n)
{
    if (n)
        return n->height;
    return -1;
}

int AVLTree::GetBalance(TreeNode *node)
{
    return height(node->left) - height(node->right);
}
TreeNode *AVLTree::InsertHelper(TreeNode *node, string const &PostContent, time_t newTime)
{
    auto comp = [&](time_t t1, time_t t2)
    {
        if (t1 == t2)
        {
            if (node)
                return node->PostContent < PostContent;
            else
                return true;
        }
        return t1 > t2;
    };
    if (node == nullptr)
        return new TreeNode(PostContent, newTime);

    if (comp(newTime, node->timestamp))
        node->left = InsertHelper(node->left, PostContent, newTime);
    else
        node->right = InsertHelper(node->right, PostContent, newTime);
    
    node->height = 1 + max(height(node->left), height(node->right));
    int balance = GetBalance(node);

    if (balance > 1 && comp(newTime, node->left->timestamp))
        return RightRotateHelper(node);
    if (balance < -1 && comp(newTime, node->right->timestamp))
        return LeftRotateHelper(node);
    if (balance > 1 && !comp(newTime, node->left->timestamp))
    {
        node->left = LeftRotateHelper(node->left);
        return RightRotateHelper(node);
    }
    if (balance < -1 && !comp(newTime, node->right->timestamp))
    {
        node->right = RightRotateHelper(node->right);
        return LeftRotateHelper(node);
    }

    return node;
}

TreeNode *AVLTree::LeftRotateHelper(TreeNode *x)
{
    TreeNode *y = x->right;
    TreeNode *T2 = y->left;
    y->left = x;
    x->right = T2;
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;
    return y;
}

TreeNode *AVLTree::RightRotateHelper(TreeNode *y)
{
    TreeNode *x = y->left;
    TreeNode *T2 = x->right;
    x->right = y;
    y->left = T2;
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;
    return x;
}
void AVLTree::GetRecentPosts(int &postCount, vector<string> &contentList, TreeNode *node) const
{
    if (postCount <= 0)
        return;
    if (!node)
        return;
    GetRecentPosts(postCount, contentList, node->left);
    if (postCount <= 0)
        return;
    postCount--;
    contentList.push_back(node->PostContent);
    if (postCount <= 0)
        return;
    GetRecentPosts(postCount, contentList, node->right);
}

void AVLTree::GetAllPosts(vector<string> &contentList, TreeNode *node) const
{
    if (!node)
        return;
    GetAllPosts(contentList, node->left);
    contentList.push_back(node->PostContent);
    GetAllPosts(contentList, node->right);
}

TreeNode *AVLTree::GetRoot() const
{
    return root;
}

void AVLTree::Insert(string const &PostContent, time_t newTime)
{
    root = InsertHelper(root, PostContent, newTime);
}

void AVLTree::Clear()
{
    DeleteTree(root);
    root = nullptr;
}

void AVLTree::DeleteTree(TreeNode* node)
{
    if (!node)
        return;

    DeleteTree(node->left);
    DeleteTree(node->right);

    delete node;
}

TreeNode* AVLTree::FindPost(TreeNode* node, const string& postContent)
{
    if (!node)
    return nullptr;
    if (node->PostContent == postContent)
        return node;
    TreeNode* left = FindPost(node->left, postContent);

    if (left)
        return left;
    return FindPost(node->right, postContent);
}

bool AVLTree::LikePost(const string& postContent)
{
    TreeNode* node = FindPost(root, postContent);

    if (!node)
        return false;

    node->likes++;

    return true;
}

bool AVLTree::UnlikePost(const string& postContent)
{
    TreeNode* node = FindPost(root, postContent);

    if (!node)
        return false;

    node->likes--;

    return true;
}

int AVLTree::GetLikes(const string& postContent)
{
    TreeNode* node = FindPost(root, postContent);
    if (!node)
        return 0;
    return node->likes;
}
