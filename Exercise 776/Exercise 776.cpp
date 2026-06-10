#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define PB776_MAX 1234567890123456789LL
#define PB776_NBDIG 19

int main(int argc, const char * argv[]) {
    clock_t nbClock = clock() ;
    int digit[PB776_NBDIG] ;
    int64_t vMax = PB776_MAX ;
    for(int i =1;i<=PB776_NBDIG;i++) {
        digit[PB776_NBDIG-i] = vMax % 10 ;
        vMax /= 10 ;
    }
    double S ;
    double Sum[(PB776_NBDIG*9+1];
    int64_t Nb[(PB776_NBDIG*9+1];
    memset(Sum,0,sizeof(Sum));
    memset(Nb,0,sizeof(Nb));
    for(int id=0;id<digit[0];id++) {
        Nb[id] ++ ;
        Sum[id] += id ;
    }
    int sumD = digit[0] ;
    uint64_t NL =  digit[0]  ;
    for(int nd=1;nd<PB776_NBDIG;nd++) {
        for(int is=nd*9;is-- > 0;) {
            int64_t nb = Nb[is] ;
            if(nb) {
                double Sum10 = Sum[is]*10 ;
                Sum[is] = Sum10 ; // id = 0
                // save value in Sum10 to allow inplace recursion
                for(int id=1;id<10;id++) {
                    Nb[is+id] += nb ;
                    Sum[is+id] += Sum10 + nb*id ;
                }
            }
        }
        NL = NL*10 ;
        for(int id=0;id<digit[nd];id++) {
            Nb[sumD+id] ++ ;
            Sum[sumD+id] += NL + id ;
        }
        sumD += digit[nd] ;
        NL +=  digit[nd]  ;
    }
    S = 0 ;
    for(int is=1;is<= PB776_NBDIG*9;is++) {
        S += Sum[is]/ (double) is ;
        //        printf("S(%d)=%.3f(x%lld)S=%.1f ",is,Sum[is],Nb[is],S);
    }
    S += (double)NL /sumD ;
    //    }
    printf("For %lld sum=%.12e in %.6fs\n",PB776_MAX,S,(float)(clock() - nbClock)/ CLOCKS_PER_SEC) ;
    return 0 ;
}