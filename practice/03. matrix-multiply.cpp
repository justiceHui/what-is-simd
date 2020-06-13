/*
avx를 이용한 행렬 곱셈
Matrix Multiply : (2000*2000) * (2000*2000)
matrix multiply(naive) : 39586ms
matrix multiply(avx) : 2546ms
Correct!
*/

#include <bits/stdc++.h>
#include <immintrin.h>
#pragma GCC target("avx,avx2,fma")
using namespace std;

struct Matrix{
    int n, m;
    float **v;

    void init(int N, int M){
        n = N; m = M;
        v = (float**)malloc(sizeof(float*) * n);
        for(int i=0; i<n; i++){
            v[i] = (float*)_aligned_malloc((sizeof(float)*m+15)>>4<<4, 256/8);
        }
        for(int i=0; i<n; i++) for(int j=0; j<m; j++) v[i][j] = (rand() & 3);
    }
    ~Matrix(){
        if(v){
            for(int i=0; i<n; i++) _aligned_free(v[i]);
            free(v);
        }
    }
};

int multiply(Matrix &a, Matrix &b, Matrix &c){
    // size check
    assert(a.m == b.n);
    assert(a.n == c.n);
    assert(c.m == b.m);
    // get start time
    chrono::system_clock::time_point st = chrono::system_clock::now();
    // multiply
    for(int i=0; i<a.n; i++) for(int j=0; j<b.m; j++){
            c.v[i][j] = 0;
            for(int k=0; k<a.m; k++) c.v[i][j] += a.v[i][k] * b.v[k][j];
        }
    // get end time
    chrono::system_clock::time_point ed = chrono::system_clock::now();
    chrono::milliseconds times = chrono::duration_cast<chrono::milliseconds>(ed - st);
    return times.count();
}

int multiply_avx(Matrix &a, Matrix &b, Matrix &c){ // c = a * b
    // size check
    assert(a.m == b.n);
    assert(a.n == c.n);
    assert(c.m == b.m);
    // get start time
    chrono::system_clock::time_point st = chrono::system_clock::now();
    // init
    for(int i=0; i<c.n; i++) memset(c.v[i], 0, sizeof(float)*c.m);
    // multiply
    for(int i=0; i<a.n; i++) for(int j=0; j<a.m; j++){
            __m256 now = _mm256_set1_ps(a.v[i][j]);
            int k = 0;
            for(; k+7<b.m; k+=8){
                __m256 *t1 = (__m256*)(b.v[j]+k);
                __m256 *t2 = (__m256*)(c.v[i]+k);
                *t2 = _mm256_add_ps(*t2, _mm256_mul_ps(now, *t1));
            }
            for(; k<b.m; k++) c.v[i][k] += a.v[i][j] * b.v[j][k];
        }
    // get end time
    chrono::system_clock::time_point ed = chrono::system_clock::now();
    chrono::milliseconds times = chrono::duration_cast<chrono::milliseconds>(ed - st);
    return times.count();
}

int main(){
    int N = 2000, M = 2000, K = 2000;
    Matrix a, b, c, d;
    a.init(N, M);
    b.init(M, K);
    c.init(N, K);
    d.init(N, K);
    cout << "Matrix Multiply : (" << N << "*" << M << ") * (" << M << "*" << K << ")\n";
    cout << "matrix multiply(naive) : " << multiply(a, b, c) << "ms\n";
    cout << "matrix multiply(avx) : " << multiply_avx(a, b, d) << "ms\n";
    for(int i=0; i<N; i++) for(int j=0; j<K; j++){
            if(c.v[i][j] != d.v[i][j]){
                cout << "Wrong!\n"; return 0;
            }
        }
    cout << "Correct!\n";
}
