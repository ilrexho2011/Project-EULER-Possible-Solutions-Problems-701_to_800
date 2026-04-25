// Returns log (binom(2n, n) / 2^(2n+1))
double f(Int n)
{
    return boost::math::lgamma(2 * n + 1) - 2 * boost::math::lgamma(n + 1) - numbers::ln2 * (2 * n + 1);
}

int main()
{
    cout << f(127311223) - log(1.0 / 39998) << "\n";
    cout << f(127311222) - log(1.0 / 39998) << "\n";
}