#include <string>
#include <iostream>
#include <fstream>
#include <regex>
#include <iterator>

/// @brief Parse a string and extract all the occurances of 'mul(x,y)', where x and y are strings of digits. Multiplies the x and y integer values together
/// and sums all occurances of the pattern. Any 'do()' or 'don't()' occurances respectively allow or disallow the inclusion of the next pattern occurances.
/// Inclusion is assumed to be allowed at the start of the pattern
/// E.g. mul(1,2)don't()mul(5,34)do()mul(2,10) should yield the value 1*2 + 2*10 == 22
/// @param line a string
/// @return The sum of the pattern values
long long findAndSumMul(std::string &line)
{
	std::regex mul_regex("do\\(\\)|don't\\(\\)|mul\\(([0-9]+),([0-9]+)\\)", std::regex_constants::ECMAScript);
	auto mul_begin = std::sregex_iterator(line.begin(), line.end(), mul_regex);
	auto mul_end = std::sregex_iterator();

	bool sum_enabled = true;
	long long sum = 0;
	for (auto it = mul_begin; it != mul_end; it++)
	{
		std::smatch match = *it;

		if (match.str() == "do()")
		{
			sum_enabled = true;
		}
		else if (match.str() == "don't()")
		{
			sum_enabled = false;
		}
		else if (sum_enabled)
		{
			sum += std::stoi(match[1].str()) * std::stoi(match[2].str());
		}
	}

	return sum;
}

int main()
{
	std::string input = "input.txt";

	std::ifstream file(input);
	std::string line;
	std::string fullString;
	long long sum = 0;
	while (std::getline(file, line))
	{
		fullString += line;
	}

	std::cout << "\t\tFull Sum == " << findAndSumMul(fullString) << std::endl;
}
