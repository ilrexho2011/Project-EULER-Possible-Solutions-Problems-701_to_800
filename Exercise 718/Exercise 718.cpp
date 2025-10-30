#include "stdafx.h"
#include <string>
#include <fstream>
#include <iostream>
#include "math.h"
#include <time.h>
#include <set>
#include <vector>
#include <algorithm>
using namespace std;

// PROBLEM 718
const __int64 MOD = 1000000007;

/*
const int a = 17;
const int b = 19;
const int c = 23;
*/

/*
const int a = 289;
const int b = 361;
const int c = 529;
*/


const __int64 a =  24137569;
const __int64 b =  47045881;
const __int64 c = 148035889;


int _tmain(int argc, _TCHAR* argv[]){
   clock_t time0 = clock();

   __int64 * firstR = new __int64[a];
   for (int i = 0; i < a; i++) firstR[i] = 1000000000000000000L;

   // create queue B with one entry, a+b+c
   __int64* toExploreB = new __int64[a+10];
   toExploreB[0] = a+b+c;
   int exitB = 0;
   int enterB = 1;

   // create empty queue C
   __int64* toExploreC = new __int64[a+10];
   int exitC = 0;
   int enterC = 0;

   while (exitB < enterB || exitC < enterC ) {
      __int64 smallest = 1000000000000000000L;
      if (exitB < enterB) smallest = toExploreB[exitB];
      if (exitC < enterC) {
         if (toExploreC[exitC] < smallest) {
            smallest = toExploreC[exitC];
            exitC++;
         }else if(toExploreC[exitC] == smallest) {
            exitB++;
            exitC++;
         }else{
            exitB++;
         }
      }else{
         exitB++;
      }

      __int64 ind = smallest % a;
      if (firstR[ind] > smallest) {
         firstR[ind] = smallest;
         toExploreB[enterB++] = smallest + b;
         toExploreC[enterC++] = smallest + c;
      }
   }

   __int64 sum = 0;
   for (int i = 0; i < a; i++) {
      __int64 n = (firstR[i] / a) % MOD;
      sum = (sum + n*i) % MOD;
      sum = (sum + a * ((n*(n-1)/2) % MOD)) % MOD;
   }

   cout << "sum = " << sum << "\n";
   cout << "exitB = " << exitB << "\n";
   cout << "exitC = " << exitC << "\n";
   cout << "time spent = " << clock() - time0 << " ms\n"; cout << "press any key to end\n"; char cinget; cin.get(cinget); 
   return 0;
}