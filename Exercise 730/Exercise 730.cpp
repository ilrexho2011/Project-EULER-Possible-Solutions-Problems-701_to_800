#include <iostream>
#include <vector>
#include <unordered_map>
#include <atomic>
#include <thread>
#include <set>
#include <time.h>
#pragma warning(pop)

constexpr int8_t N_exp = 8;
constexpr int8_t M = 100;

constexpr int64_t int_pow(int64_t b, int8_t e)
{
    int64_t a = 1;
    for (int8_t ix = 0; ix < e; ++ix)
    {
        a *= b;
    }
    return a;
}

constexpr int64_t N = int_pow(10, N_exp);

constexpr int64_t gcd(int64_t a, int64_t b)
{
    while (b != 0)
    {
        a %= b;
        if (a == 0)
        {
            return b;
        }
        b %= a;
    }
    return a;
}

constexpr int64_t m_pow(int64_t n, int64_t e, int64_t m)
{
    int64_t a = 1;
    n %= m;
    if (n < 0)
    {
        n += m;
    }

    if (n == 0)
    {
        return 0;
    }

    while (e > 0)
    {
        if ((e % 2) == 1)
        {
            a = (a * n) % m;
        }
        e >>= 1;
        n = (n * n) % m;
    }
    return a;
}

constexpr int64_t legendre_symbol(int64_t a, int64_t p)
{
    return m_pow(a, (p - 1) / 2, p);
}

constexpr int64_t modinv(int64_t n, int64_t m)
{
    int64_t rzero = n;
    int64_t rone = m;

    int64_t szero = 1;
    int64_t sone = 0;

    while (rone != 0)
    {
        int64_t q = rzero / rone;

        int64_t temp = rone;
        rone = rzero - q * rone;
        rzero = temp;

        temp = sone;
        sone = szero - q * sone;
        szero = temp;
    }

    int64_t ans = szero % m;
    if (ans < 0)
    {
        ans += m;
    }
    return ans;
}

constexpr int64_t tonelli_shanks(int64_t n, int64_t p)
{
    int64_t Q = p - 1;
    int64_t S = 0;

    while ((Q % 2) == 0)
    {
        Q /= 2;
        S += 1;
    }

    int64_t z = 2;
    while (legendre_symbol(z, p) == 1)
    {
        z += 1;
    }

    int64_t m = S;
    int64_t c = m_pow(z, Q, p);
    int64_t t = m_pow(n, Q, p);
    int64_t R = m_pow(n, (Q + 1) / 2, p);

    while (true)
    {
        if (t == 0)
        {
            return 0;
        }

        if (t == 1)
        {
            return R;
        }

        int64_t w = t;
        int64_t i = 0;

        while (w != 1)
        {
            i += 1;
            w = m_pow(w, 2, p);
        }

        int64_t b = m_pow(c, static_cast<int64_t>(1) << (m - i - 1), p);
        m = i;
        c = m_pow(b, 2, p);
        t = (t * c) % p;
        R = (R * b) % p;
    }
}

constexpr int64_t CRT(int64_t a, int64_t m, int64_t b, int64_t n)
{
    int64_t k = modinv(m, n) * (b - a);

    int64_t ans = (a + k * m) % (m * n);
    if (ans < 0)
    {
        ans += m * n;
    }
    return ans;
 }

struct factor_chunk
{
    uint32_t primes[4];
    int8_t exponents[4];
};

struct prime_power {
    uint32_t prime;
    int8_t exponent;
};

class prime_factorization
{
private:
    std::vector<factor_chunk> prime_factors;
    std::vector<uint8_t> num_primes;
    std::unordered_map<uint32_t, factor_chunk> extra_primes;
public:
    void init(void)
    {
        prime_factors.resize(N+1);
        num_primes.resize(N+1);

        for (uint32_t ix = 2; ix <= N; ++ix)
        {
            if (num_primes[ix] == 0)
            {
                int8_t exp = 1;
                for (uint64_t pp = ix; pp <= N; pp *= ix, exp += 1)
                {
                    uint64_t next_pp = pp * ix;
                    for (uint64_t mpp = pp; mpp <= N; mpp += pp)
                    {
                        if ((mpp % next_pp) == 0)
                        {
                            continue;
                        }

                        if (num_primes[mpp] < 4)
                        {
                            prime_factors[mpp].primes[num_primes[mpp]] = ix;
                            prime_factors[mpp].exponents[num_primes[mpp]] = exp;
                        }
                        else
                        {
                            extra_primes[static_cast<uint32_t>(mpp)].primes[num_primes[mpp] - 4] = ix;
                            extra_primes[static_cast<uint32_t>(mpp)].exponents[num_primes[mpp] - 4] = exp;
                        }
                        num_primes[mpp] += 1;
                    }
                }
            }
        }
    }

    std::vector<prime_power> get_factors(uint32_t x)
    {
        std::vector<prime_power> x_factors;
        x_factors.resize(num_primes[x]);

        for (uint8_t ix = 0; ix < std::min(static_cast<uint8_t>(4), num_primes[x]); ++ix)
        {
            x_factors[ix].prime = prime_factors[x].primes[ix];
            x_factors[ix].exponent = prime_factors[x].exponents[ix];
        }

        for (uint8_t ix = 4; ix < num_primes[x]; ++ix)
        {
            x_factors[ix].prime = extra_primes[x].primes[ix - 4];
            x_factors[ix].exponent = extra_primes[x].exponents[ix - 4];
        }

        return x_factors;
    }
};

