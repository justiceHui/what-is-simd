#include <bits/stdc++.h>
#include <immintrin.h>
#pragma GCC target("avx,avx2,fma")
using namespace std;

typedef long long ll;

ll rangeSum(int *a, int s, int e){
    ll ret = 0; int i;
    __m256i avxSum = _mm256_setzero_si256();
    for(i=s; (i&7) && i<=e; i++) ret += a[i];
    for(; i+7<=e; i+=8){
        __m256i now = _mm256_load_si256((__m256i const *)(a+i));
        avxSum = _mm256_add_epi32(avxSum, now);
    }
    for(; i<=e; i++) ret += a[i];
    int tmp[8];
    _mm256_storeu_si256((__m256i*)tmp, avxSum);
    for(i=0; i<8; i++) ret += tmp[i];
    return ret;
}

int rangeMax(int *a, int s, int e){
    int ret = (int)(-1e9-7); int i;
    __m256i avxMax = _mm256_set1_epi32((int)(-1e9-7));
    for(i=s; (i&7) && i<=e; i++) ret = max(ret, a[i]);
    for(; i+7<=e; i+=8){
        __m256i now = _mm256_load_si256((__m256i const *)(a+i));
        avxMax = _mm256_max_epi32(avxMax, now);
    }
    for(; i<=e; i++) ret = max(ret, a[i]);
    int tmp[8];
    _mm256_storeu_si256((__m256i*)tmp, avxMax);
    for(i=0; i<8; i++) ret = max(ret, tmp[i]);
    return ret;
}

int rangeMin(int *a, int s, int e){
    int ret = (int)(1e9+7), i;
    __m256i avxMin = _mm256_set1_epi32((int)(1e9+7));
    for(i=s; (i&7) && i<=e; i++) ret = min(ret, a[i]);
    for(; i+7<=e; i+=8){
        __m256i now = _mm256_load_si256((__m256i const *)(a+i));
        avxMin = _mm256_min_epi32(avxMin, now);
    }
    for(; i<=e; i++) ret = min(ret, a[i]);
    int tmp[8];
    _mm256_storeu_si256((__m256i*)tmp, avxMin);
    for(i=0; i<8; i++) ret = min(ret, tmp[i]);
    return ret;
}

// 검증용
ll psum[314159];
int mx = (int)(-1e9-7);
int rand(int l, int r);

int main(){
    const int n = 314159;
    const int s = 917, e = 31415;
    alignas(256) int a[n];
    for(int i=0; i<n; i++) a[i] = rand(-5000, 5000);
    psum[0] = a[0];
    for(int i=1; i<n; i++) psum[i] += psum[i-1] + a[i];
    for(int i=s; i<=e; i++) mx = max(mx, a[i]);
    cout << rangeSum(a, s, e) << " " << psum[e] - psum[s-1] << "\n";
    cout << rangeMax(a, s, e) << " " << mx << "\n";
}

int rand(int l, int r){
    mt19937 rd((unsigned)chrono::steady_clock::now().time_since_epoch().count());
    uniform_int_distribution<int> rnd(l, r);
    return rnd(rd);
}
