#include <string>
#include <iostream>
#include <fstream>
#include <regex>
#include <iterator>

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
