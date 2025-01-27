#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <assert.h>
#include <algorithm>
#include <utility>

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
	int dist = 0;
	assert(a.size() == b.size());
	for (int i = 0; i < a.size(); i++)
	{
		dist += std::abs(a[i] - b[i]);
	}
	return dist;
}

int main()
{
	std::string input = "input.txt";
	std::pair<std::vector<int>, std::vector<int>> lists = splitAndParseFile(input);

	std::sort(lists.first.begin(), lists.first.end());
	std::sort(lists.second.begin(), lists.second.end());

	int dist = calcDistanceBetweenSortedLists(lists.first, lists.second);
	std::cout << "Distance between lists: " << dist << std::endl;

	return 0;
}