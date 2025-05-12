#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

vector<int> read_disk_map(const string &file_name) {
    vector<int> map;
    ifstream file(file_name);

    // get the single line disk map 
    string line;
    getline(file, line);
    int file_idx = 0;
    for (int i = 0; i < line.size(); i++) {
        if (i % 2) {
            // Odd indices, input is giving us the amount of free space
            for (int x = 0; x < line[i] - '0'; x++) {
                map.push_back(-1);
            }
        } else {
            // Even indices, input is the number of blocks of a new file
            for (int x = 0; x < line[i] - '0'; x++) {
                map.push_back(file_idx);
            }
            file_idx++;
        }
    }
    return map;
}

/// @brief Compact a file map by moving from right to left through the map and moving files to the first available empty position from the left.
/// @param map A file map to traverse and reposition files
/// @return A new vector which resembles a compacted file (all free space to the right of the last file id)
vector<int> compact_file_map(const vector<int> &map) {
    vector<int> compact(map);
    int left_empty = 0;
    int right_file = map.size() - 1;

    while (left_empty < right_file) {
        // Move our respective pointers if they're not pointing to the correct elements.
        if (compact[left_empty] != -1) {
            left_empty++;
            continue;
        }
        if (compact[right_file] == -1) {
            right_file--;
            continue;
        }

        // Both pointers are in the correct locations, so we can swap the elements.
        compact[left_empty++] = compact[right_file];
        compact[right_file--] = -1;
    }

    return compact;
}

/// @brief Calculate the checksum by multiplying every file position's id with it's index
/// @param map the file map of files
/// @return The checksum of the file.
long long file_map_checksum(const vector<int> &map) {
    long long sum = 0;
    for (long long i = 0; i < map.size(); i++) {
        if (map[i] == -1) continue;
        sum += map[i] * i;
    }
    return sum;
}

int main() {
    string file("input.txt");

    vector<int> map = read_disk_map(file);
    auto compact = compact_file_map(map);

    cout << "Checksum of the compacted file = " << file_map_checksum(compact) << endl;
}
