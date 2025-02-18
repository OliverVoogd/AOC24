#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stack>
#include <set>
#include <numeric>
using namespace std;

typedef pair<int, int> Vec2;

vector<Vec2> get_orthagonal_adjecent(const Vec2 &v)
{
    return {
        {v.first - 1, v.second}, {v.first + 1, v.second}, {v.first, v.second - 1}, {v.first, v.second + 1}};
}

class Grid
{
private:
    vector<vector<int>> grid;
    vector<Vec2> heads;
    int rows, cols;
    bool in_bounds(const Vec2 &pos) const
    {
        return pos.first >= 0 && pos.first < cols && pos.second >= 0 && pos.second < rows;
    }
    bool in_bounds_and_accessible(const Vec2 &pos, int cur_height) const
    {
        return in_bounds(pos) && (at(pos) - cur_height == 1);
    }
    int at(const Vec2 &pos) const
    {
        return grid[pos.second][pos.first];
    }
    int at(int x, int y) const
    {
        return grid[y][x];
    }

public:
    void parse_file(const string &path)
    {
        ifstream file(path);
        string line;
        int row = 0, col = 0;
        while (getline(file, line))
        {
            cols = line.size();
            grid.push_back({});
            for (int col = 0; col < line.size(); col++)
            {
                grid.back().push_back(line[col] - '0');
                if (line[col] == '0')
                {
                    heads.push_back({col, row});
                }
            }
            row++;
        }
        rows = row;
        file.close();
    }

    void print_grid() const
    {
        for (int i = 0; i < grid.size(); i++)
        {
            for (int j = 0; j < grid[i].size(); j++)
            {
                cout << grid[i][j];
            }
            cout << endl;
        }
    }

    int score_trail(const Vec2 &head) const
    {
        stack<Vec2> boundary;
        set<Vec2> explored;
        boundary.push(head);
        int summits = 0;
        while (boundary.size())
        {
            Vec2 cur = boundary.top();
            boundary.pop();
            if (at(cur) == 9)
            {
                summits++;
                continue;
            }
            for (auto next : get_orthagonal_adjecent(cur))
            {
                if (in_bounds_and_accessible(next, at(cur)) && explored.count(next) == 0)
                {
                    boundary.push(next);
                    explored.insert(next);
                }
            }
        }

        return summits;
    }

    int score_all_trails() const
    {
        return accumulate(heads.begin(), heads.end(), 0, [*this](int lhs, const Vec2 &rhs)
                          { return lhs + score_trail(rhs); });
    }
};

int main()
{
    string input = "input.txt";
    Grid g;
    g.parse_file(input);
    cout << "The sum of all trailhead scores is: " << g.score_all_trails() << endl;
}