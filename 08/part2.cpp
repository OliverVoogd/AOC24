#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <unordered_map>

using namespace std;

struct Vector2
{
	int x;
	int y;

	Vector2 operator+(const Vector2 &other) const
	{
		return {x + other.x, y + other.y};
	}
	Vector2 operator-(const Vector2 &other) const {
		return {x - other.x, y - other.y};
	}
    Vector2 operator*(const int &i) const {
        return {x * i, y * i};
    }
};

unordered_map<char, vector<Vector2>> parse_antenna_map(const string &filepath, int &out_n)
{
	unordered_map<char, vector<Vector2>> map;
	string line;
	ifstream file(filepath);

	int x = 0;
	while (getline(file, line))
	{
		for (int y = 0; y < line.size(); y++)
		{
			if (line[y] != '.')
			{
				map[line[y]].push_back({x, y});
			}
		}
		x++;
	}
	file.close();

	out_n = x;
	return map;
}

/// @brief  Check if a position vector is within the defined MxN grid
/// @param pos The vector2 to check
/// @param m Vertical dimension (rows)
/// @param n Horizontal dimension (columns)
/// @return true if with MxN
bool point_inside_grid(const Vector2 &pos, int m, int n) {
	return (pos.x >= 0 && pos.x < m && pos.y >= 0 && pos.y < n);
}

/// @brief Find all of the antinodes that correspond to the two nodes given. These
/// antinodes must be within the MxM grid that is defined. Antinodes must exist along the line
/// that extends through A and B, and must be some integer multiply of D := A - B.
vector<Vector2> find_antinodes_inside_grid(const Vector2 &A, const Vector2 &B, int m) {
    Vector2 D = A - B;
	vector<Vector2> res;
	int i = 0;
    while (point_inside_grid(A + D * i, m, m)) {
        res.push_back(A + (D * i));
        i++;
    }
    i = 0;
    while (point_inside_grid(B - (D * i), m, m)) {
        res.push_back(B - (D * i));
        i++;
    }
	return res;
}

/// @brief Find the number of antinodes (a position that is on a line between two antennas and is
/// double the distance from one of the nodes than the other) that exist within the bounds of a grid.
/// Antinodes can overlap antennas, but each position is only counted as an antinode.
/// @param grid_size The dimensions of the NxN grid the antennas exist on
/// @param antennas a map of antennas stored under their character representation
/// @return An integer of the number of unique positions antinodes are at.
int calc_antinodes_inside_grid(int grid_size, unordered_map<char, vector<Vector2>> antennas) {
	vector<vector<bool>> grid(grid_size, vector<bool>(grid_size, false));

	for (auto [key, positions] : antennas) {
		for (int i = 0; i < positions.size(); i++) {
			for (int j = i + 1; j < positions.size(); j++) {
				auto antinodes = find_antinodes_inside_grid(positions[i], positions[j], grid_size);
				for (auto an : antinodes) {
					grid[an.x][an.y] = true;
				}
			}
		}
	}

	int num = 0;
	for (int i = 0; i < grid_size; i++) {
		for (int j = 0; j < grid_size; j++) {
			num += (grid[i][j] ? 1 : 0);
		}
	}
	return num;
}

int main()
{
	int n;
	// Read in the n x n grid of antenna locations.
	unordered_map<char, vector<Vector2>> antenna_points = parse_antenna_map("input.txt", n);
	
	cout << "Number of antinodes within grid: " << calc_antinodes_inside_grid(n, antenna_points) << endl;
	return 0;
}