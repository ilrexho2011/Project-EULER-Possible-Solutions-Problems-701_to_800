#include <cstdio>
#include <ctime>

using namespace std;

typedef long long int llint;


const int DATA_SIZE = 60;

struct Item {
    llint start, end;
    llint size;
    llint sum;
};

Item items[DATA_SIZE];
int total_item;

llint slove_seg(llint n, int idx) {
    if (n==0) {
        return 0;
    }
    if (n == items[idx].size) {
        return items[idx].sum;
    }

    llint size = items[idx - 1].size;
    if (n < size) { // 左边一半
        return n + slove_seg(n, idx - 1);
    }

    llint ret = size + items[idx-1].sum;
    n -= size;
    if (n>0) {
        ret += n + slove_seg(n, idx - 1);
    }

    return ret;
}

void init() {

    int i = 0;

    items[i].start = 1;
    items[i].size = 1;
    items[i].end = items[i].start + items[i].size - 1;
    items[i].sum = 0;

    for (i=1; i<DATA_SIZE; ++i) {
        items[i].size  = items[i-1].size * 2;
        items[i].start = items[i-1].end + 1;
        items[i].end   = items[i].start + items[i].size - 1;
        items[i].sum   = 2 * items[i-1].sum + items[i].size - 1;
    }
}

llint slove(llint n) {
    int idx = 0;
    llint ans = 0;
    while (items[idx].end <= n) {
        ans += items[idx].sum;
        idx++;
    }

    ans += slove_seg(n - items[idx-1].end, idx);
    return ans;
}


int main() {
    clock_t start_time = clock();


    init();

    llint n = 1e16;
    // scanf("%lld", &n);
    printf("ans S(%lld) = %lld\n", n, slove(n));

    clock_t end_time = clock();
    printf("process end, use time %.0f ms\n", (double)(end_time - start_time) / CLOCKS_PER_SEC * 1000);

    return 0;
}
