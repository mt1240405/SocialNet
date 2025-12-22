/*
 * Name: Tanmay Modi
 * Entry No. : 2024MT10405
 * Assignment: SocialNet
 */

#include "header.hpp"

int main()
{
    // std::ifstream fin("input.txt");
    // std::ofstream fout("output.txt");

    // if (!fin.is_open() || !fout.is_open())
    // {
    //     std::cerr << "Error: could not open input.txt or output.txt\n";
    //     return 1;
    // }

    // std::cin.rdbuf(fin.rdbuf());
    // std::cout.rdbuf(fout.rdbuf());

    socialnet sn;
    std::string command;
    auto to_lower = [](std::string &s)
    {
        std::transform(s.begin(), s.end(), s.begin(), ::tolower);
    };
    while (std::cin >> command)
    {
        if (command == "ADD_USER")
        {
            std::string username;
            std::cin >> username;
            to_lower(username);
            sn.add_user(username);
        }
        else if (command == "ADD_FRIEND")
        {
            std::string u1, u2;
            std::cin >> u1 >> u2;
            to_lower(u1);
            to_lower(u2);
            sn.add_friend(u1, u2);
        }
        else if (command == "ADD_POST")
        {
            std::string username;
            std::cin >> username;
            std::string content;
            to_lower(username);
            std::getline(std::cin >> std::ws, content);
            sn.add_post(username, content);
        }
        else if (command == "LIST_FRIENDS")
        {
            std::string username;
            std::cin >> username;
            to_lower(username);
            sn.list_friends(username);
        }
        else if (command == "SUGGEST_FRIENDS")
        {
            std::string username;
            int n;
            std::cin >> username >> n;
            to_lower(username);
            sn.suggest_friends(username, n);
        }
        else if (command == "DEGREES_OF_SEPARATION")
        {
            std::string u1, u2;
            std::cin >> u1 >> u2;
            to_lower(u1);
            to_lower(u2);
            sn.degrees_of_separation(u1, u2);
        }
        else if (command == "OUTPUT_POSTS")
        {
            std::string username;
            int n;
            std::cin >> username >> n;
            to_lower(username);
            sn.output_posts(username, n);
        }
        else if (command == "EXIT")
        {
            std::cout << "Exiting...\n";
            break;
        }
        else
        {
            std::cout << "Unknown command\n";
        }
    }
    // fin.close();
    // fout.close();
}
