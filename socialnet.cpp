#include "header.hpp"

void socialnet::add_user(const std::string &username)
{
    if (user_list.find(username) != user_list.end())
    {
        std::cout << "Username already exists\n";
        return;
    }
    int id = user_list.size();
    user_list[username] = id;
    g.insert(id, username);
}

void socialnet::add_friend(const std::string &u1, const std::string &u2)
{
    if (g.get_user_index(u1) < 0)
        return;
    if (g.get_user_index(u2) < 0)
        return;
    if(u1 == u2) {
        std::cout << "Friendships must be between different users\n";
        return;
    }
    g.add_edge(user_list[u1], user_list[u2]);
}

void socialnet::add_post(const std::string &username, const std::string &content)
{
    if (g.get_user_index(username) < 0)
        return;
    time_t t = time(nullptr);
    user_content_list[username].real_insert(content, t);
}

void socialnet::list_friends(const std::string &username) const
{
    if (g.get_user_index(username) < 0)
        return;
    g.sorted_adjacency_list(user_list.at(username));
}

void socialnet::suggest_friends(const std::string &username, int n) const
{
    if (g.get_user_index(username) < 0)
        return;
    g.edge_of_edges(user_list.at(username), n);
}

void socialnet::degrees_of_separation(const std::string &u1, const std::string &u2) const
{
    if (g.get_user_index(u1) < 0)
        return;
    if (g.get_user_index(u2) < 0)
        return;
    std::cout << g.path_length(user_list.at(u1), user_list.at(u2)) << "\n";
}

void socialnet::output_posts(const std::string &username, int n) const
{
    if (g.get_user_index(username) < 0)
        return;
    std::vector<std::string> content_list;
    if (!(n == -1 || n > 0))
    {
        std::cout << "Invalid post count\n";
        return;
    }
    auto it = user_content_list.find(username);
    if (it == user_content_list.end() || it->second.get_root() == nullptr)
    {
        std::cout << "No posts found for user\n";
        return;
    }
    if (n == -1)
    {
        user_content_list.at(username).search_all(content_list, user_content_list.at(username).get_root());
    }
    if (n > 0)
        user_content_list.at(username).search(n, content_list, user_content_list.at(username).get_root());
    for (const auto &post : content_list)
        std::cout << post << '\n';
}
