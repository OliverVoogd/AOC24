#include <vector>
#include <string>
#include <cmath>
#include <iostream>

bool calc_out(const std::vector<int> goal, int A)
{
    std::vector<int> res;
    for (int i = 0; i < goal.size(); i++)
    {
        int t = (A % 8) ^ 5;
        int out = ((t ^ 6) ^ (int)(A / std::pow(2, t))) % 8;
        A = A / std::pow(2, 3);
        // A = A / std::pow(2, 3);
        // int out = A % 8;

        res.push_back(out);
        if (out != goal[i])
            return false;
    }

    for (auto x : res)
    {
        std::cout << ", " << x;
    }
    std::cout << std::endl;
    return true;
}

int main()
{
    std::vector<int> init = {2, 4, 1, 5, 7, 5, 1, 6, 0, 3, 4, 3, 5, 5, 3, 0};

    // I_M= 2147483647
    int A = 656000000;
    while (true)
    {
        if (calc_out(init, A))
        {
            std::cout << "Found at " << A << std::endl;
            break;
        }
        A++;
        if (A % 1000000 == 0)
        {
            std::cout << "Searched " << A << std::endl;
        }
    }

    return 0;
}