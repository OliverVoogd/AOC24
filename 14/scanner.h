#include <string>

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
