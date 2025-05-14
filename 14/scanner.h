#include <string>
#include <sstream>

class ScanSequence
{

private:
    const std::string seq;
    size_t len;
    size_t cur;

    bool isAtEnd();

    char peek();

    bool consume();

    char pop();

    void advance();

public:
    ScanSequence(std::string str);

    // Scan and produce a string, ending with the 'endChar' delimiter
    std::string scanString(const char &endChar);

    // Scan past a given 'endChar', producing nothing
    void skipUntilPast(const char &endChar);

    // Scan and produce an Int. Ends at any non digit character
    int scanInt();

    // Consume a given sequence. Returns false if that sequence doesn't exist at the current position.
    // Sets seq.cur past the consumed sequence
    bool consumeString(const std::string &string);
};

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
