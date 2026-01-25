#include <stdio.h>
#include <stdint.h>

#define MODULUS ((uint64_t)1000000007)
#define N ((uint64_t)100000000)

uint32_t invFacMod[N + 1];

uint64_t powMod(uint64_t a, uint64_t b) { //Calculates result= (a^b) mod MODULUS
  uint64_t result= 1;
  while ( b ) {
    if (b & 1) result= (result * a) % MODULUS;
    a= (a * a) % MODULUS;
    b>>= 1;
  }
  return result;
} //powMod()

int main() {
  uint64_t mu, pf= 1, answer= 0, nFacMod= 1;
  uint32_t y= N / 2, pf2= 1, twoExpN_mod= powMod(2, N);
  invFacMod[0]= 1;
  for (uint64_t i= 1; i <= N; i++) {
    nFacMod= (nFacMod * i) % MODULUS;
    invFacMod[i]= powMod(nFacMod, MODULUS - 2); //Fermat's little theorem
  }

  for (uint32_t x= 0; x <= N; x+= 2) {
    mu= invFacMod[y];
    mu= (mu * mu) % MODULUS;
    answer+=(((((nFacMod * invFacMod[x]) % MODULUS) * mu) % MODULUS) * pf2) % MODULUS;
    pf= (pf * twoExpN_mod) % MODULUS;
    pf2= (pf * pf) % MODULUS;
    y--;
  }

  printf("\n\ne743: %lu\n", answer % MODULUS);
  return 0;
}