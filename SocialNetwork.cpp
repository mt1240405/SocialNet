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
    CommandJournal::Clear();
    UserContentList.clear();
}

void SocialNetwork::LikePost(const string& username, const string& postContent)
{
    if (graph.getUserIndex(username) < 0)
        return;
    if (!UserContentList[username].LikePost(postContent))
    {
        cout << "Post not found\n";
        return;
    }
    if(!isReplay)
        CommandJournal::Append("LIKE_POST " + username + " " + postContent);
}

void SocialNetwork::UnlikePost(const string& username, const string& postContent)
{
    if (graph.getUserIndex(username) < 0)
        return;
    if (!UserContentList[username].UnlikePost(postContent))
    {
        cout << "Post not found\n";
        return;
    }
    if(!isReplay)
        CommandJournal::Append("UNLIKE_POST " + username + " " + postContent);
}

double SocialNetwork::FeedScore(TreeNode *post, double affinity) const
{
    double ageHours = difftime(time(nullptr), post->timestamp) / 3600.0;
    double recency = 1.0 / (1.0 + ageHours);
    return 8.0 * log2(post->likes + 1) + 2.0 * sqrt(affinity) + 3.0 * recency;
}

void SocialNetwork::GenerateUserFeed(const string &username, int feedSize) const
{
    if (graph.getUserIndex(username) < 0)
        return;
    if (feedSize <= 0)
        return;
    int userId = UserList.at(username);

    vector<FriendAffinity> friends;
    for (int friendId : graph.GetFriends(userId))
    {
        friends.push_back(
        {
            friendId,
            (double)(graph.MutualFriendCount(userId, friendId)+1.0)
        });
    }
    if (friends.empty())
    {
        cout << "Feed is empty\n";
        return;
    }
    sort(friends.begin(), friends.end(),
    [](const FriendAffinity &a, const FriendAffinity &b)
    {
        return a.affinity > b.affinity;
    });

    double totalAffinity = 0;
    for (const FriendAffinity &friendInfo : friends)
        totalAffinity += max(1.0, friendInfo.affinity);
    int candidateBudget = 10 * feedSize;

    vector<FeedPost> candidates;
    for (const FriendAffinity &friendInfo : friends)
    {
        int postsToTake = min(10,max(1,(int)((friendInfo.affinity / totalAffinity) * candidateBudget)));
        vector<TreeNode*> posts;

        auto it = UserContentList.find(graph.GetUsername(friendInfo.userId));
        if (it == UserContentList.end())
            continue;
        it->second.GetRecentPostNodes(postsToTake, posts, it->second.GetRoot());
        for (TreeNode *post : posts)
        {
            candidates.push_back(
            {
                post,
                friendInfo.userId,
                FeedScore(post, friendInfo.affinity)
            });
        }
    }

    sort(candidates.begin(), candidates.end(),
    [](const FeedPost &a, const FeedPost &b)
    {
        return a.score > b.score;
    });
    for (int i = 0; i < min(feedSize, (int)candidates.size()); i++)
    {
        cout << graph.GetUsername(candidates[i].authorId) << '\n';
        cout << candidates[i].post->PostContent << '\n';
        cout << "Likes: " << candidates[i].post->likes << "\n\n";
    }
}
