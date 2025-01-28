#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

typedef std::vector<int> Report;

int sign(int i)
{
	if (i == 0)
		return 0;
	if (i < 0)
		return -1;
	return 1;
}

Report splitStringToReport(const std::string &line)
{
	std::string tmp;
	std::stringstream ss(line);
	Report report;
	while (std::getline(ss, tmp, ' '))
	{
		report.push_back(std::stoi(tmp));
	}

	return report;
}

std::vector<Report> parseReports(const std::string &input)
{
	std::ifstream file(input);
	std::string line;
	std::vector<Report> reports;

	while (std::getline(file, line))
	{
		reports.push_back(splitStringToReport(line));
	}

	file.close();
	return reports;
}

void printVec(Report r)
{
	for (auto x : r)
	{
		std::cout << x << ", ";
	}
	std::cout << std::endl;
}

bool isReportSafe(const Report &report)
{
	int gradient = 0;
	for (int i = 0; i < report.size() - 1; i++)
	{
		int thisDiff = report[i + 1] - report[i];
		if (!gradient)
		{
			gradient = sign(thisDiff);
		}
		else
		{
			if (sign(thisDiff) != gradient)
				return false;
		}

		if (std::abs(thisDiff) < 1 || std::abs(thisDiff) > 3)
			return false;
	}
	return true;
}

bool isReportSafeWithDampener(const Report &report)
{
	// Check the report, removing one value each time to see if we can brute force a
	// safe list
	// Initially check the whole list
	if (isReportSafe(report))
		return true;

	for (int i = 0; i < report.size(); i++)
	{
		Report copy(report);
		copy.erase(copy.begin() + i);
		if (isReportSafe(copy))
			return true;
	}

	return false;
}

int main()
{
	// std::string input = "test.txt";
	std::string input = "input.txt";
	std::vector<Report> reports = parseReports(input);

	int numSafe = 0;
	for (auto r : reports)
	{
		numSafe += int(isReportSafeWithDampener(r));
	}

	std::cout << "Number of Safe Reports: " << numSafe << std::endl;
}