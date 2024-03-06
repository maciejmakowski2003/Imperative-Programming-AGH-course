#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define SIZE 40

#define TEST 1    // 1 - dla testow,  0 - dla oceny automatycznej

void read_vec(double x[], size_t n) {
    for(size_t i=0; i<n; ++i)  scanf("%lf",x++);
}

void print_vec(const double x[], size_t n) {
    for(size_t i=0; i<n; ++i) printf("%.4f ",x[i]);
    printf("\n");
}

void read_mat52(double A[][SIZE], size_t m, size_t n) {
    for(size_t i=0; i<m; ++i) {
        for(size_t j=0; j<n; ++j)  scanf("%lf",&A[i][j]);
    }
}

void print_mat52(const double A[][SIZE], size_t m, size_t n) {
    for(size_t i=0; i<m; ++i) {
        for(size_t j=0; j<n; ++j)  printf("%.4f ",A[i][j]);
        printf("\n");
    }
}

// 5.2.1 Triangularyzacja, wyznacznik i rozwiazanie Ax=b dla  macierzy kwadratowej.
// Wersja rozszerzona: Wybor wiersza z maksymalna waroscia |elementu glownego|
// Przy wymianie wierszy nalezy zastosowac wetor permutacji indeksow wierszy.
// Jezeli maksymalna wartosc |elementu glownego| < eps, to wyznacznik = 0.
// Zwraca wyznacznik det. Jezeli =0,  to triangularyzacja moze byc niedokonczona.
// Jezeli wyznacznik != 0, i b,x != NULL,
// to w wektorze x umieszcza rozwiazanie ukladu rownan Ax=b.

int swap(double A[][SIZE],size_t n, int idx[], int pos){
    int maxi=0;
    int index;

    for(int i=pos;i<n;i++){
        if(fabs(A[idx[i]][pos])>maxi){
            maxi=fabs(A[idx[i]][pos]);
            index=i;
        }
    }

    return index;
}


double gauss(double A[][SIZE], double b[], double x[], size_t n, double eps) {
    int idx[n];
    int index;
    int tmp;
    double tmp2;
    double det=1;

    for(int i=0;i<n;i++){
        idx[i]=i;
    }

    for(int i=0;i<n;i++){
        index=swap(A,n,idx,i);//rows swap
        if(i!=index){
            det*=-1;
        }
        tmp=idx[i];
        idx[i]=idx[index];
        idx[index]=tmp;

        if(fabs(A[idx[i]][i])<eps){
            return NAN;
        }

        else{
            for(int j=i+1;j<n;j++){//rows subtraction
                tmp2=A[idx[j]][i]/A[idx[i]][i];

                for(int k=0;k<n;k++){
                    A[idx[j]][k]-=A[idx[i]][k]*tmp2;
                }
                b[idx[j]]-=b[idx[i]]*tmp2;
            }
        }
    }

    for(int i=0;i<n;i++){
        det*=A[idx[i]][i];
    }


    for(int i=n-1;i>=0;i--){
        for(int j=i+1;j<n;j++){
            b[idx[i]]-=A[idx[i]][j]*x[j];
        }
        x[i]=b[idx[i]]/A[idx[i]][i];
    }

    return det;
}

// 5.2.2
// Zwraca wyznacznik i w tablicy B macierz odwrotna (jezlei wyznacznik != 0)
// Jezeli maksymalna bezwzgledna wartosc elementu glownego < eps,
// to funkcja zwraca wartosc wyznacznika det = 0.
// Funkcja zmienia wartosci takze w tablicy A.

double matrix_inv(double A[][SIZE], double B[][SIZE], size_t n, double eps, double C[][SIZE]) {
    int idx[n];
    int index;
    int tmp;
    double tmp2;
    double det=1;

    for(int i=0;i<n;i++){
        idx[i]=i;
    }

    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){

            if(i==j){
                B[i][j]=1;
            }

            else{
                B[i][j]=0;
            }
        }
    }


    for(int i=0;i<n;i++){//lower
        index=swap(A,n,idx,i);//rows swap
        if(i!=index){
            det*=-1;
        }
        tmp=idx[i];
        idx[i]=idx[index];
        idx[index]=tmp;

        if(fabs(A[idx[i]][i])<eps){
            return NAN;
        }

        else{
            det*=A[idx[i]][i];
            tmp2=1/A[idx[i]][i];//-> A[idx[i]][i]=1
            for(int j=0;j<n;j++){
                A[idx[i]][j]*=tmp2;
                B[idx[i]][j]*=tmp2;
            }
            for(int j=i+1;j<n;j++){
                tmp2=A[idx[j]][i]/A[idx[i]][i];

                for(int k=0;k<n;k++){
                    A[idx[j]][k]-=A[idx[i]][k]*tmp2;
                    B[idx[j]][k]-=B[idx[i]][k]*tmp2;
                }
            }
        }
    }


    for(int i=n-1;i>0;i--){//upper
        for(int j=i-1;j>=0;j--){
            tmp2=A[idx[j]][i];
            for(int k=0;k<n;k++){
                B[idx[j]][k]-=B[idx[i]][k]*tmp2;
            }
        }
    }

    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            C[i][j]=B[idx[i]][j];
        }
    }

    return det;
}

int main52(void) {

    double A[SIZE][SIZE], B[SIZE][SIZE], C[SIZE][SIZE];
    double b[SIZE], x[SIZE], det, eps = 1.e-13;

    int to_do;
    size_t m,n,p,q;
    if(TEST) printf("Wpisz nr zadania ");
    scanf ("%d", &to_do);
    switch (to_do) {
        case 1:
            if(TEST) printf("Wpisz liczbe wierszy i kolumn mac. kwadratowej: ");
            scanf("%d", &n);
            if(TEST) printf("Wpisz macierz A (wierszami): ");
            read_mat52(A,n,n);
            if(TEST) printf("Wpisz wektor b: ");
            read_vec(b,n);
            det = gauss(A, b, x, n, eps);
            printf("%.4f\n",det);
            //print_mat(A,n,n);
            if(det) print_vec(x,n);
            break;
        case 2:
            if(TEST) printf("Wpisz rozmiar macierzy n = ");
            scanf("%d",&n);
            if(TEST) printf("Wpisz elementy macierzy (wierszami): ");
            read_mat52(A,n,n);
            det = matrix_inv(A,B,n,eps,C);
            printf("%.4f\n",det);
            if(det) print_mat52(C,n,n);
            break;
        default:
            printf("NOTHING TO DO FOR %d\n", to_do);
    }
    return 0;
}

