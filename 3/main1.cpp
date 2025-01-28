#include <string>
#include <iostream>
#include <fstream>
#include <regex>
#include <iterator>

long long findAndSumMul(std::string &line)
{
	std::regex mul_regex("mul\\(([0-9]+),([0-9]+)\\)", std::regex_constants::ECMAScript);
	auto mul_begin = std::sregex_iterator(line.begin(), line.end(), mul_regex);
	auto mul_end = std::sregex_iterator();

	long long sum = 0;
	for (auto it = mul_begin; it != mul_end; it++)
	{
		std::smatch match = *it;

		sum += std::stoi(match[1].str()) * std::stoi(match[2].str());
	}

	return sum;
}

int main()
{
	std::string input = "input.txt";

	std::ifstream file(input);
	std::string line;
	long long sum = 0;
	while (std::getline(file, line))
	{
		long long x = findAndSumMul(line);
		std::cout << "Sum of multiplies: ";
		std::cout << x << std::endl;
		sum += x;
	}

	std::cout << "\t\tFull Sum == " << sum << std::endl;
}