using sqrt_dict = std::unordered_map<int64_t, std::vector<int64_t>>;

std::vector<int64_t> m_sqrt_single(const prime_power& ppow, int64_t k, sqrt_dict& cache)
{
    int64_t ppow_calc = int_pow(ppow.prime, ppow.exponent);
    if (1 == cache.count(ppow_calc))
    {
        return cache[ppow_calc];
    }

    std::vector<int64_t> all_sqrts;

    if ((ppow.exponent == 1) && (ppow.prime != 2))
    {
        if (legendre_symbol(-k, ppow.prime) > 1)
        {
            return all_sqrts;
        }
    }

    if (ppow.exponent == 1)
    {
        if (ppow.prime == 2)
        {
            if ((k % 2) == 1)
            {
                all_sqrts.push_back(1);
            }
            else
            {
                all_sqrts.push_back(0);
            }
        }
        else
        {
            int64_t r = tonelli_shanks(-k, ppow.prime);
            all_sqrts.push_back(r);
            if (r != 0)
            {
                all_sqrts.push_back(ppow.prime - r);
            }
        }
    }
    else
    {
        prime_power smaller_exp;
        smaller_exp.prime = ppow.prime;
        smaller_exp.exponent = ppow.exponent - 1;
        std::vector<int64_t> unlifted_ans = m_sqrt_single(smaller_exp, k, cache);
        all_sqrts.reserve(unlifted_ans.size());
        for (int64_t r : unlifted_ans)
        {
            int64_t f_prime = r * 2;
            if ((f_prime % ppow.prime) == 0)
            {
                int64_t lower_exp = ppow_calc / ppow.prime;
                if ((r * r + k) % ppow_calc == 0)
                {
                    for (int64_t ix = 0; ix < ppow.prime; ++ix)
                    {
                        int64_t new_r = (r + ix * lower_exp) % ppow_calc;
                        all_sqrts.push_back(new_r);
                    }
                }
            }
            else
            {
                int64_t lower_exp = ppow_calc / ppow.prime;
                int64_t new_r = r - ((r * r + k) % ppow_calc) * modinv(f_prime, lower_exp);
                new_r %= ppow_calc;
                if (new_r < 0)
                {
                    new_r += ppow_calc;
                }
                all_sqrts.push_back(new_r);
            }
        }
    }

    if (ppow_calc <= N / 2)
    {
        cache[ppow_calc] = all_sqrts;
    }
    return all_sqrts;
}

std::vector<int64_t> m_sqrt(const std::vector<prime_power>& factors, int64_t k, sqrt_dict& cache)
{
    std::vector<int64_t> all_sqrts;
    all_sqrts.push_back(0);
    int64_t current_mod = 1;

    for (auto&& ppow : factors)
    {
        int64_t ppow_calc = int_pow(ppow.prime, ppow.exponent);
        std::vector<int64_t> new_sqrts;
        std::vector<int64_t> single_sqrts = m_sqrt_single(ppow, k, cache);
        new_sqrts.reserve(single_sqrts.size() * all_sqrts.size());
        for (int64_t r : single_sqrts)
        {
            for (int64_t a : all_sqrts)
            {
                new_sqrts.push_back(CRT(a, current_mod, r, ppow_calc));
            }
        }
        all_sqrts = std::move(new_sqrts);
        current_mod *= ppow_calc;
    }

    return all_sqrts;
}

prime_factorization factorization;
std::atomic<int64_t> total = 0;
std::atomic<uint8_t> threads_running = 0;

void zero_thread(void)
{
    int64_t thread_total = 0;

    for (int64_t r = 2; ((2 * r * r + 2 * r) <= N); ++r)
    {
        for (int64_t s = 1; (s < r) && ((2 * r * r + 2 * r * s) <= N); ++s)
        {
            if (gcd(r, s) != 1)
            {
                continue;
            }

            if (((r % 2) == 1) && ((s % 2) == 1))
            {
                continue;
            }

            thread_total += 1;
        }
    }

    total += thread_total;
    threads_running--;
}


void thread_function(int64_t k)
{
    sqrt_dict cache;
    int64_t thread_total = 0;

    for (int64_t w = 1; w <= N; ++w)
    {
        for (int64_t x : m_sqrt(factorization.get_factors(static_cast<uint32_t>(w)), k, cache))
        {
            int64_t p = w * w - x * x - k;

            if ((p > 0) && (p % (w * 2) == 0))
            {
                p /= w * 2;

                if ((w + x <= N)  && (p <= x) && (gcd(p, gcd(x, k)) == 1))
                {
                    thread_total += 1;
                }
            }
        }
    }

    total += thread_total;
    threads_running--;
}


int main()
{
    time_t start = time(nullptr);

    const auto max_threads = std::thread::hardware_concurrency();
    std::chrono::milliseconds sleep_time(100);

    std::vector<std::thread> all_threads;
    all_threads.reserve(M + 1);

    threads_running++;
    all_threads.push_back(std::thread(zero_thread));

    factorization.init();

    for (int64_t ix = 1; ix <= M; ++ix)
    {
        while (threads_running >= max_threads)
        {
            std::this_thread::sleep_for(sleep_time);
        }
        threads_running++;
        all_threads.push_back(std::thread(thread_function, ix));
    }

    for (auto&& th : all_threads)
    {
        th.join();
    }

    std::cout << "Answer: " << total << std::endl;
    std::cout << "Time: " << difftime(time(nullptr), start) << std::endl;

    return 0;
}