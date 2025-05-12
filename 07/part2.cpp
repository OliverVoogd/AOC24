#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

class Calibrations
{
private:
	vector<long long> test_values;
	vector<vector<long long>> equations;

	/// @brief Checks if the equation at eq_idx can be made to equal the test value at eq_idx by combining
	/// the operands with either + or *, evaluating from left-to-right ignoring precedence
	/// @param eq_idx the combination of test_values[eq_idx] and equations[eq_idx] to sue
	/// @return true if the equation and test_value can be made equal
	bool can_make_true_using_add_multiply(int eq_idx) const;
	/// @brief Recursively search the possible combination of all 3 operators until we find a valid way to combine the operands to
	/// equal the test value.
	/// @param eq_idx The equation to check
	/// @return true if the equation can be made to produce the test_value, false otherwise.
	bool can_make_true_using_concat(int eq_idx) const;
	static bool can_make_true_am_aux(const long long &value, const vector<long long> &eq, int i);
	static bool can_make_true_cc_aux(const long long &lhs, int i, const long long &test_value, const vector<long long> &eq);

public:
	/// @brief Parse an input file which consists of lines in the format <test value>: <operand> <operand> ...
	/// Clears the current stored vectors test_values and equations and populates them.
	/// @param filename The input file to parse
	void parse_calibrations(const string &filename);

	/// @brief Check every equation using Calibrations::can_make_true_using_add_multiply to determine if we can make the equation true, and if
	/// we should include the corresponding test value in our calibration sum.
	/// @return The sum of the test values for equations that can be made true.
	long long sum_values_for_true_equations_only_add_multiply() const;
	/// @brief Check every equation first using can_make_true_using_add_multiply (for speed) and then using can_make_true_using_concat to sum all
	/// equations which can be accepted by combining operands with addition, multiplication and subtraction.
	/// @return The sum of the test values for equations that can be made true using all three operators.
	long long sum_values_for_true_equations_including_concat() const;
};

void Calibrations::parse_calibrations(const string &filename)
{
	test_values.clear();
	equations.clear();
	ifstream file(filename);
	string line;

	while (getline(file, line))
	{
		stringstream eq(line);
		// Get the test value from the full equation string stream
		getline(eq, line, ':');

		test_values.push_back(stoll(line));
		equations.push_back({});
		// Get the list of operands from the rest of the string stream
		while (getline(eq, line, ' '))
		{
			if (line.size())
				equations.back().push_back(stoll(line));
		}
	}
	file.close();
}

bool Calibrations::can_make_true_using_add_multiply(int eq_idx) const
{
	return can_make_true_am_aux(test_values[eq_idx], equations[eq_idx], equations[eq_idx].size() - 1);
}
bool Calibrations::can_make_true_am_aux(const long long &value, const vector<long long> &eq, int i)
{
	// Test we have reached the last value
	// To be able to be successful, we need the last value to equal value
	if (i == 0)
	{
		return eq[i] == value;
	}

	bool possible = false;
	// Check if we are able to combined incorporate eq[i] into the equation using multiplication
	// value mod eq[i] == 0
	if (value % eq[i] == 0)
	{
		possible = can_make_true_am_aux(value / eq[i], eq, i - 1);
	}

	// If that option wasn't successful, we should try again but simpling adding eq[i] instead of multiplying
	if (!possible)
	{
		possible = can_make_true_am_aux(value - eq[i], eq, i - 1);
	}

	return possible;
}

bool Calibrations::can_make_true_using_concat(int eq_idx) const
{
	return can_make_true_cc_aux(equations[eq_idx][0], 1, test_values[eq_idx], equations[eq_idx]);
}
bool Calibrations::can_make_true_cc_aux(const long long &lhs, int i, const long long &test_value, const vector<long long> &eq)
{
	// Check termination if i has gone past the end of eq
	if (i == eq.size())
	{
		return test_value == lhs;
	}

	bool valid = false;
	// Try concatenating lhs and eq[i]
	valid = can_make_true_cc_aux(stoll(to_string(lhs) + to_string(eq[i])), i + 1, test_value, eq);

	// Try multiplication
	if (!valid)
		valid = can_make_true_cc_aux(lhs * eq[i], i + 1, test_value, eq);

	// Try addition
	if (!valid)
		valid = can_make_true_cc_aux(lhs + eq[i], i + 1, test_value, eq);

	return valid;
}

long long Calibrations::sum_values_for_true_equations_only_add_multiply() const
{
	long long sum = 0;
	for (int i = 0; i < test_values.size(); i++)
	{
		if (can_make_true_using_add_multiply(i))
			sum += test_values[i];
	}
	return sum;
}

long long Calibrations::sum_values_for_true_equations_including_concat() const
{
	long long sum = 0;
	for (int i = 0; i < test_values.size(); i++)
	{
		if (can_make_true_using_add_multiply(i) || can_make_true_using_concat(i))
		{
			sum += test_values[i];
			continue;
		}
	}

	return sum;
}

int main()
{
	string input("input.txt");

	Calibrations cb;
	cb.parse_calibrations(input);

	cout << "Sum of the test values for equations the can be made true: " << cb.sum_values_for_true_equations_including_concat() << endl;
}