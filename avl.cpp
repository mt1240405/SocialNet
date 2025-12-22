#include "header.hpp"

int AVL_tree::height(tree_node *n)
{
    if (n)
        return n->height;
    return -1;
}

int AVL_tree::getBalance(tree_node *node)
{
    return height(node->left) - height(node->right);
}
tree_node *AVL_tree::insert(tree_node *node, std::string const &post_content, time_t new_time)
{
    auto comp = [&](time_t t1, time_t t2)
    {
        if (t1 == t2)
        {
            if (node)
                return node->post_content < post_content;
            else
                return true;
        }
        return t1 > t2;
    };
    if (node == nullptr)
        return new tree_node(post_content, new_time);
    if (comp(new_time, node->timestamp))
        node->left = insert(node->left, post_content, new_time);
    else
        node->right = insert(node->right, post_content, new_time);
    node->height = 1 + std::max(height(node->left), height(node->right));
    int balance = getBalance(node);
    if (balance > 1 && comp(new_time, node->left->timestamp))
        return right_rotate_helper(node);
    if (balance < -1 && comp(new_time, node->right->timestamp))
        return left_rotate_helper(node);
    if (balance > 1 && !comp(new_time, node->left->timestamp))
    {
        node->left = left_rotate_helper(node->left);
        return right_rotate_helper(node);
    }
    if (balance < -1 && !comp(new_time, node->right->timestamp))
    {
        node->right = right_rotate_helper(node->right);
        return left_rotate_helper(node);
    }
    return node;
}

tree_node *AVL_tree::left_rotate_helper(tree_node *x)
{
    tree_node *y = x->right;
    tree_node *T2 = y->left;
    y->left = x;
    x->right = T2;
    x->height = std::max(height(x->left), height(x->right)) + 1;
    y->height = std::max(height(y->left), height(y->right)) + 1;
    return y;
}

tree_node *AVL_tree::right_rotate_helper(tree_node *y)
{
    tree_node *x = y->left;
    tree_node *T2 = x->right;
    x->right = y;
    y->left = T2;
    y->height = std::max(height(y->left), height(y->right)) + 1;
    x->height = std::max(height(x->left), height(x->right)) + 1;
    return x;
}
void AVL_tree::search(int &k, std::vector<std::string> &content_list, tree_node *node) const
{
    if (k <= 0)
        return;
    if (!node)
        return;
    search(k, content_list, node->left);
    if (k <= 0)
        return;
    k--;
    content_list.push_back(node->post_content);
    if (k <= 0)
        return;
    search(k, content_list, node->right);
}

void AVL_tree::search_all(std::vector<std::string> &content_list, tree_node *node) const
{
    if (!node)
        return;
    search_all(content_list, node->left);
    content_list.push_back(node->post_content);
    search_all(content_list, node->right);
}

tree_node *AVL_tree::get_root() const
{
    return root;
}

void AVL_tree::real_insert(std::string const &post_content, time_t new_time)
{
    root = insert(root, post_content, new_time);
}
