#include "../scanner.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <queue>

using namespace std;

class Maze
{
private:
    enum Direction
    {
        NORTH = 0,
        EAST,
        SOUTH,
        WEST
    };

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
    };

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

    Reindeer nextInDir(const Reindeer &cur)
    {
        switch (cur.dir)
        {
        case NORTH:
            return {cur.x, cur.y - 1, cur.dir, cur.score + 1};
        case EAST:
            return {cur.x + 1, cur.y, cur.dir, cur.score + 1};
        case SOUTH:
            return {cur.x, cur.y + 1, cur.dir, cur.score + 1};
        case WEST:
        default:
            return {cur.x - 1, cur.y, cur.dir, cur.score + 1};
        }
    }
    Reindeer nextRotateCounterClockwise(const Reindeer &cur)
    {
        switch (cur.dir)
        {
        case NORTH:
            return {cur.x, cur.y, WEST, cur.score + 1000};
        case EAST:
            return {cur.x, cur.y, NORTH, cur.score + 1000};
        case SOUTH:
            return {cur.x, cur.y, EAST, cur.score + 1000};
        case WEST:
        default:
            return {cur.x, cur.y, SOUTH, cur.score + 1000};
        }
    }
    Reindeer nextRotateClockwise(const Reindeer &cur)
    {
        switch (cur.dir)
        {
        case NORTH:
            return {cur.x, cur.y, EAST, cur.score + 1000};
        case EAST:
            return {cur.x, cur.y, SOUTH, cur.score + 1000};
        case SOUTH:
            return {cur.x, cur.y, WEST, cur.score + 1000};
        case WEST:
        default:
            return {cur.x, cur.y, NORTH, cur.score + 1000};
        }
    }
    vector<Reindeer> getAllRotations(const Reindeer &cur)
    {
        vector<Reindeer> rot;
        rot.push_back(nextRotateClockwise(cur));
        rot.push_back(nextRotateClockwise(rot.back()));
        rot.push_back(nextRotateCounterClockwise(cur));
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
                    start = {x, y, EAST, 0};
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

    void printMaze(const Reindeer &cur = {0, 0, NORTH, 0})
    {
        for (int y = 0; y < maze.size(); y++)
        {
            for (int x = 0; x < maze[y].size(); x++)
            {
                if (cur.x == x && cur.y == y)
                {
                    cout << 'R';
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
            // cout << "\nQueue= " << boundary.size();
            // printf(". Checking {%d, %d, %d, %d}\n", cur.x, cur.y, cur.dir, cur.score);
            // printMaze(cur);
            // Check for the first finding of the end cell
            if (at(cur) == END)
                return cur.score;

            for (auto next : getValidAdjacent(cur))
            {
                if (at(next) != END)
                    at(next) = EXPLORED;
                boundary.push(next);
            }

            // std::string s;
            // cin >> s;
            // if (s == "q")
            // {
            //     break;
            // }
        }

        return -1;
    }
};

int main()
{
    Maze m("input.txt");
    m.printMaze();

    cout << m.findShortestPath() << endl;
}