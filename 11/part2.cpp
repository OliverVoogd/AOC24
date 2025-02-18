#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>

using namespace std;

/// @brief Blink each stone, updating to it's new value.
/// Groups stones by the face value, ignoring it's position in order to update every stone with the same face value at once.
/// @param old_stones The map of stone face values (keys) to number of that stone value (value)
/// @return Creates and returns a new map containing the new count of each stone face.
unordered_map<long long, long long> blink_stones(const unordered_map<long long, long long> &old_stones)
{
    unordered_map<long long, long long> stones;

    for (auto [stone, count] : old_stones)
    {
        if (stone == 0)
        {
            stones[stone + 1] += count;
        }
        else if (to_string(stone).size() % 2 == 0)
        {
            string ss = to_string(stone);
            stones[stoll(ss.substr(0, ss.size() / 2))] += count;
            stones[stoll(ss.substr(ss.size() / 2))] += count;
        }
        else
        {
            stones[stone * 2024] += count;
        }
    }

    return stones;
}

unordered_map<long long, long long> parse_stones(const string &path)
{
    unordered_map<long long, long long> stones;
    string line;
    ifstream file(path);
    if (getline(file, line))
    {
        stringstream ss(line);
        string s;
        while (getline(ss, s, ' '))
        {
            stones[stoll(s)]++;
        }
    }
    file.close();
    return stones;
}

long long blink_stones_n_times(unordered_map<long long, long long> stones, int n)
{
    for (int i = 0; i < n; i++)
    {
        stones = blink_stones(stones);
    }

    long long sum = 0;
    for (auto [stone, count] : stones)
    {
        sum += count;
    }

    return sum;
}

int main()
{
    unordered_map<long long, long long> stones = parse_stones("input.txt");
    int n = 75;

    cout << "Number of stones after " << n << " blinks = " << blink_stones_n_times(stones, n) << endl;
}