vector<num> freq(n);
freq[0] = 1;
for(int k = 1; k < n; k++){
	freq[k] = freq[k-1] + ncr((n + k) / 2 - 1, k);
	if(k >= 2 && (n + k) % 2 == 0) freq[k] += freq[k-2];
}
for(int k = 0; k < n; k++) freq[k] += ncr((n + k) / 2 - 1, k-1);
freq[n-1] += 1;
int B = 2;
while(B < n) B <<= 1;
vector<num> cnt(B, 0);
for(int i = 0; i < n; i++){
	cnt[n-1-i] += freq[i];
}
FST(cnt, 0);
for(num& x : cnt){
	x = pow(x, s);
}
FST(cnt, 1);
num ans = cnt[0];
