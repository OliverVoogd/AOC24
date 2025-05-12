#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>

using namespace std;

typedef vector<int> update;
typedef vector<int> pages;

/// @brief Reads a file into conditional rules and lines of page numbers.
/// Can determine if the lines of page numbers are ordered correctly based on the conditional rules,
/// and can also sum all the median values of the correctly ordered lines of numbers.
class PrintOrdering
{
private:
	unordered_map<int, pages> conditional_key_before_values;

	vector<update> all_updates;

	/// @brief Checks if lhs needs to appear before rhs in the print output ordering.
	/// @param lhs a page number
	/// @param rhs second page number
	/// @return Returns true if the pair lhs|rhs exists in the input file, otherwise returns false
	bool printBefore(int lhs, int rhs) const;

public:
	/// @brief Parse a file into conditional rules and vectors of page numbers
	/// @param input a file containing the conditional rules first in the form X|Y, then a one line break, then any number of lines
	/// of comma separated integer values.
	void parse_conditional_file(const string &input);

	/// @brief Check each line of updates stored in all_updates and then sum the median of the correctly ordered lines
	/// @return the sum of the medians of the correct update lines.
	int get_center_sum_from_correct_updates() const;

	/// @brief Checks an single line all_updates[update_idx] for valid ordering.
	/// @param update_idx The index of the update line to check
	/// @return True if correctly ordered.
	bool check_update(int update_idx) const;
	/// @brief Get each incorrectly ordered update, and sort it properly. Sum the medians of these updates.
	/// Sorting is performed using the standard library sort function and custom comparison.
	/// @return the integer sum of the medians of the now-correctly sorted (incorrect) update lines
	int reorder_incorrect_sum_median() const;
	/// @brief Sort an update line using the standard library sort algorithm and our (private) custom comparison function printBefore
	/// @param up the update to sort
	/// @return a copy of `up`, correctly sorted.
	update sort_update(const update &up) const;

	size_t num_updates() const { return all_updates.size(); };
};

void PrintOrdering::parse_conditional_file(const string &input)
{
	conditional_key_before_values.clear();
	all_updates.clear();

	ifstream file(input);
	string line;
	while (getline(file, line) && line.size() != 0)
	{
		// Parse the pairs of conditions
		size_t sep_pos = line.find_first_of('|');
		int before = stoi(line.substr(0, sep_pos));
		int after = stoi(line.substr(sep_pos + 1));
		conditional_key_before_values[before].push_back(after);
	}

	while (getline(file, line))
	{
		// Parse the lists of pages to print
		stringstream line_stream(line);
		string page;
		vector<int> these_pages;
		while (getline(line_stream, page, ','))
		{
			these_pages.push_back(stoi(page));
		}

		all_updates.push_back(these_pages);
	}

	file.close();
}

bool PrintOrdering::check_update(int update_idx) const
{
	const update &up = all_updates[update_idx];
	unordered_map<int, int> page_to_position;

	// Iterate through every page in this update in order, recording the position each page appears at
	for (int i = 0; i < up.size(); i++)
	{
		int page = up[i];
		page_to_position[page] = i;

		// Check if there are any conditions on this page value
		if (!conditional_key_before_values.count(page))
			continue;
		// Iterate through our stored conditions and check every page that is required to be after the current page
		// Check if the 'after' pages have been seen before, and if they've appeared before the current page
		for (auto page_after : conditional_key_before_values.at(page))
		{
			if (page_to_position.count(page_after) && page_to_position.at(page_after) < i)
			{
				return false;
			}
		}
	}

	return true;
}

int PrintOrdering::get_center_sum_from_correct_updates() const
{
	int sum = 0;
	for (int i = 0; i < num_updates(); i++)
	{
		if (check_update(i))
		{
			sum += all_updates[i][all_updates[i].size() / 2];
		}
	}
	return sum;
}

bool PrintOrdering::printBefore(int lhs, int rhs) const
{
	if (!conditional_key_before_values.count(lhs))
		return false;
	vector<int> afters = conditional_key_before_values.at(lhs);
	return count(afters.begin(), afters.end(), rhs);
}

update PrintOrdering::sort_update(const update &up) const
{
	update copied(up);
	sort(copied.begin(), copied.end(), [*this](const int &lhs, const int &rhs)
		 { return this->printBefore(lhs, rhs); });
	return copied;
}

int PrintOrdering::reorder_incorrect_sum_median() const
{
	int sum = 0;
	for (int i = 0; i < num_updates(); i++)
	{
		if (!check_update(i))
		{
			update sorted = sort_update(all_updates[i]);
			sum += sorted[sorted.size() / 2];
		}
	}
	return sum;
}

int main()
{
	string input = "input.txt";

	PrintOrdering po;
	po.parse_conditional_file(input);
	cout << "Sum of middle of correct updates: " << po.get_center_sum_from_correct_updates() << endl;
	cout << "Sum of middle of incorrect updates resorted to be corred: " << po.reorder_incorrect_sum_median() << endl;
}