#include "SocialNetwork.hpp"

int main()
{
    SocialNetwork sn;
    string command;

    auto toLower = [](string &s)
    {
        transform(s.begin(), s.end(), s.begin(), ::tolower);
    };

    while (cin >> command)
    {
        if (command == "ADD_USER")
        {
            string username;
            cin >> username;
            toLower(username);
            sn.AddUser(username);
        }
        else if (command == "ADD_FRIEND")
        {
            string user1, user2;
            cin >> user1 >> user2;
            toLower(user1);
            toLower(user2);
            sn.AddFriend(user1, user2);
        }
        else if (command == "ADD_POST")
        {
            string username;
            cin >> username;
            string content;
            toLower(username);
            getline(cin >> ws, content);
            sn.AddPost(username, content);
        }
        else if (command == "LIST_FRIENDS")
        {
            string username;
            cin >> username;
            toLower(username);
            sn.ListFriends(username);
        }
        else if (command == "SUGGEST_FRIENDS")
        {
            string username;
            int n;
            cin >> username >> n;
            toLower(username);
            sn.SuggestFriends(username, n);
        }
        else if (command == "DEGREES_OF_SEPARATION")
        {
            string user1, user2;
            cin >> user1 >> user2;
            toLower(user1);
            toLower(user2);
            sn.DegreesOfSeparation(user1, user2);
        }
        else if (command == "OUTPUT_POSTS")
        {
            string username;
            int postCount;
            cin >> username >> postCount;
            toLower(username);
            sn.OutputPosts(username, postCount);
        }
        else if (command == "EXIT")
        {
            cout << "Exiting...\n";
            break;
        }
        else
        {
            cout << "Unknown command\n";
        }
        this_thread::sleep_for(chrono::seconds(1));
    }
}
