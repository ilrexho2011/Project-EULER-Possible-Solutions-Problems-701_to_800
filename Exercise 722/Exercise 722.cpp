#include <iostream>
#include <cmath>
#include <cfloat>

using namespace std;

const int XP=25;
const int k=15;

int main()
{
    long double a,b,sum,term;
    uint64_t i;
    b=pow(2.0L,XP);
    a=b-1;
    sum=0;
    //cout.precision(LDBL_DIG);
    cout.precision(12);
    cout << scientific;
    for(i=10000000000ULL;i>=1;i--)
    {
        term=pow(i,k)/(pow(b/a,i)-1);
        sum+=term;
        //cout << i << " " << term << " " << sum << endl;
    }
    cout << sum << endl;
    return 0;
}