// fast exponentiation
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

typedef struct FRACTRED64 {
    int64_t n ;
    int64_t d ;
} FRACTRED64 ;

static int64_t Bezout64_pow2(FRACTRED64 fr1,int64_t mod) {
    int64_t s0 = 1, s1 = 0;
    int64_t t0 = 0, t1 = -1 ;
    int64_t n1 = fr1.n ;
    int64_t d1 = fr1.d ;
    int64_t S0=1,S1=0 ;
    int64_t T0=0, T1=mod-1;
    //   int64_t A = modPow(2,fr1.d,mod)-1 ;
    //   int64_t B = modPow(2,fr1.n,mod)-1 ;
    
    do { // euclid
        int64_t q = (int64_t) (d1 / n1) ;
        int64_t  tmp = d1 - q * n1 ;
        d1 = n1 ;  n1 =  tmp ;
        tmp = s0 + q * s1  ;
        s0 = s1 ; s1 = tmp ;
        tmp = t0  + q * t1 ;
        t0 = t1 ; t1 = tmp ;

        // parallel Euclid for power of 2.
        int64_t Q = modPow(2, (int64_t)n1, mod); // Q = 2**n1
        int64_t pow2_d1 = modPow(2,d1,mod) ; // 2**d1
        Q = ( Q * (modPow(pow2_d1,q,mod)-1) ) % mod ; //2**n1 * ( 2**(q*d1) - 1 )
        int64_t inv_pow2_d1 = modPow(pow2_d1-1,mod-2,mod) ;
        Q =  ( Q * inv_pow2_d1) % mod ;// 2**n1 * ( 2**(q*d1) - 1 ) / (2**d1 - 1)
        
        int64_t TMP = (S0 + Q * S1) % mod ;
        S0 = S1 ; S1 = TMP ;
        TMP = (T0 + Q * T1) % mod ;
        T0 = T1 ; T1 = TMP ;
    } while ( n1 ) ;
    //    int64_t chk = (A * S0 - B * T0 ) % mod ;
    int64_t nbp= ((S0+mod-T0-1)*2) % mod ;
    return nbp ;
}


#define PB758_PMAX  1000
int main() {
    int64_t S=0 ;
    int32_t *tbPrime=calloc(PB758_PMAX,sizeof(tbPrime[0]));
    int32_t nbPrime = 0 ;
    for(int i=2;i<PB758_PMAX;i++) { // erastosthene
        if(tbPrime[i]==0) {
            tbPrime[nbPrime++]= i ;
            for(int j=2*i;j<PB758_PMAX;j+=i) tbPrime[j] = 1 ;
        }
    }
    for(int ip=0;ip<nbPrime;ip++) {
        int32_t p = tbPrime[ip];
        for(int iq=ip+1;iq<nbPrime;iq++) {
            int32_t q = tbPrime[iq] ;
            FRACTRED64 pq2 = (FRACTRED64){(int64_t)p*p*p*p*p,(int64_t)q*q*q*q*q} ;
            int64_t nbP = Bezout64_pow2(pq2,1000000007) ;
            S = (S + nbP) % 1000000007 ;
        }
    }
    free(tbPrime);
    printf("S=%lld\n",S) ;
    return 1 ;
}