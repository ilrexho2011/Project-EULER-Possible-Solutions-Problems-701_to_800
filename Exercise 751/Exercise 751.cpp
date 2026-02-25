#include <stdio.h>
#include <gmp.h>

mpf_t mpfTau, mpfTauFloor, mpfNextTau, mpfN, mpfX, mpfD;

void calcNextTau() {
  mpf_set_ui(mpfN, 1);
  mpf_floor(mpfNextTau, mpfTau);
  for(int i= 1; i < 1000; i++) {
    mpf_floor(mpfTauFloor, mpfTau);
    mpf_add_ui(mpfTau, mpfTau, 1);
    mpf_sub(mpfTau, mpfTau, mpfTauFloor);
    mpf_mul(mpfTau, mpfTau, mpfTauFloor);
    mpf_floor(mpfX, mpfTau);

    mpf_set(mpfD, mpfX);
    do {
      mpf_mul_ui(mpfN, mpfN, 10);
      mpf_div_ui(mpfD, mpfD, 10);
    } while( mpf_cmp_ui(mpfD, 1) >= 0 );
    mpf_div(mpfX, mpfX, mpfN);
    mpf_add(mpfNextTau, mpfNextTau, mpfX);
  }
} //calcNextTau()

int main(){
  printf("\nGNU MP version: %s\n\n", gmp_version);

  mpf_set_default_prec(4096);
  mpf_inits(mpfTau, mpfTauFloor, mpfNextTau, mpfN, mpfX, mpfD, NULL);

  mpf_set_d(mpfNextTau, 2.0L);
  do {
    mpf_set(mpfTau, mpfNextTau);
    calcNextTau();
    gmp_printf("%.*Ff\n", 150, mpfNextTau);
  } while( 1 );

  mpf_clears(mpfTau, mpfTauFloor, mpfNextTau, mpfN, mpfX, mpfD, NULL);
} //main()