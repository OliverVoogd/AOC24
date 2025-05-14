#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <regex>

#include "scanner.h"

using namespace std;

#define WIDTH 11 // 101
#define HEIGHT 7 // 103
// Top left is 0,0

typedef struct
{
    int x;
    int y;
} Vec2;

typedef struct
{
    Vec2 pos;
    const Vec2 vel;
} Robot;

// vector<Robot> parse_robots(const string &file)
// {
//     ifstream input(file);
//     string line;

//     input.close();
// }

int main()
{
    string file("input.txt");
    string test("p=0,4 v=3,-3");

    ScanSequence line(test);
    cout << line.consumeString("p=0,") << endl;
    cout << line.scanInt();

    return 0;
}