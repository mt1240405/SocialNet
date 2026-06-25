#pragma once
#include "Common.hpp"

class CommandJournal
{
public:
    static void Append(const string& commandLine);
    static void Clear();
};
