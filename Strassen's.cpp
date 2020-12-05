#include <iostream>
#include <iomanip>
#include <random>
#include <cmath>
#include <time.h>


using namespace std;
int index(int x, int y, int w){
    return (x * w + y);
}

void ijk_multiplication(int A[], int B[], int w, int C[]){
    int i, j, k;
    for(i=0; i<w; i++){
        for(j=0; j<w; j++){
            for(k=0; k<w; k++){
                C[index(i, j, w)]
                = (A[index(i, k, w)] % 101) * (B[index(k, j, w)] % 101);
            }
        }
    }
}

void strassen_multiplication(int A[], int B[], int w, int C[]){
    //base case
    if(w <= 8){//brute force
        ijk_multiplication(A, B, w, C);
        return;
    }
    
    int i, j, k, l;
    int n = w/2;
    int m = n * n;
    
    //divide
    int T0[m], T1[m], T2[m], T3[m], T4[m], T5[m], T6[m], T7[m], T8[m], T9[m], A11[m], A22[m], B11[m], B22[m];

    for(i=0; i<n; i++){
        for(j=0; j<n; j++){
            k = i + n;
            l = j + n;
            T0[index(i, j, n)] = B[index(i, j, w)] - B[index(i, j, w)];
            T1[index(i, j, n)] = A[index(i, j, w)] + A[index(i, l, w)];
            T2[index(i, j, n)] = A[index(k, j, w)] + A[index(k, l, w)];
            T3[index(i, j, n)] = B[index(k, j, w)] - B[index(i, j, w)];
            T4[index(i, j, n)] = A[index(i, j, w)] + A[index(k, l, w)];
            T5[index(i, j, n)] = B[index(i, j, w)] + B[index(k, l, w)];
            T6[index(i, j, n)] = A[index(i, l, w)] - A[index(k, l, w)];
            T7[index(i, j, n)] = B[index(k, j, w)] + B[index(k, l, w)];
            T8[index(i, j, n)] = A[index(i, j, w)] - A[index(k, j, w)];
            T9[index(i, j, n)] = B[index(i, j, w)] + B[index(i, l, w)];
            A11[index(i, j, n)] = A[index(i, j, w)];
            A22[index(i, j, n)] = A[index(k, l, w)];
            B11[index(i, j, n)] = B[index(i, j, w)];
            B22[index(i, j, n)] = B[index(k, l, w)];
        }
    }
    
    //conquer
    int M1[m], M2[m], M3[m], M4[m], M5[m], M6[m], M7[m];
    strassen_multiplication(A11, T0, n, M1);
    strassen_multiplication(T1, B22, n, M2);
    strassen_multiplication(T2, B11, n, M3);
    strassen_multiplication(A22, T3, n, M4);
    strassen_multiplication(T4, T5, n, M5);
    strassen_multiplication(T6, T7, n, M6);
    strassen_multiplication(T8, T9, n, M7);
    
    
    for(i=0; i<n; i++){
        for(j=0; j<n; j++){
            k = i + n;
            l = j + n;
            C[index(i, j, n)] = M5[index(i, j, n)] + M4[index(i, j, n)] - M2[index(i, j, n)] + M6[index(i, j, n)];
            C[index(i, l, n)] = M1[index(i, j, n)] + M2[index(i, j, n)];
            C[index(k, j, n)] = M3[index(i, j, n)] + M4[index(i, j, n)];
            C[index(k, l, n)] = M5[index(i, j, n)] + M1[index(i, j, n)] - M3[index(i, j, n)] - M7[index(i, j, n)];
        }
    }
}

void filling(int A[], int B[], int w){
    int i, j;
    int D[w*w];
    int E[w*w];
    for(i=0; i<w*w; i++){
        D[i] = A[i];
        E[i] = B[i];
    }
    int new_w = pow(2, ceil(log2(w)));
    A = new int[new_w * new_w];
    B = new int[new_w * new_w];
    
    for(i=0; i<w; i++){
        for(j=0; j<w; j++){
            A[index(i, j, new_w)] = D[index(i, j, w)];
            B[index(i, j, new_w)] = E[index(i, j, w)];
        }
    }
    for(i=0; i<new_w; i++){
        for(j=w; j<new_w; j++){
            A[index(i, j, new_w)] = 0;
            B[index(i, j, new_w)] = 0;
        }
    }
    for(i=w; i<new_w; i++){
        for(j=0; j<w; j++){
            A[index(i, j, new_w)] = 0;
            B[index(i, j, new_w)] = 0;
        }
    }
}


int main() {
    cout<<"Please input the width of these two matrices: \n";
    int w;
    cin>>w;
    int m = w*w;
    int i, j;
    //int D[m], E[m];
    int *A = new int[m];
    int *B = new int[m];
    
    random_device rd;
    for(i=0; i<w; i++){
        for(j=0; j<w; j++){
            A[index(i, j, w)] = rd() % 101;
            B[index(i, j, w)] = rd() % 101;
        }
    }
    //handle if sizes are not n!= 2^n
    if(log2(m)-int(log2(m)) != 0){
        filling(A, B, w);
        w = pow(2, ceil(log2(w)));
        m = w * w;
    }
    
    
    
    int C1[m], C2[m];
    
    clock_t start, finish;
    double duration_stra, duration_tra;
    
    start = clock();
    //strassen_multiplication(A, B, w, C1);
    finish = clock();
    duration_stra = (double)(finish - start) / CLOCKS_PER_SEC;

    start = clock();
    ijk_multiplication(A, B, w, C2);
    finish = clock();
    duration_tra = (double)(finish - start) / CLOCKS_PER_SEC;
    
    double ratio = duration_stra / duration_tra;
    cout<<"ratio: "<<ratio<<endl;
    return 0;
}






/*for(i=0; i<m; i++){
 for(j=0; j<m; j++){
 T[0][i][j] = B12[i][j] - B22[i][j];
 T[1][i][j] = A11[i][j] + A12[i][j];
 T[2][i][j] = A21[i][j] + A22[i][j];
 T[3][i][j] = B21[i][j] - B11[i][j];
 T[4][i][j] = A11[i][j] + A22[i][j];
 T[5][i][j] = B11[i][j] + B22[i][j];
 T[6][i][j] = A12[i][j] - A22[i][j];
 T[7][i][j] = B21[i][j] + B22[i][j];
 T[8][i][j] = A11[i][j] - A21[i][j];
 T[9][i][j] = B11[i][j] + B12[i][j];
 }
 }*/



