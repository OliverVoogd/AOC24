#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <assert.h>
#include <algorithm>
#include <utility>
#include <unordered_map>

/// @brief Iterate through the file and split each line into two integers separated by a space
/// Store each integer in a vector for each column, and return as a pair of vectors.
/// @param input file path string. The file must contain two columns of integers separated by spaces.
/// @return A pair object containing vectors of the two columns.
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

/// @brief Calculate the 'distance' between sorted lists. This 'distance' value is calculated
/// by summing all the values of a[i] multiplied the number of occurances of a[i] in list b
/// @param a Main vector of values
/// @param b Vector of values where each value occurs >= 1 times.
/// @return the 'distance' sum
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