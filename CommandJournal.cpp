#include "CommandJournal.hpp"

void CommandJournal::Append(const string& commandLine)
{
    ofstream logFile("commands.log", ios::app);
    logFile << commandLine << '\n';
}

void CommandJournal::Clear()
{
    ofstream logFile("commands.log", ios::trunc);
}
