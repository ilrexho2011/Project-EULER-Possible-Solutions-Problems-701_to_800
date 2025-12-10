#include <iostream>
#include <iomanip>
#include <vector>
#include <boost/multiprecision/cpp_int.hpp>

using boost::multiprecision::cpp_int;

// Computes (10^exp) mod modVal using exponentiation by squaring in big integers.
static cpp_int modExp10(const cpp_int& exp, const cpp_int& modVal)
{
    cpp_int result = 1;
    cpp_int base   = 10 % modVal;
    cpp_int e      = exp;
    
    while (e > 0) {
        if ((e & 1) != 0)
            result = (result * base) % modVal;
        base = (base * base) % modVal;
        e >>= 1;
    }
    return result;
}

int main()
{
    // We want A(10^16) = floor(A * 10^(10^16 + 9)) mod 10^10,
    // where A = sum_{i=1..∞} [1 / (3^i * 10^(3^i))].
    //
    // Let M = 10^16 + 9. We'll sum i=1..m where 3^m <= M < 3^(m+1).

    const unsigned long long N = 10000000000000000ULL; // 10^16
    const unsigned long long M = N + 9ULL;             // 10^16 + 9

    // Precompute powers of 3 up to 3^m <= M
    std::vector<cpp_int> p3;
    p3.push_back(1); // 3^0
    {
        cpp_int current = 1;
        while(true){
            cpp_int nxt = current * 3;
            if(nxt > M) break;
            p3.push_back(nxt);
            current = nxt;
        }
    }
    int m = static_cast<int>(p3.size() - 1); // largest i with 3^i <= M

    const unsigned long long MOD10_10 = 10000000000ULL; // 10^10

    // We'll accumulate the integer part in sumIntMod (mod 10^10).
    unsigned long long sumIntMod = 0ULL;

    // We'll sum fractional parts in fractionSum with common denominator 3^m.
    // fractionSum = Σ remainder_i * 3^(m-i), taken mod 3^m.
    // But we must do a local carry each time fractionSum >= 3^m.
    cpp_int p3m = p3[m];
    cpp_int fractionSum = 0;  // 0 <= fractionSum < some multiple of 3^m

    for(int i = 1; i <= m; i++){
        // Denominator: 3^i
        // term = 10^(M - 3^i) / 3^i
        // We'll do T_i = 10^(M - p3[i]) mod [3^i * 10^10] to extract the part that matters mod 10^10.
        
        cpp_int p3i = p3[i];
        cpp_int modVal = p3i * MOD10_10;     // can be ~5.5e25 for i ~ 33
        cpp_int exponent = cpp_int(M) - p3i; // M - 3^i >= 0 for i <= m
        
        // T_i = 10^exponent mod modVal
        cpp_int T_i = modExp10(exponent, modVal);

        // integerPart_i = floor(T_i / 3^i)
        // remainder_i   = T_i % 3^i
        cpp_int integerPart_i = T_i / p3i;
        cpp_int remainder_i   = T_i % p3i;  // the fraction's numerator

        // Add integerPart_i to sumIntMod (mod 10^10)
        // (Convert integerPart_i mod 10^10 into 64 bits, then add.)
        cpp_int integerPart_modcpp = integerPart_i % cpp_int(MOD10_10);
        unsigned long long integerPart_mod =
            integerPart_modcpp.convert_to<unsigned long long>();
        sumIntMod = (sumIntMod + integerPart_mod) % MOD10_10;

        // fractional part: remainder_i / 3^i => remainder_i * 3^(m-i) / 3^m
        cpp_int fracPart = remainder_i * p3[m - i];
        fractionSum += fracPart;
        
        // Now handle any carry from fractionSum if fractionSum >= 3^m.
        cpp_int carryNow = fractionSum / p3m;  // how many multiples of 3^m
        fractionSum %= p3m;                   // keep remainder
        if(carryNow != 0){
            // Add that carry to sumIntMod (mod 10^10).
            // carryNow might be > 1 if fractionSum wrapped multiple times.
            cpp_int carryMod = carryNow % cpp_int(MOD10_10);
            unsigned long long carry_ull = carryMod.convert_to<unsigned long long>();
            sumIntMod = (sumIntMod + carry_ull) % MOD10_10;
        }
    }

    // After all i from 1..m, fractionSum < 3^m, so there's no additional integer carry.
    // sumIntMod now holds floor( Σ_{i=1..m} [10^(M - 3^i) / 3^i ] ) mod 10^10.
    // Terms for i>m are negligible for the first 10 digits at that position.

    // Print with leading zeros.
    std::cout << std::setw(10) << std::setfill('0') << sumIntMod << std::endl;
    return 0;
}