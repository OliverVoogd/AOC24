#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;

vector<vector<long long>> concat_permutations(const vector<long long> &eq)
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

int main()
{
	vector<long long> eq = {11, 6, 16, 20};

	print_vec(eq);
	auto perms = concat_permutations(eq);
	for (auto p : perms)
	{
		print_vec(p);
	}
}