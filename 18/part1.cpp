#include "../scanner.h"

#include <vector>
#include <string>
#include <iostream>
#include <queue>

#define ROWS 71
#define COLS 71
#define MAX_BYTES 1024
enum Cell
{
    EMPTY,
    BLOCKED,
    BOUNDARY,
    VISITED
};

char cellStrs[] = {'.', '#', 'B', '+'};

struct Pos
{
    int x;
    int y;
    int steps;
    std::vector<Pos> getAdjacent()
    {
        std::vector<Pos> res;
        if (x > 0)
        {
            res.push_back({x - 1, y, steps + 1});
        }
        if (x < ROWS - 1)
        {
            res.push_back({x + 1, y, steps + 1});
        }
        if (y > 0)
        {
            res.push_back({x, y - 1, steps + 1});
        }
        if (y < COLS - 1)
        {
            res.push_back({x, y + 1, steps + 1});
        }

        return res;
    }
};

class Grid
{
private:
    Cell grid[ROWS][COLS];

    void initGrid()
    {
        for (int x = 0; x < ROWS; x++)
        {
            for (int y = 0; y < COLS; y++)
            {
                grid[x][y] = EMPTY;
            }
        }
    }

    void fillGrid(std::string file, int bytes)
    {
        ScanSequence ss(file, true);
        while (--bytes >= 0 && !ss.isAtEnd())
        {
            int x = ss.scanInt();
            ss.consumeString(",");
            int y = ss.scanInt();
            grid[x][y] = BLOCKED;
        }
    }

    Cell &at(const Pos &p)
    {
        return grid[p.x][p.y];
    }

    bool visitable(const Pos &p)
    {
        return at(p) == EMPTY;
    }

public:
    Grid(std::string file, int bytes)
    {
        initGrid();
        fillGrid(file, bytes);
    }

    void printGrid()
    {
        for (int y = 0; y < COLS; y++)
        {
            for (int x = 0; x < ROWS; x++)
            {
                std::cout << cellStrs[grid[x][y]];
            }
            std::cout << std::endl;
        }
        std::cout << '\n';
    }

    int findShortestPath()
    {
        auto posCompare = [](const Pos &l, const Pos &r)
        { return l.steps > r.steps; };
        std::priority_queue<Pos, std::vector<Pos>, decltype(posCompare)> boundary(posCompare);

        boundary.push({0, 0, 0});
        while (boundary.size())
        {
            Pos cur = boundary.top();
            boundary.pop();

            // printGrid();

            // printf("cur = {%d,%d}\n", cur.x, cur.y);
            if (cur.x == (ROWS - 1) && cur.y == (COLS - 1))
            {
                return cur.steps;
            }

            // visit the cell
            at(cur) = VISITED;
            for (auto next : cur.getAdjacent())
            {
                if (at(next) == EMPTY)
                {
                    boundary.push(next);
                    at(next) = BOUNDARY;
                }
            }
        }

        return -1;
    }
};

int main()
{
    Grid g("input.txt", MAX_BYTES);
    g.printGrid();

    std::cout << g.findShortestPath() << std::endl;
}