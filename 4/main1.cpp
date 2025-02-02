#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

struct coord {
  int x;
  int y;
};

class WordSearcher {
private:
    vector<vector<char>> grid;
    vector<coord> adjDirs = {
        {-1, -1}, {0, -1}, {1, -1},
        {-1,  0},          {1,  0},
        {-1,  1}, {0,  1}, {1,  1}
    };
    
    void readFile(const string &f);
    vector<coord> allCharCoords(char c) const;
    bool inBounds(const coord &pos) const;
    vector<coord> getAdjacent(coord pos) const;
    
    int findMatchesAux(const string &word, coord pos, coord dir);
public:
    WordSearcher(const string &input);
    
    int findMatches(const string &word);
};


void WordSearcher::readFile(const string &f) {
    grid.clear();
    string line;
    ifstream file(f);
    
    while (getline(file, line)) {
        grid.push_back({});
        for (auto c : line) { 
            grid.back().push_back(c);
        }
    }
    file.close();
}
WordSearcher::WordSearcher(const string &input) {
    readFile(input);
}
vector<coord> WordSearcher::allCharCoords(char c) const {
    vector<coord> coords;
    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[i].size(); j++) {
            if (grid[i][j] == c) coords.push_back({i, j});
        }
    }
    return coords;
}
bool WordSearcher::inBounds(const coord &pos) const {
    return (pos.x >= 0 && pos.x < grid.size() && pos.y >= 0 && pos.y < grid[pos.x].size());
}
vector<coord> WordSearcher::getAdjacent(coord pos) const {
    vector<coord> adj;
    for (auto dir : adjDirs) {
        coord d = {dir.x + pos.x, dir.y + pos.y};
        if (inBounds(d)) {
            adj.push_back(d);
        }
    }
    return adj;
}
// Search the grid for starting letters, and the search from
// each starting letter in every possible direction (straight line only)
int WordSearcher::findMatches(const string &word) {
    int total = 0;
    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[i].size(); j++) {
            if (grid[i][j] == word[0]) {
                for (auto dir : adjDirs) {
                    total += findMatchesAux(word, {i, j}, dir);
                }
            }
        }
    }
    
    return total;
}
// word has been found in the range [0, idx). grid[pos] == word[idx - 1]
// Check if we've finished the word, so idx == |word|
// If we haven't check adjacent cells for word[idx], and continue the search from each
int WordSearcher::findMatchesAux(const string &word, coord pos, coord dir) {
    for (int i = 1; i < word.size(); i++) {
        pos = {pos.x + dir.x, pos.y + dir.y};
        if (!inBounds(pos) || grid[pos.x][pos.y] != word[i]) {
            return 0;
        }
    }
    return 1;
}

int main()
{
    string input("test.txt");
    
    WordSearcher ws(input);
    
    int matches = ws.findMatches("XMAS");
    
    cout << "Matches: " << matches << endl;
    return 0;
}
