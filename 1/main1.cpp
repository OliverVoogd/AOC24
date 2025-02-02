#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <assert.h>
#include <algorithm>
#include <utility>

/// @brief Iterate through the file and split each line into two integers separated by a space
/// Store each integer in a vector for each column, and return as a pair of vectors.
/// @param input file path string. The file must contain two columns of integers separated by spaces.
/// @return A pair object containing vectors of the two columns.
std::pair<std::vector<int>, std::vector<int>> splitAndParseFile(const std::string &input)
{
	std::vector<int> list1, list2;
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

		list1.push_back(std::stoi(line.substr(0, first_space)));
		list2.push_back(std::stoi(line.substr(last_space + 1)));
	}

	file.close();

	return {list1, list2};
}

/// @brief Sum the absolute distance between each pair of values in the given lists
/// @param a The first list of integer values
/// @param b The second list of integer values. Must be the same length as a
/// @return An integer value representing the distance between the two lists.
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