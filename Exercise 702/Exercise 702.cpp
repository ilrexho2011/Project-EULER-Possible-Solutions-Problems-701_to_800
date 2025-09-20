#include <iostream>

using namespace std;


long pow2(long n){
     long fac=1;
     for(long i=0; i<n; i++){
         fac*=2;
     }
     return fac;
}

long N = 123456789;
long max_level = 28;

long max_Size = pow2(max_level);

long *layers = new long[max_Size];
long *residues = new long[max_Size];
long *nums = new long[max_Size];


long *bests = new long[N];


void ini_heights(long level){
    long level2 = pow2(level);
    long h, res, layer;
    for (long i=0; i<level2; i++) {
        h = N*i;
        residues[i] = h % level2;
        layers[i] = h / level2;
    }
}

// O(N) function after closely observing the pattern of the tricky part
void ini_nums(long level){
    long level2 = pow2(level);
    long pattern_add = (level2 - residues[1]) % level2 - 1;
    long pattern_mod = level2;

    long cur = 0;

    long num_weird, cur_next, t;


    for (long i=0; i<level2; i++) {
        t = (level2-residues[i]) % level2;

        num_weird = cur + t - 1;
        cur_next = (cur+pattern_add) % pattern_mod;
        if (cur > cur_next){
            pattern_add--;
        }
        pattern_mod--;
        cur = cur_next;

        nums[i] = num_weird;

    }
    nums[0] = 0;

}

void ini_bests(){ 
    long num_triangles;
    for (long i=0; i<N; i++){
        num_triangles = 2 * N - i;
        bests[i] = num_triangles * max_level;
    }
}

void update_bests(long level){
    long level2 = pow2(level);
    ini_heights(level);
    ini_nums(level); 

    long cur_layer = -1;
    long cur_duration = 0;
    long cur_triangles, this_triangles, this_layer, cur_num, this_num;
    for (long i=1; i<level2; i++){
        this_layer = layers[i];
        this_triangles = 2*N - this_layer;
        this_num = nums[i];
        if (this_layer != cur_layer) {

            if (cur_duration > 0) {
                bests[cur_layer] = cur_triangles * level;
            } else {
                bests[cur_layer] -= cur_num;
            }
            cur_triangles = this_triangles;
            cur_layer = this_layer;
            cur_num = this_num;
            cur_duration = 0;
        } else {
            cur_duration++;
        }
    } 
    cur_duration++;
    if (cur_duration > 1) {
        bests[cur_layer] = cur_triangles * level;
    } else {
        bests[cur_layer] -= cur_num;
    }
}


int main(){
    ini_bests();

    for (long i = max_level; i > 0; i--){
        update_bests(i);
    }

    long total = 0;
    for (long i=0; i<N; i++){
        total += bests[i];
    }
    cout << total << endl;
}