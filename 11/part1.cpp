#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

vector<long long> blink_stones(const vector<long long> &old_stones)
{
    vector<long long> stones;

    for (auto stone : old_stones)
    {
        if (stone == 0)
        {
            stones.push_back(1);
        }
        else if (to_string(stone).size() % 2 == 0)
        {
            string ss = to_string(stone);
            stones.push_back(stoll(ss.substr(0, ss.size() / 2)));
            stones.push_back(stoll(ss.substr(ss.size() / 2)));
        }
        else
        {
            stones.push_back(stone * 2024);
        }
    }

    return stones;
}

vector<long long> parse_stones(const string &path)
{
    vector<long long> stones;
    string line;
    ifstream file(path);
    if (getline(file, line))
    {
        stringstream ss(line);
        string s;
        while (getline(ss, s, ' '))
        {
            stones.push_back(stoll(s));
        }
    }
    file.close();
    return stones;
}

long long blink_stones_n_times(vector<long long> stones, int n)
{
    for (int i = 0; i < n; i++)
    {
        stones = blink_stones(stones);
    }
    return stones.size();
}

int main()
{
    vector<long long> stones = parse_stones("input.txt");
    int n = 75;

    cout << "Number of stones after " << n << " blinks = " << blink_stones_n_times(stones, n) << endl;
}