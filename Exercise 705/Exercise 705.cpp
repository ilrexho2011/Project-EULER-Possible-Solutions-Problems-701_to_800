#include <vector>
#include <cstdint>
#include <array>
#include <numeric>
#include <cstddef>
#include <primesieve.hpp>

namespace Euler::Problem705 {
    using std::uint64_t, std::uint8_t, std::vector;

    template<uint64_t N>
    bool nextDigit(primesieve::iterator &primes, vector<uint8_t> &buffer, uint8_t &digit) {
        if (buffer.empty()) {
            uint64_t prime = primes.next_prime();
            if (prime >= N) {
                return false;
            }
            while (prime != 0) {
                uint8_t d = prime % 10;
                if (d != 0) {
                    buffer.push_back(d);
                }
                prime /= 10;
            }
        }
        digit = buffer.back();
        buffer.pop_back();
        return true;
    }

    template<uint64_t N = 100'000'000, uint64_t M = 1'000'000'007>
    uint64_t solve() {
        const static std::array<vector<uint8_t>, 9> divisors{
                {{1}, {1, 2}, {1, 3}, {1, 2, 4}, {1, 5}, {1, 2, 3, 6}, {1, 7}, {1, 2, 4, 8}, {1, 3, 9}}};

        primesieve::iterator primes;
        uint8_t digit;
        vector<uint8_t> digitBuffer;
        std::array<uint64_t, 9> counts{};
        uint64_t total = 0;
        uint64_t numSequences = 1;

        while (nextDigit<N>(primes, digitBuffer, digit)) {
            vector<uint8_t> const &digitDivisors = divisors[digit - 1];
            uint64_t numDivisors = digitDivisors.size();
            total *= numDivisors;
            for (uint8_t divisor: digitDivisors) {
                total += std::accumulate(counts.cbegin() + divisor,
                                         counts.cend(), static_cast<uint64_t>(0));
            }
            total %= M;
            for (std::size_t i = 0; i < 9; ++i) {
                counts[i] *= numDivisors;
                counts[i] %= M;
            }
            for (uint8_t divisor: digitDivisors) {
                counts[divisor - 1] += numSequences;
                counts[divisor - 1] %= M;
            }
            numSequences *= numDivisors;
            numSequences %= M;
        }
        return total;
    }
}