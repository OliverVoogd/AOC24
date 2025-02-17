#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

vector<int> read_disk_map(const string &file_name)
{
    vector<int> map;
    ifstream file(file_name);

    // get the single line disk map
    string line;
    getline(file, line);
    int file_idx = 0;
    for (int i = 0; i < line.size(); i++)
    {
        if (i % 2)
        {
            // Odd indices, input is giving us the amount of free space
            for (int x = 0; x < line[i] - '0'; x++)
            {
                map.push_back(-1);
            }
        }
        else
        {
            // Even indices, input is the number of blocks of a new file
            for (int x = 0; x < line[i] - '0'; x++)
            {
                map.push_back(file_idx);
            }
            file_idx++;
        }
    }
    return map;
}

/// @brief Find the leftmost block of free space that meets the size requirements given.
int find_leftmost_free_space(const vector<int> &map, int req_size)
{
    int left_empty = 0;
    int i = 0;
    while (i < map.size())
    {
        if (map[i] == -1 && i - left_empty >= req_size)
            return left_empty;
        if (map[i] != -1)
        {
            if (i - left_empty >= req_size)
                return left_empty;
            left_empty = i + 1;
        }
        i++;
    }

    return -1;
}

/// @brief Finds the starting index (leftmost position) of the rightmost file in given map, only searching to the left of the prev_idx variable.
/// The intention here is to find the next file to sort into position.
/// @param map The file map to search through
/// @param prev_idx the position to search to the left of
/// @return A pair containing first: the leftmost position of the file, and second: the length of that file.
pair<int, int> find_rightmost_file_from_idx(const vector<int> &map, int prev_idx)
{
    if (prev_idx <= 0)
        return {-1, -1};
    prev_idx--;
    int file_idx = map[prev_idx];
    int right_pos = -1;

    if (file_idx != -1)
    {
        right_pos = prev_idx;
    }
    for (int i = prev_idx - 1; i >= 0; i--)
    {
        if (file_idx != -1 && (map[i] == -1 || map[i] != file_idx))
        {
            // move passed the left edge of the file block
            return {i + 1, right_pos - i};
        }
        if (file_idx == -1 && map[i] != -1)
        {
            // move into a file block from the right edge
            file_idx = map[i];
            right_pos = i;
        }
    }

    if (file_idx != -1)
    {
        return {0, right_pos + 1};
    }
    return {-1, -1};
}

/// @brief Compact a file map by moving whole blocks of files working right to left through the map to move them to the left most
/// available blocks of free space.
/// @param map A file map to traverse and reposition files
/// @return A new vector which resembles a compacted file (all free space to the right of the last file id)
vector<int> compact_file_map_moving_whole_files(const vector<int> &map)
{
    vector<int> compact(map);

    pair<int, int> rightmost_file = {compact.size(), 0};
    int leftmost_free;

    int i = 0;
    // Iterate through every file and seek to move them to the left into available free space
    while (rightmost_file.first > 0)
    {
        rightmost_file = find_rightmost_file_from_idx(compact, rightmost_file.first);
        leftmost_free = find_leftmost_free_space(compact, rightmost_file.second);

        // Check if we've got a valid location
        if (leftmost_free == -1 || leftmost_free > rightmost_file.first)
        {
            // decrement the last position if we can't move that file.
            i++;
            continue;
        }
        // Move the file into the available location
        for (int i = 0; i < rightmost_file.second; i++)
        {
            compact[leftmost_free + i] = compact[rightmost_file.first + i];
            compact[rightmost_file.first + i] = -1;
        }
    }

    return compact;
}

/// @brief Calculate the checksum by multiplying every file position's id with it's index
/// @param map the file map of files
/// @return The checksum of the file.
long long file_map_checksum(const vector<int> &map)
{
    long long sum = 0;
    for (long long i = 0; i < map.size(); i++)
    {
        if (map[i] == -1)
            continue;
        sum += map[i] * i;
    }
    return sum;
}

int main()
{
    string file("input.txt");

    vector<int> map = read_disk_map(file);
    auto compact = compact_file_map_moving_whole_files(map);
    cout << "Checksum of the compacted file = " << file_map_checksum(compact) << endl;
}
