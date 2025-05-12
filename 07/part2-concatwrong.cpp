#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

template <typename T>
void print_vec(const vector<T> &vec)
{
	cout << vec.front();
	for (int i = 1; i < vec.size(); i++)
	{
		cout << ", " << vec[i];
	}
	cout << endl;
}

void print_t(long long t, vector<long long> eq)
{
	cout << t << ": ";
	print_vec(eq);
}

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
	bool can_make_true_using_add_multiply(const long long &value, const vector<long long> &eq) const;
	static bool can_make_true_am_aux(const long long &value, const vector<long long> &eq, int i);
	static vector<vector<long long>> create_eq_concat_permutations(const vector<long long> &eq);

public:
	/// @brief Parse an input file which consists of lines in the format <test value>: <operand> <operand> ...
	/// Clears the current stored vectors test_values and equations and populates them.
	/// @param filename The input file to parse
	void parse_calibrations(const string &filename);

	/// @brief Check every equation using Calibrations::can_make_true to determine if we can make the equation true, and if
	/// we should include the corresponding test value in our calibration sum.
	/// @return The sum of the test values for equations that can be made true.
	long long sum_values_for_true_equations() const;

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
bool Calibrations::can_make_true_using_add_multiply(const long long &value, const vector<long long> &eq) const
{
	return can_make_true_am_aux(value, eq, eq.size() - 1);
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

vector<vector<long long>> Calibrations::create_eq_concat_permutations(const vector<long long> &eq)
{
	vector<vector<long long>> permutations;
	int n = eq.size() - 1;
	unsigned bit_max = (1 << n) - 1;

	// Iterate through all possible permutations of our concatenation values.
	// We ignore the permutation where no values are concatenated, as we assume that's already been checked.
	for (unsigned bits = 1; bits <= bit_max; bits++)
	{
		vector<long long> this_perm;
		unsigned b_concat = bits;
		long long rhs = eq.back();
		// Iterate through the bits of the current permuation, and concatenate the value between 1 bits
		// So if our 'bits' permuation is 101 for a 4 length vector v, we need to concatenate v[0] and v[1], v[2] and v[3]
		// So if bits is our permutation, and v is our vector, bits[i] concatenates v[i] and v[i + 1] for all i in range 0 to |bits| = |v| - 1
		for (int i = n - 1; i >= 0; i--)
		{
			if (b_concat % 2 == 1)
			{
				rhs = stoll(to_string(eq[i]) + to_string(rhs));
			}
			else
			{
				this_perm.push_back(rhs);
				rhs = eq[i];
			}
			b_concat = b_concat >> 1;
		}
		this_perm.push_back(rhs);
		reverse(this_perm.begin(), this_perm.end());
		permutations.push_back(this_perm);
	}

	return permutations;
}

long long Calibrations::sum_values_for_true_equations() const
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
		// if (can_make_true_using_add_multiply(i))
		// {
		// 	sum += test_values[i];
		// 	continue;
		// }

		if (can_make_true_using_add_multiply(i))
		{
			continue;
		}

		// If we can't make it true using only addition and multiplication, we need to check if concatinating any pairs of values gives us a viable result.
		for (auto eq : create_eq_concat_permutations(equations[i]))
		{
			if (can_make_true_using_add_multiply(test_values[i], eq))
			{
				cout << "\t";
				print_vec(eq);
				sum += test_values[i];
				break;
			}
		}
		print_t(test_values[i], equations[i]);
	}

	return sum;
}

int main()
{
	string input("input.txt");

	Calibrations cb;
	cb.parse_calibrations(input);

	cout << "Sum of the test values for equations the can be made true: \n"
		 << cb.sum_values_for_true_equations_including_concat() << endl;
}