#include "../scanner.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <queue>

using namespace std;

enum Direction
{
    NORTH = 0,
    EAST,
    SOUTH,
    WEST
};

char directionSymbols[] = {'^', '>', 'v', '<'};

enum Cell
{
    END,
    EMPTY,
    EXPLORED,
    WALL
};

struct Reindeer
{
    int x;
    int y;
    Direction dir;
    int score;
    vector<Reindeer> moves;

    Reindeer()
    {
        x = 0;
        y = 0;
        dir = NORTH;
        score = 0;
        moves = {};
    }

    Reindeer(int x, int y, Direction dir, int score, const Reindeer &prev) : x(x), y(y), dir(dir), score(score)
    {
        moves = vector<Reindeer>(prev.moves);
        moves.push_back(prev);
    }
};

class Maze
{
private:
    vector<vector<Cell>> maze;

    Reindeer start;

    Cell &at(const Reindeer &pos)
    {
        return maze[pos.y][pos.x];
    }
    Cell &at(const int &x, const int &y)
    {
        return maze[y][x];
    }

    void printReindeer(const Reindeer &r)
    {
        cout << "\tr={" << r.x << ", " << r.y << ", " << directionSymbols[r.dir] << ", " << r.score << "}\n";
    }

    Reindeer nextInDir(const Reindeer &cur)
    {
        switch (cur.dir)
        {
        case NORTH:
            return Reindeer(cur.x, cur.y - 1, cur.dir, cur.score + 1, cur);
        case EAST:
            return Reindeer(cur.x + 1, cur.y, cur.dir, cur.score + 1, cur);
        case SOUTH:
            return Reindeer(cur.x, cur.y + 1, cur.dir, cur.score + 1, cur);
        case WEST:
        default:
            return Reindeer(cur.x - 1, cur.y, cur.dir, cur.score + 1, cur);
        }
    }
    Direction getNextDir(Direction dir, bool left)
    {
        int d = (int)dir;
        // if left = true, rotate counterclockwise
        if (left)
            d = 4 + d - 1;
        else
            d = 4 + d + 1;
        return (Direction)(d % 4);
    }
    Reindeer nextRotateCounterClockwise(const Reindeer &cur)
    {
        return Reindeer(cur.x, cur.y, getNextDir(cur.dir, true), cur.score + 1000, cur);
    }
    Reindeer nextRotateClockwise(const Reindeer &cur)
    {
        return Reindeer(cur.x, cur.y, getNextDir(cur.dir, false), cur.score + 1000, cur);
    }
    vector<Reindeer> getAllRotations(const Reindeer &cur)
    {
        vector<Reindeer> rot;
        Reindeer r = nextRotateClockwise(cur);
        // printReindeer(r);
        Reindeer l = nextRotateCounterClockwise(cur);
        // printReindeer(l);

        // Only accept the moves if there's a free cell to move into
        if (at(nextInDir(r)) <= EMPTY)
            rot.push_back(r);
        if (at(nextInDir(l)) <= EMPTY)
            rot.push_back(l);

        return rot;
    }
    // When we move forwards, also get all of the rotations we could be in at that new location
    // Thus avoiding infinite rotating in a valid cell
    vector<Reindeer> getValidAdjacent(const Reindeer &cur)
    {
        vector<Reindeer> adj;
        Reindeer forward = nextInDir(cur);
        if (at(forward) <= EMPTY)
        {
            adj.push_back(forward);
            for (auto r : getAllRotations(forward))
            {
                adj.push_back(r);
            }
        }

        return adj;
    }

public:
    Maze(const std::string &file)
    {
        ScanSequence ss("input.txt", true);
        int y = 0;
        while (!ss.isAtEnd())
        {
            maze.push_back({});
            int x = 0;
            while (!ss.isAtEnd() && ss.peekAtChar() != '\n')
            {
                char c = ss.scanChar();
                switch (c)
                {
                case '#':
                    maze.back().push_back(WALL);
                    break;
                case 'S':
                    maze.back().push_back(EXPLORED);
                    start = {x, y, EAST, 0, {}};
                    break;
                case 'E':
                    maze.back().push_back(END);
                    break;
                default:
                    maze.back().push_back(EMPTY);
                    break;
                }

                x++;
            }
            y++;
            ss.consumeWhitespace();
        }
    }

    void printMaze(const Reindeer &cur = {0, 0, NORTH, 0, {}})
    {
        for (int y = 0; y < maze.size(); y++)
        {
            for (int x = 0; x < maze[y].size(); x++)
            {
                if (cur.x == x && cur.y == y)
                {
                    cout << directionSymbols[cur.dir];
                    continue;
                }
                switch (at(x, y))
                {
                case EMPTY:
                    cout << '.';
                    break;
                case WALL:
                    cout << '#';
                    break;
                case EXPLORED:
                    if (x == start.x && y == start.y)
                        cout << 'S';
                    else
                        cout << '*';
                    break;
                case END:
                default:
                    cout << 'E';
                    break;
                }
            }
            cout << '\n';
        }
    }

    int findShortestPath()
    {
        auto customReindeerLess = [](const Reindeer &L, const Reindeer &R)
        { return L.score > R.score; };
        priority_queue<Reindeer, vector<Reindeer>, decltype(customReindeerLess)> boundary(customReindeerLess);

        boundary.push(start);
        for (auto r : getAllRotations(start))
        {
            boundary.push(r);
        }
        while (boundary.size() > 0)
        {
            Reindeer cur = boundary.top();
            boundary.pop();

            // Print grid
            cout << "\nQueue= " << boundary.size();
            printf(". Checking {%d, %d, %d, %d}\n", cur.x, cur.y, cur.dir, cur.score);
            printMaze(cur);
            // Check for the first finding of the end cell
            if (at(cur) == END)
                return cur.score;

            for (auto next : getValidAdjacent(cur))
            {
                if (at(next) != END)
                    at(next) = EXPLORED;
                boundary.push(next);
            }

            std::string s;
            cin >> s;
            if (s == "q")
            {
                break;
            }
        }

        return -1;
    }
};

int main()
{
    Maze m("input.txt");
    // m.printMaze();

    cout << m.findShortestPath() << endl;
}