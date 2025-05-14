#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>
#include <regex>
#include <cmath>

#include "scanner.h"

using namespace std;

#define WIDTH 101
#define HEIGHT 103
// Top left is 0,0

typedef struct
{
    int x;
    int y;
} Vec2;

typedef struct
{
    Vec2 pos;
    Vec2 vel;
} Robot;

// v: (2, -3)
// 0: (2, 4)
// 1: (4, 1)
// 2: (6, 5)
// 3: (8, 2)
// 4: (10, 6)
// 5: (1, 3) = ((2,4) + 5 * (2,-3)) mod (11, 7)

void print_robot(Robot r)
{
    cout << "<r> (" << r.pos.x << "," << r.pos.y << ") -> (" << r.vel.x << "," << r.vel.y << ")\n";
}

void visualise_robots(const vector<Robot> &robots)
{
    int grid[WIDTH][HEIGHT];
    for (int i = 0; i < WIDTH; i++)
    {
        for (int j = 0; j < HEIGHT; j++)
        {
            grid[i][j] = 0;
        }
    }

    std::for_each(robots.begin(), robots.end(), [&grid](const Robot &robot)
                  { grid[robot.pos.x][robot.pos.y]++; });

    for (int j = 0; j < HEIGHT; j++)
    {
        for (int i = 0; i < WIDTH; i++)
        {
            if (grid[i][j] == 0)
            {
                cout << '.';
            }
            else
            {
                cout << grid[i][j];
            }
        }
        cout << "\n";
    }
}

vector<Robot> parse_robots(const string &file)
{
    ifstream input(file);
    ScanSequence ss(input);
    input.close();

    vector<Robot> robots;

    ss.applyWithDelim('\n', [&robots](ScanSequence &ss)
                      {
        ss.consumeString("p=");
        int x = ss.scanInt();
        ss.consumeString(",");
        int y = ss.scanInt();
        ss.skipUntilPast('=');
        int vX = ss.scanInt();
        ss.consumeString(",");
        int vY = ss.scanInt();

        robots.push_back({{x, y}, {vX, vY}}); });

    return robots;
}

static int floor_mod(int a, int n)
{
    return a - (n * floor((float)a / (float)n));
}
Robot new_moved_robot(const Robot &robot, int secs)
{
    return {{floor_mod((robot.pos.x + secs * robot.vel.x), WIDTH), floor_mod((robot.pos.y + secs * robot.vel.y), HEIGHT)},
            {robot.vel.x, robot.vel.y}};
}
vector<Robot> move_robots_by_sec(const vector<Robot> &start, uint secs = 0)
{
    vector<Robot> end(start.size());

    transform(start.begin(), start.end(), end.begin(), [&secs](const Robot &robot)
              { return new_moved_robot(robot, secs); });
    return end;
}

typedef enum
{
    TOP_LEFT,
    TOP_RIGHT,
    BOTTOM_LEFT,
    BOTTOM_RIGHT,
    NONE
} Quadrent;

Quadrent robot_quadrent(const Robot &robot)
{
    // check for on the midpoints first.
    if (robot.pos.x == WIDTH / 2 || robot.pos.y == HEIGHT / 2)
    {
        return NONE;
    }

    if (robot.pos.x < WIDTH / 2)
    {
        if (robot.pos.y < HEIGHT / 2)
        {
            return TOP_LEFT;
        }
        return BOTTOM_LEFT;
    }
    else
    {
        if (robot.pos.y < HEIGHT / 2)
        {
            return TOP_RIGHT;
        }
        return BOTTOM_RIGHT;
    }
}

int calculate_safety_factor(const vector<Robot> &robots)
{
    int quads[5] = {0, 0, 0, 0, 0};

    for (auto r : robots)
    {
        quads[robot_quadrent(r)]++;
    }

    return quads[TOP_LEFT] * quads[TOP_RIGHT] * quads[BOTTOM_LEFT] * quads[BOTTOM_RIGHT];
}

int main()
{
    string file("input.txt");
    vector<Robot> robots = parse_robots(file);

    // for_each(robots.begin(), robots.end(), print_robot);
    // cout << "\n";

    // visualise_robots(robots);

    robots = move_robots_by_sec(robots, 100);

    cout << calculate_safety_factor(robots);
    cout << "\n";

    // for_each(robots.begin(), robots.end(), print_robot);
    // visualise_robots(robots);

    return 0;
}