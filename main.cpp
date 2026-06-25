#include "SocialNetwork.hpp"
#include "CommandJournal.hpp"

void Run(SocialNetwork &sn)
{
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
        else if(command == "CLEAR")
        {
            sn.Clear();
        }
        else if (command == "LIKE_POST")
        {
            string username;
            cin >> username;
            string content;
            toLower(username);
            getline(cin >> ws, content);
            sn.LikePost(username, content);
        }
        else if (command == "UNLIKE_POST")
        {
            string username;
            cin >> username;
            string content;
            toLower(username);
            getline(cin >> ws, content);
            sn.UnlikePost(username, content);
        }
        else
        {
            cout << "Unknown command\n";
        }
        if(!sn.isReplay)
        this_thread::sleep_for(chrono::seconds(1));
    }
}

int main()
{
    SocialNetwork sn;
    // ios::sync_with_stdio(false);
    // cin.tie(nullptr);

    CommandJournal commandJournal;
    ifstream logFile("commands.log");
    auto *CinBuffer = cin.rdbuf();
    cin.rdbuf(logFile.rdbuf());
    sn.isReplay = true;

    Run(sn);
    cin.rdbuf(CinBuffer);
    sn.isReplay = false;
    Run(sn);
}
