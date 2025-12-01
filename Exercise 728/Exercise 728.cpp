static uint64_t modPow(int64_t a,int64_t exp,uint64_t mod) {
    uint64_t aPow2 = a % mod ;
    uint64_t aPowExp = (exp & 1) ? aPow2 : 1 ;
    int i ;
    for(i=1;exp >= (1LL<<i) ;i++) {
        aPow2 = (aPow2 * aPow2) % mod ;
        if( (1LL<<i) & exp) {
            aPowExp = (aPowExp * aPow2 ) % mod  ;
        }
    }
    return aPowExp ;
}

#define PB728_N 10000000
#define PB728_MOD   1000000007
int main(int argc, char **argv) {
    int32_t *phi = malloc((PB728_N+1) * sizeof(phi[0])) ;
    for(int i=0;i<=PB728_N;i++) phi[i]=i ;
    for(int i=2;i<=PB728_N;i++) {
        if(phi[i] == i) { // new prime
            for(int np=i;np<=PB728_N;np+= i) {
                phi[np] = phi[np]/i * (i-1) ;
            }
        }
    }
    int64_t S = 0;
    int64_t Pow2n1 = 1 ;
    for(int n1=1;n1<=PB728_N;n1++) {
        int maxG = PB728_N / n1 ;
        int sumK = (n1 & 1) ? 3*phi[n1]/2 : 2*phi[n1]   ;
        int64_t Sg  ;
        if(maxG > 20) { // computation by inverse for geometric serie
            Sg = modPow(Pow2n1,maxG,PB728_MOD)-1 ;
            Sg = (Sg * modPow(Pow2n1-1,1000000005,PB728_MOD)) % PB728_MOD ;
            Sg = (Sg * Pow2n1)% PB728_MOD ;
        } else { // direct computation for geometric serie
            Sg = Pow2n1 ;
            for(int ig=1; ig<maxG;ig++) { Sg = ((Sg+1)*Pow2n1) % PB728_MOD   ; }
        }
        S = (S + 2 + sumK * Sg) % PB728_MOD ;
        Pow2n1 *= 2 ;
        if(Pow2n1 >=PB728_MOD )Pow2n1 -= PB728_MOD;
    }
    printf("S=%lld",S) ;
    return 0 ;
}