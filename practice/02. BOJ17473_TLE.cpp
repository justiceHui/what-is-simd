/*
** 백준 17473 수열과 쿼리 25
** 로컬에서 돌려보니 5200~5400ms 정도 나오던데 어떻게 줄이지
** 정답 코드의 출력 결과와 빠른 비교를 위해 모든 출력값을 xor한 결과만 출력합니다.
*/

#include <bits/stdc++.h>
#include <immintrin.h>
#pragma GCC target("avx,avx2,fma")
using namespace std;

const int N = 202020;
const int wordSize = 256 / 8 / sizeof(int);

int n, q;
alignas(256) int a[N], tmp[wordSize];
__m256i vec[N/wordSize];

void rangeAnd(int s, int e, int x){
    int i = s;
    if(s & 7){
        int now = s >> 3;
        _mm256_storeu_si256((__m256i *)(a+(now<<3)), vec[now]);
        for(; (i&7) && i<=e; i++) a[i] &= x;
        vec[now] = _mm256_load_si256((__m256i *)(a+(now<<3)));
    }
    if(i > e) return;
    __m256i now = _mm256_set1_epi32(x);
    for(; i+7<=e; i+=8){
        vec[i >> 3] = _mm256_and_si256(vec[i >> 3], now);
    }
    if((e&7) != 7){
        int now = i >> 3;
        _mm256_storeu_si256((__m256i *)(a+(now<<3)), vec[now]);
        for(; i<=e; i++) a[i] &= x;
        vec[now] = _mm256_load_si256((__m256i *)(a+(now<<3)));
    }
}

void rangeOr(int s, int e, int x){
    int i = s;
    if(s & 7){
        int now = s >> 3;
        _mm256_storeu_si256((__m256i *)(a+(now<<3)), vec[now]);
        for(; (i&7) && i<=e; i++) a[i] |= x;
        vec[now] = _mm256_load_si256((__m256i *)(a+(now<<3)));
    }
    if(i > e) return;
    __m256i now = _mm256_set1_epi32(x);
    for(; i+7<=e; i+=8){
        vec[i >> 3] = _mm256_or_si256(vec[i >> 3], now);
    }
    if((e&7) != 7){
        int now = i >> 3;
        _mm256_storeu_si256((__m256i *)(a+(now<<3)), vec[now]);
        for(; i<=e; i++) a[i] |= x;
        vec[now] = _mm256_load_si256((__m256i *)(a+(now<<3)));
    }
}

int rangeMax(int s, int e){
    int mx = (int)(-1e9), i = s;
    __m256i vec_max = _mm256_set1_epi32(mx);
    if(s & 7){
        int now = s >> 3;
        _mm256_storeu_si256((__m256i *)(a+(now<<3)), vec[now]);
        for(; (i&7) && i<=e; i++) mx = max(mx, a[i]);
    }
    if(i > e) return mx;
    for(; i+7<=e; i+=8){
        vec_max = _mm256_max_epi32(vec_max, vec[i >> 3]);
    }
    if((e&7) != 7){
        int now = i >> 3;
        _mm256_storeu_si256((__m256i *)(a+(now<<3)), vec[now]);
        for(; i<=e; i++) mx = max(mx, a[i]);
    }
    _mm256_storeu_si256((__m256i *)tmp, vec_max);
    for(i=0; i<8; i++) mx = max(mx, tmp[i]);
    return mx;
}

int main(){
    freopen("../in", "r", stdin);
    ios_base::sync_with_stdio(0); cin.tie(0);
    chrono::system_clock::time_point st = chrono::system_clock::now();
    int ans = 0;
    cin >> n;
    for(int i=0; i<n; i++) cin >> a[i];
    for(int i=0; i<n; i+=8){
        vec[i >> 3] = _mm256_load_si256((__m256i *)(a+i));
    }
    cin >> q;
    while(q--){
        int op, a, b, x; cin >> op >> a >> b; a--; b--;
        if(op == 3) ans ^= rangeMax(a, b);
        else{
            cin >> x;
            if(op == 1) rangeAnd(a, b, x);
            else rangeOr(a, b, x);
        }
    }
    cout << ans;
    chrono::system_clock::time_point ed = chrono::system_clock::now();
    chrono::milliseconds times = chrono::duration_cast<chrono::milliseconds>(ed - st);
    cout << "\n" << times.count();
}
