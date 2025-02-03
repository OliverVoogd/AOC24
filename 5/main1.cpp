#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

typedef vector<int> update;
typedef vector<int> pages;

class PrintOrdering
{
private:
	unordered_map<int, pages> conditional_key_before_values;

	vector<update> all_updates;

public:
	void parse_conditional_file(const string &input);

	int get_center_sum_from_correct_updates() const;

	bool check_update(int update_idx) const;

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

int main()
{
	string input = "input.txt";

	PrintOrdering po;
	po.parse_conditional_file(input);
	cout << "Sum of middle of correct updates: " << po.get_center_sum_from_correct_updates() << endl;
}