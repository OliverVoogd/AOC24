#include "scanner.h"

#include <string>
#include <sstream>

bool ScanSequence::isAtEnd()
{
    return cur >= len;
}

char ScanSequence::peek()
{
    return seq[cur];
}

bool ScanSequence::consume()
{
    if (isAtEnd())
        return false;
    cur++;
    return true;
}

char ScanSequence::pop()
{
    return seq[cur++];
}

void ScanSequence::advance()
{
    cur++;
}

static bool isDigit(const char &c)
{
    return (c >= '0' && c <= '9');
}

ScanSequence::ScanSequence(std::string str) : seq(str)
{
    cur = 0;
    len = str.size();
}

std::string ScanSequence::scanString(const char &endChar)
{
    std::stringstream ss;
    while (!isAtEnd() && peek() != endChar)
    {
        ss << pop();
    }
    return ss.str();
}

void ScanSequence::skipUntilPast(const char &endChar)
{
    while (!isAtEnd() && peek() != endChar)
    {
        advance();
    }
}

int ScanSequence::scanInt()
{
    std::stringstream ss;
    if (peek() == '-')
    {
        ss << pop();
    }

    while (isDigit(peek()))
    {
        ss << pop();
    }

    return stoi(ss.str());
}

bool ScanSequence::consumeString(const std::string &string)
{
    int cur_idx = 0;
    while (cur_idx < string.size())
    {
        if (isAtEnd())
        {
            return false;
        }
        if (seq[cur] != string[cur_idx])
        {
            return false;
        }

        advance();
        cur_idx++;
    }
    return true;
}