#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <assert.h>
#include <algorithm>
#include <utility>
#include <unordered_map>

std::pair<std::vector<int>, std::vector<int>> splitAndParseFile(const std::string &input)
{
	std::pair<std::vector<int>, std::vector<int>> lists;
	std::ifstream file(input);
	if (!file.is_open())
	{
		return {{}, {}};
	}

	std::string line;
	int a, b;
	while (std::getline(file, line))
	{
		size_t first_space = line.find_first_of(' ');
		size_t last_space = line.find_last_of(' ');

		lists.first.push_back(std::stoi(line.substr(0, first_space)));
		lists.second.push_back(std::stoi(line.substr(last_space + 1)));
	}

	file.close();

	return lists;
}

int calcDistanceBetweenSortedLists(const std::vector<int> &a, const std::vector<int> &b)
{
	// build an hash map for the second list
	// the map contains <value> : <occurances>
	std::unordered_map<int, int> reps;
	for (auto x : b)
	{
		reps[x]++;
	}

	int dist = 0;
	for (auto x : a)
	{
		dist += x * reps[x];
	}

	return dist;
}

int main()
{
	std::string input = "input.txt";
	std::pair<std::vector<int>, std::vector<int>> lists = splitAndParseFile(input);

	int dist = calcDistanceBetweenSortedLists(lists.first, lists.second);

	std::cout << "Distance between lists: " << dist << std::endl;

	return 0;
}