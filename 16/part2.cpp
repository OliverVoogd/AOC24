#include "../scanner.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <stack>

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

    Reindeer()
    {
        x = 0;
        y = 0;
        dir = NORTH;
        score = 0;
    }

    Reindeer(int x, int y, Direction dir, int score, const Reindeer &prev) : x(x), y(y), dir(dir), score(score)
    {
    }
};

class Maze
{
private:
    vector<vector<Cell>> maze;
    Reindeer start;

    stack<Reindeer> stackDFS;
    vector<vector<Reindeer>> validPaths;

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

    static inline Reindeer nextInDir(const Reindeer &cur)
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
    static inline Direction getNextDir(Direction dir, bool left)
    {
        int d = (int)dir;
        // if left = true, rotate counterclockwise
        if (left)
            d = 4 + d - 1;
        else
            d = 4 + d + 1;
        return (Direction)(d % 4);
    }

    static inline Reindeer rotate(const Reindeer &cur, bool left)
    {
        return {cur.x, cur.y, getNextDir(cur.dir, left), cur.score + 1000, cur};
    }
    inline Reindeer rotatedAndForward(const Reindeer &cur, bool left) const
    {
        Reindeer rotated = rotate(cur, left);
        return nextInDir(rotated);
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
        }
        // Counterclockwise rotation
        Reindeer rNext = rotatedAndForward(cur, false);
        // Clockwise rotation
        Reindeer lNext = rotatedAndForward(cur, true);
        // Only accept the moves if there's a free cell to move into
        if (at(rNext) <= EMPTY)
            adj.push_back(rNext);
        if (at(lNext) <= EMPTY)
            adj.push_back(lNext);

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

    // int findShortestPath()
    // {
    //     auto customReindeerLess = [](const Reindeer &L, const Reindeer &R)
    //     { return L.score > R.score; };
    //     priority_queue<Reindeer, vector<Reindeer>, decltype(customReindeerLess)> boundary(customReindeerLess);
    //
    //     boundary.push(start);
    //     for (auto r : getAllRotations(start))
    //     {
    //         boundary.push(r);
    //     }
    //     while (boundary.size() > 0)
    //     {
    //         Reindeer cur = boundary.top();
    //         boundary.pop();
    //
    //         // Print grid
    //         cout << "\nQueue= " << boundary.size();
    //         printf(". Checking {%d, %d, %d, %d}\n", cur.x, cur.y, cur.dir, cur.score);
    //         printMaze(cur);
    //         // Check for the first finding of the end cell
    //         if (at(cur) == END)
    //             return cur.score;
    //
    //         for (auto next : getValidAdjacent(cur))
    //         {
    //             if (at(next) != END)
    //                 at(next) = EXPLORED;
    //             boundary.push(next);
    //         }
    //
    //         std::string s;
    //         cin >> s;
    //         if (s == "q")
    //         {
    //             break;
    //         }
    //     }
    //
    //     return -1;
    // }

    void storeValidPathStack()
    {
        printf("\tStoring (not yet) a stack of size %lu and with score %d\n", stackDFS.size(), stackDFS.top().score);
    }
    void DFSAllPaths()
    {
        stackDFS = {};
        validPaths.clear();

        DFSAllPathsAux(start);

        // validPaths should be complete
        std::cout << validPaths.size() << std::endl;
    }

    // This function recursively performs a DFS through the maze.
    // Each time it finds the END, it adds a 'path' vector to validPaths.
    //
    void DFSAllPathsAux(Reindeer pos)
    {
        // let's do all the update stuff whenever we explore a cell
        // explore the cell we're searching
        auto temp = at(pos);
        at(pos) = EXPLORED;
        // then add this cell to the stack
        stackDFS.push(pos);

        // Now that we've officially explored this cell, we can recurse adjacent cells
        // printMaze(pos);
        // printf("Called DFSAux at {%d, %d}\n", pos.x, pos.y);
        // std::string s;
        // std::cin >> s;
        // if (s == "q")
        // {
        //     quick_exit(0);
        // }

        // adj is a vector of <= EMPTY cells
        auto adj = getValidAdjacent(pos);
        for (auto next : adj)
        {
            // When we find the END cell, append it to the completed path and terminate
            // as if we've found the END at this depth, there won't be a better way to get there if
            // we move into a different cell from here
            if (at(next) == END)
            {
                stackDFS.push(next);
                storeValidPathStack();
                stackDFS.pop();
                break;
            }

            // Not the end, and we know next is not explored, continue downwards
            // printf("\tRecursing into DFSAux at {%d, %d}\n", next.x, next.y);
            DFSAllPathsAux(next);
            // DFSAllPathsAux handles resetting cells once it's finished
        }

        // printf("Resetting cell {%d, %d}\n", pos.x, pos.y);
        // before we leave this cell (to backtrack up a level), unexplore it
        at(pos) = temp;
        // and pop from the stack
        stackDFS.pop();
    }
};

int main()
{
    Maze m("input.txt");
    // m.printMaze();

    m.DFSAllPaths();
}