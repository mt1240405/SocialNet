#include "SocialNetwork.hpp"
#include "CommandJournal.hpp"

void SocialNetwork::AddUser(const string &username)
{
    if (UserList.find(username) != UserList.end())
    {
        cout << "Username already exists\n";
        return;
    }
    if(!isReplay)
        CommandJournal::Append("ADD_USER " + username);
    int id = UserList.size();
    UserList[username] = id;
    graph.AddUser(id, username);
}

void SocialNetwork::AddFriend(const string &user1, const string &user2)
{
    if (graph.getUserIndex(user1) < 0)
        return;
    if (graph.getUserIndex(user2) < 0)
        return;
    if(user1 == user2) {
        cout << "Friendships must be between different users\n";
        return;
    }
    if(!isReplay)
            CommandJournal::Append("ADD_FRIEND " + user1 + " " + user2);
    graph.AddFriendship(UserList[user1], UserList[user2]);
}

void SocialNetwork::AddPost(const string &username, const string &content)
{
    if (graph.getUserIndex(username) < 0)
        return;
    time_t postCreated = time(nullptr);
    UserContentList[username].Insert(content, postCreated);
    if(!isReplay)
            CommandJournal::Append("ADD_POST " + username + " " + content);
}

void SocialNetwork::ListFriends(const string &username) const
{
    if (graph.getUserIndex(username) < 0)
        return;
    graph.SortedFriendsList(UserList.at(username));
}

void SocialNetwork::SuggestFriends(const string &username, int friendCount) const
{
    if (graph.getUserIndex(username) < 0)
        return;
    graph.SuggestMutualFriends(UserList.at(username), friendCount);
}

void SocialNetwork::DegreesOfSeparation(const string &user1, const string &user2) const
{
    if (graph.getUserIndex(user1) < 0)
        return;
    if (graph.getUserIndex(user2) < 0)
        return;
    cout << graph.shortestPathLength(UserList.at(user1), UserList.at(user2)) << "\n";
}

void SocialNetwork::OutputPosts(const string &username, int postCount) const
{
    if (graph.getUserIndex(username) < 0)
        return;
    vector<string> content_list;
    if (!(postCount == -1 || postCount > 0))
    {
        cout << "Invalid post count\n";
        return;
    }
    auto it = UserContentList.find(username);
    if (it == UserContentList.end() || it->second.GetRoot() == nullptr)
    {
        cout << "No posts found for user\n";
        return;
    }
    TreeNode* root = UserContentList.at(username).GetRoot();
    if (postCount == -1)
    {
        UserContentList.at(username).GetAllPosts(content_list, root);
    }
    if (postCount > 0)
        UserContentList.at(username).GetRecentPosts(postCount, content_list, root);
    for (const auto &post : content_list)
        cout << post << '\n';
}

void SocialNetwork::Clear()
{
    graph.Clear();
    UserList.clear();
    for (auto& tree : UserContentList)
        tree.second.Clear();

    UserContentList.clear();
}

void SocialNetwork::LikePost(const string& username, const string& postContent)
{
    if (graph.getUserIndex(username) < 0)
        return;
    if (!UserContentList[username].LikePost(postContent))
        cout << "Post not found\n";
}

void SocialNetwork::UnlikePost(const string& username, const string& postContent)
{
    if (graph.getUserIndex(username) < 0)
        return;
    if (!UserContentList[username].UnlikePost(postContent))
        cout << "Post not found\n";
}
