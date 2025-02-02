#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

struct coord
{
	int x;
	int y;
	coord operator+(const coord &other) { return {x + other.x, y + other.y}; };
};

class WordSearcher
{
private:
	vector<vector<char>> grid;

	const vector<coord> adjDirs = {
		{-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {1, 0}, {-1, 1}, {0, 1}, {1, 1}};

	void readFile(const string &f);

	vector<coord> allCharCoords(char c) const;

	bool inBounds(const coord &pos) const;

	bool onGridEdge(const coord &pos) const;

	vector<coord> getAdjacent(coord pos) const;

	inline char at(coord pos) const { return grid[pos.x][pos.y]; };
	inline char at(int r, int c) const { return grid[r][c]; };

	int findMatchesAux(const string &word, const coord &dir, coord pos) const;

public:
	WordSearcher(const string &input);

	int findMatches(const string &word) const;

	int findCrossMatches() const;
};

void WordSearcher::readFile(const string &f)
{
	grid.clear();
	string line;
	ifstream file(f);

	while (getline(file, line))
	{
		grid.push_back({});
		for (auto c : line)
		{
			grid.back().push_back(c);
		}
	}
	file.close();
}

WordSearcher::WordSearcher(const string &input)
{
	readFile(input);
}

// Get a vector of all coordinates which contain the given character.
vector<coord> WordSearcher::allCharCoords(char c) const
{
	vector<coord> coords;
	for (int i = 0; i < grid.size(); i++)
	{
		for (int j = 0; j < grid[i].size(); j++)
		{
			if (at(i, j) == c)
				coords.push_back({i, j});
		}
	}
	return coords;
}

bool WordSearcher::inBounds(const coord &pos) const
{
	return (pos.x >= 0 && pos.x < grid.size() && pos.y >= 0 && pos.y < grid[pos.x].size());
}

bool WordSearcher::onGridEdge(const coord &pos) const
{
	return (pos.x == 0 || pos.y == 0 || pos.x == grid.size() - 1 || pos.y == grid[pos.x].size() - 1);
}
vector<coord> WordSearcher::getAdjacent(coord pos) const
{
	vector<coord> adj;
	for (auto dir : adjDirs)
	{
		coord d = dir + pos;
		if (inBounds(d))
		{
			adj.push_back(d);
		}
	}
	return adj;
}

// Find all versions of a given word in the grid. The word can be matched in all directions,
// but must be in a straight line. Letters can be used multiple times.
// Start by finding all positions of the starting letter, then search in every direction from each starting letter for the complete word.
int WordSearcher::findMatches(const string &word) const
{
	int total = 0;
	for (int i = 0; i < grid.size(); i++)
	{
		for (int j = 0; j < grid[i].size(); j++)
		{
			if (at(i, j) == word[0])
			{
				// Search from this correct starting position in up to 8 different directions.
				for (auto dir : adjDirs)
				{
					total += findMatchesAux(word, dir, {i, j});
				}
			}
		}
	}

	return total;
}

// word has already found in the range [0, idx). We know that grid[pos] == word[idx - 1]
// Check if we've finished the word, so idx == |word|
// Continue the search in the given direction, checking each new position for leaving the board or not a match.
// Return the number of matches, which can only be 1 for the given direction.
int WordSearcher::findMatchesAux(const string &word, const coord &dir, coord pos) const
{
	for (int i = 1; i < word.size(); i++)
	{
		pos = pos + dir;
		if (!inBounds(pos) || at(pos) != word[i])
		{
			return 0;
		}
	}
	return 1;
}

// Find the cross matches of a word in the grid. A cross match is two diagonal copies of a word that overlap on the middle char.
// An X match for MAS:
//   M . S
//   . A .
//   S . M
// For now, assume the word is MAS to decrease work needed to code.
int WordSearcher::findCrossMatches() const
{
	coord ul = {-1, -1}, br = {1, 1}, ur = {-1, 1}, bl = {1, -1};
	// Work through every A in the grid, as it must be the middle of the X-MAS
	int matches = 0;
	for (auto pos : allCharCoords('A'))
	{
		// Check that the A is not on the edge of the board
		if (onGridEdge(pos))
			continue;

		// Check that both MAS are there on the diagonals
		pair<coord, coord>
			diag1 = {pos + ul, pos + br},
			diag2 = {pos + ur, pos + bl};

		bool diag1Correct = ((at(diag1.first) == 'M' && at(diag1.second) == 'S') || (at(diag1.first) == 'S' && at(diag1.second) == 'M'));
		bool diag2Correct = ((at(diag2.first) == 'M' && at(diag2.second) == 'S') || (at(diag2.first) == 'S' && at(diag2.second) == 'M'));
		matches += int(diag1Correct && diag2Correct);
	}
	return matches;
}

int main()
{
	string input("input.txt");

	WordSearcher ws(input);

	int matches = ws.findCrossMatches();

	cout << "Matches: " << matches << endl;
	return 0;
}
