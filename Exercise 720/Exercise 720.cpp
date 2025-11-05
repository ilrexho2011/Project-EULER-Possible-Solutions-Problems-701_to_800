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

// PROBLEM 720 Unpredictable Permutations

const int N = 33554432;
const int MOD = 1000000007;

bool * used = new bool[N+1];
int  * perm = new int [N+1];
int  * fact = new int [N+1];
int indexToPlaceNext = 1;


class PartialSumCalculator {
public:

   // Rytter's Algorithm? for partial sums
   int ** count;
   int * arraySize;
   int nLevels;
   int N;

   // Constructor to calculate partial sums up to N (included)
   PartialSumCalculator(int N) {
      int s = N + 1;
      nLevels = 0;
      while (s > 0) {
         s = s / 2;
         nLevels++;
      }
      arraySize = new int[nLevels];
      arraySize[0] = N+1;
      count = new int*[nLevels];
      for (int i = 0; i < nLevels; i++) {
         if (i != 0) arraySize[i] = (arraySize[i-1] + 1) / 2;
         count[i] = new int[arraySize[i]];
         for (int j = 0; j < arraySize[i]; j++) count[i][j] = 0;
      }
   }

   // Destructor
   ~PartialSumCalculator() {
      delete[] arraySize;
      for (int i = 0; i < nLevels; i++) delete[] count[i];
      delete[] count;
   }

   void add(int index, int n) {
      for (int level = 0; level < nLevels; level++) {
         count[level][index] += n;
         index = index / 2;
      }
   }

   void increment(int index) {
      add(index, 1);
   }

   int getPartialSum(int n) {
      int sum = 0;
      int level = 0;
      while (n > 0) {
         if (!(n & 1)) {
            sum += count[level][n];
         }
         level++;
         n = (n - 1) / 2;
      }
      sum += count[level][0];
      return sum;
   }
};


void recursiveSolution(int start, int end, int reflect, int first, int decrement) {
   if (decrement == 1) {
      perm[start] = first;
   }else{
      perm[end] = first;
   }
   if (end == 4) {
      perm[3] = reflect - perm[2];
   }else{
      recursiveSolution(start, end/2, reflect-decrement, (first == 1 ? 2 : 2*first-1), decrement*2);
      for (int i = 1; i < end/2; i++) {
         perm[i+end/2] = reflect - perm[end/2-i+1];
      }
   }
}


bool recursiveSearch() {
   //for (int i = 1; i <= N; i++) cout << perm[i] << " ";
   //cout << "\n";
   if (indexToPlaceNext == N+1) {
      cout << "solution found!\n";
      for (int i = 1; i <= N; i++) cout << perm[i] << " ";
      cout << "\n";
      return true;
   }
   for (int candidate = 1; candidate <= N; candidate++) {
      bool validCandidate = false;
      if (!used[candidate]) {
         validCandidate = true;
         for (int i = 1; i < indexToPlaceNext; i++) {
            int determ = 2*candidate - perm[i];
            if (determ > 0 && determ <= N && !used[determ]) {
               //cout << "   candidate " << candidate << " blocked by " << perm[i] << " because " << determ << " not used yet\n";
               validCandidate = false;
               break;
            }
         }
      }
      if (validCandidate) {
         //cout << "SETTING " << candidate << ", indexToPlaceNext = " << indexToPlaceNext << "\n";
         perm[indexToPlaceNext] = candidate;
         indexToPlaceNext++;
         used[candidate] = true;
         if (recursiveSearch()) return true;
         //cout << "REMOVING " << candidate << " from position " << (indexToPlaceNext-1) << "\n";
         used[candidate] = false;
         indexToPlaceNext--;
         perm[indexToPlaceNext] = 0;
      }
   }
   //cout << "NO SOLUTION FOUND!\n";
   return false;
}


void search() {
   perm[1] = 1;
   used[1] = true;
   indexToPlaceNext = 2;
   recursiveSearch();
}


int _tmain(int argc, _TCHAR* argv[]){
   clock_t time0 = clock();

   // initialization
   for (int i = 0; i <= N; i++) {
      used[i] = false;
      perm[i] = 0;
   }
   fact[N] = 1;
   for (int i = 1; i <= N; i++) {
      fact[N-i] = (int)(((__int64)fact[N+1-i] * i) % MOD);
   }

   // recursive search for small N
   //search();
   
   // recursive solution
   perm[1] = 1;
   perm[2] = N/2 + 1;
   recursiveSolution(1,N,N+1,1,1);
   perm[N] = N;

   // calculate position of permutation
   PartialSumCalculator partialSumCalculator(N);
   __int64 sum = 0;
   for (int i = 1; i <= N; i++) {
      int nBelow = partialSumCalculator.getPartialSum(perm[i] - 1); // number of indexes j such that perm[j] < perm[i]
      sum = (sum + fact[i] * (__int64)(perm[i]-nBelow-1)) % MOD;
      partialSumCalculator.increment(perm[i]);
   }

   cout << "sum = " << (sum+1) << "\n";
   cout << "time spent = " << clock() - time0 << " ms\n"; cout << "press any key to end\n"; char cinget; cin.get(cinget); 
   return 0;
}