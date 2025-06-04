#ifndef SCANNER_H
#define SCANNER_H

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <functional>
#include <stdint.h>

class ScanSequence
{

private:
    std::string seq;
    size_t len;
    size_t cur;

    bool isAtEnd();

    char peek();

    bool consume();

    char pop();

    void advance();

public:
    ScanSequence(std::string str);
    ScanSequence(std::ifstream &file);

    char Peek()
    {
        return peek();
    }
    // Scan and produce a string, ending with the 'endChar' delimiter
    std::string scanString(const char &endChar);

    // Scan past a given 'endChar', producing nothing
    void skipUntilPast(const char &endChar);

    // Scan and produce an Int. Ends at any non digit character
    int scanInt();
    uint8_t scanUInt8();

    // Consume a given sequence. Returns false if that sequence doesn't exist at the current position.
    // Sets seq.cur past the consumed sequence
    bool consumeString(const char &string);
    bool consumeString(const std::string &string);

    void consumeWhitespace();

    void applyWithDelim(const char &delim, std::function<void(ScanSequence &)> apply);
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
    cur++;
    return true;
}

char ScanSequence::pop()
{
    return seq[cur++];
}

void ScanSequence::advance()
{
    if (isAtEnd())
        return;
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

ScanSequence::ScanSequence(std::ifstream &file)
{
    std::stringstream buffer;
    buffer << file.rdbuf();
    seq = buffer.str();
    cur = 0;
    len = seq.size();
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
    advance(); // step past the endChar
}

void ScanSequence::consumeWhitespace()
{
    while (!isAtEnd() && (peek() == ' ' || peek() == '\n'))
    {
        advance();
    }
}

int ScanSequence::scanInt()
{
    std::stringstream ss;
    consumeWhitespace();
    if (!isAtEnd() && peek() == '-')
    {
        ss << pop();
    }

    while (!isAtEnd() && isDigit(peek()))
    {
        ss << pop();
    }

    return stoi(ss.str());
}

uint8_t ScanSequence::scanUInt8()
{
    consumeWhitespace();
    uint8_t val = 0;
    while (!isAtEnd() && isDigit(peek()))
    {
        val = (val * 10) + (pop() - '0');
    }
    return val;
}

bool ScanSequence::consumeString(const char &string)
{
    return consumeString(std::string(1, string));
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

void ScanSequence::applyWithDelim(const char &delim, std::function<void(ScanSequence &)> apply)
{
    while (!isAtEnd())
    {
        apply(*this);
        if (!consumeString(delim))
            return;
    }
}

#endif