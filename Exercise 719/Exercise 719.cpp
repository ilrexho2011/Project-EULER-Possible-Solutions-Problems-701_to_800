#include <iostream>

using namespace std;

typedef long long INT;

bool is_S_number(INT num, INT root)
{
    uint max_permutations = 1;
    for (INT n = num; n > 9; n /= 10)
    {
        max_permutations <<= 1;
    }
    for (uint perm = 0; perm < max_permutations; ++perm)
    {
        INT div = 10;
        INT n = num;
        INT sum = 0;
        uint p = perm;
        while (n > 0)
        {
            if ((p & 1) == 0)
            {
                sum += n % div;
                n /= div;
                div = 10;
            }
            else
            {
                div *= 10;
            }
            p >>= 1;
        }
        if (sum == root)
        {
            return true;
        }
    }
    return false;
}

int main(void)
{
    INT sum = 0;
    for (INT root = 2; root <= 1000000; ++root)
    {
        INT square = root * root;
        if (is_S_number(square, root))
        {
            sum += square;
        }
    }
    cout << sum << endl;
    return 0;
}