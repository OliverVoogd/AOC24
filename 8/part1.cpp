#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

struct coord
{
	int x;
	int y;

	coord operator+(const coord &other)
	{
		return {x + other.x, y + other.y};
	}
};

unordered_map<char, vector<coord>> parse_antenna_map(const string &filepath, int &out_n)
{
	unordered_map<char, vector<coord>> map;
	string line;
	ifstream file(filepath);

	int y = 0;
	while (getline(file, line))
	{
		for (int x = 0; x < line.size(); x++)
		{
			if (line[x] != '.')
			{
				map[line[x]].push_back({x, y});
			}
		}
		y++;
	}
	file.close();

	out_n = y;
	return map;
}

int main()
{
	int n;
	// Read in the n x n grid of antenna locations.
	unordered_map<char, vector<coord>> antenna_coords = parse_antenna_map("input.txt", n);

	cout << n << endl;
	return 0;
}