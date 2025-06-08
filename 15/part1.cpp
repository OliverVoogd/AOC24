#include <iostream>
#include <vector>
#include <string>

#include "../scanner.h"

// #define DEBUG_PRINT_TURNS

class Warehouse
{
private:
    enum Cells
    {
        EMPTY,
        WALL,
        BOX
    };
    struct Pos
    {
        int x;
        int y;

        Pos operator+(const Pos &other) const
        {
            return {x + other.x, y + other.y};
        }
        bool operator==(const Pos &other) const
        {
            return (x == other.x) && (y == other.y);
        }
        bool operator!=(const Pos &other) const
        {
            return !operator==(other);
        }
    };

    std::vector<std::vector<Cells>> grid;

    std::vector<char> moves;

    Pos robot;

    Cells at(const Pos &pos)
    {
        return grid[pos.y][pos.x];
    }

    Cells
    parseCell(const char c, int x, int y)
    {
        switch (c)
        {
        case '#':
            return Cells::WALL;
        case '.':
            return Cells::EMPTY;
        case 'O':
            return Cells::BOX;
        case '@':
            robot = {x, y};
            return Cells::EMPTY;
        default:
            return WALL;
        }
    }
    void parseGrid(ScanSequence &ss)
    {
        grid.clear();
        int y = 0;
        int x;
        while (ss.peekAtChar() == '#')
        {
            grid.push_back({});
            x = 0;
            while (ss.peekAtChar() != '\n')
            {
                grid.back().push_back(parseCell(ss.scanChar(), x, y));
                x++;
            }
            ss.consumeWhitespace();
            y++;
        }
    }

    bool shuffleBoxes(const Pos &cur, const Pos &dir)
    {
        Pos next = cur + dir;
        while (at(next) == BOX)
        {
            next = next + dir;
        }
        // After the loop, next is either a WALL or an EMPTY
        if (at(next) == EMPTY)
        {
            // std::cout << "End shuffle starting at " << cur.x << "," << cur.y << std::endl;
            // the ideal situation, delete the first box and add one in at the empty
            grid[next.y][next.x] = BOX;
            grid[cur.y][cur.x] = EMPTY;
            return true;
        }

        // Otherwise, we've hit a wall and we can't shuffle the boxes
        return false;
    }

    Pos moveRobot(const Pos &cur, const Pos &dir)
    {
        Pos target = cur + dir;
        if (at(target) == WALL)
        {
            return cur;
        }
        if (at(target) == EMPTY)
        {
            return target;
        }

        if (at(target) == BOX)
        {
            // If we can shuffle the boxes over
            // Then we can move into the space
            if (shuffleBoxes(target, dir))
            {
                return target;
            }
        }

        return cur;
    }

public:
    Warehouse(const std::string &input)
    {
        std::ifstream file(input);
        ScanSequence ss(file);

        parseGrid(ss);
        ss.consumeWhitespace();

        while (!ss.isAtEnd())
        {
            moves.push_back(ss.scanChar());
        }

        file.close();
    }

    void printGrid(bool printMoves)
    {
        for (int y = 0; y < grid.size(); y++)
        {
            for (int x = 0; x < grid[y].size(); x++)
            {
                switch (grid[y][x])
                {
                case WALL:
                    std::cout << '#';
                    break;
                case EMPTY:
                {
                    if (x == robot.x && y == robot.y)
                    {
                        std::cout << '@';
                    }
                    else
                    {
                        std::cout << '.';
                    }
                    break;
                }
                case BOX:
                    std::cout << 'O';
                    break;
                }
            }
            std::cout << std::endl;
        }

        if (printMoves)
        {
            std::cout << '\n';
            for (auto c : moves)
            {
                std::cout << c;
            }
            std::cout << '\n';
        }
    }

    int sumCoords()
    {
        int sum = 0;
        for (int y = 0; y < grid.size(); y++)
        {
            for (int x = 0; x < grid[y].size(); x++)
            {
                if (grid[y][x] == BOX)
                {
                    sum += (100 * y) + x;
                }
            }
        }
        return sum;
    }
    void run()
    {
        for (auto move : moves)
        {
            Pos dest;

            switch (move)
            {
            case '^':
                dest = moveRobot(robot, {0, -1});
                break;
            case '>':
                dest = moveRobot(robot, {1, 0});
                break;
            case 'v':
                dest = moveRobot(robot, {0, 1});
                break;
            case '<':
                dest = moveRobot(robot, {-1, 0});
                break;
            }

            // Move the robot if we can
            robot = dest;
            // #ifdef DEBUG_PRINT_TURNS
            //             std::cout << "Robot attempts " << move << " from (" << robot.x << "," << robot.y << ") to (" << dest.x << "," << dest.y << ")\n";
            //             printGrid();
            // #endif
        }
    }
};

int main()
{
    std::string file("input.txt");
    Warehouse w(file);

    w.printGrid(false);
    w.run();

    std::cout << "\nFinished movements: \n";
    w.printGrid(false);

    std::cout << "\nSum = " << w.sumCoords() << std::endl;
}