#include <string>
#include <vector>
#include <utility>
#include <iostream>
#include <fstream>

using namespace std;

// each direction is (row, col)
vector<pair<int, int>> coord_directions = {
	{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

enum direction
{
	UP = 0,
	RIGHT = 1,
	DOWN = 2,
	LEFT = 3
};

struct guard
{
	int r;
	int c;
	direction dir;
};

/// @brief Read a grid from file into a 2d vector
/// @param filename file path
/// @param start_symbol the symbol indicating the position of the guard
/// @param out_guard a struct to update with guard position and facing
/// @return the 2d grid representing the room
vector<vector<char>> read_single_char_grid(const string &filename, char start_symbol, guard &out_guard)
{
	ifstream file(filename);
	vector<vector<char>> grid;

	string line;
	int row = 0;
	while (getline(file, line))
	{
		grid.push_back({});
		for (int col = 0; col < line.size(); col++)
		{
			grid.back().push_back(line[col]);
			if (line[col] == start_symbol)
			{
				out_guard.r = row;
				out_guard.c = col;
				out_guard.dir = UP;
			}
		}
		row++;
	}

	file.close();
	return grid;
}

/// @brief Get a pair of the next position the guard will move to. Convenience func
/// @param g guard struct
/// @return a pair of the new values.
pair<int, int> get_next_position(const guard &g)
{
	auto dir = coord_directions[g.dir];
	return {dir.first + g.r, dir.second + g.c};
}

/// @brief Update the position of the passed-by-reference guard struct, using the grid to check if we've left the room.
/// If the space to move into is a # char, we must rotate 90 clockwise until we have a free movement
/// @param out_guard reference to the guard struct, to update
/// @param grid grid object
/// @return true if we can continue moving, false if we've left the grid.
bool update_guard_position(guard &out_guard, const vector<vector<char>> &grid)
{
	// Check if the next position is a collision position
	auto next_pos = get_next_position(out_guard);
	if (next_pos.first < 0 || next_pos.first >= grid.size() || next_pos.second < 0 || next_pos.second >= grid[0].size())
	{
		return false;
	}

	while (grid[next_pos.first][next_pos.second] == '#')
	{
		// Rotate the guard by 90deg. Continue moving
		out_guard.dir = direction((out_guard.dir + 1) % 4);
		next_pos = get_next_position(out_guard);
	}
	out_guard.r = next_pos.first;
	out_guard.c = next_pos.second;
	return true;
}

/// @brief Iterate through the guards positions, maintaining the visited positions as a second grid
/// to check every movement so we can skip counting duplicate spaces visisted.
/// @param g the guard struct, with initial position and direction set from the file read
/// @param grid the grid read from the file
/// @return the number of unique cells visited
int follow_guard_count_unique(guard g, const vector<vector<char>> &grid)
{
	vector<vector<bool>> visited(grid.size(), vector<bool>(grid[0].size(), false));
	visited[g.r][g.c] = true;
	int vc = 1;
	// Update the guard's position one step until it leaves the grid
	while (update_guard_position(g, grid))
	{
		if (!visited[g.r][g.c])
		{
			vc++;
			visited[g.r][g.c] = true;
		}
	}
	return vc;
}

int main()
{
	string input("input.txt");
	guard g;
	auto grid = read_single_char_grid(input, '^', g);

	int unique_visited = follow_guard_count_unique(g, grid);
	cout << "Number of unique cells visited: " << unique_visited << endl;

	return 0;
}