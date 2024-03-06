#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

typedef struct Matrix{
    int rows;
    int cols;
    double* data;
}Matrix;

Matrix* add_scalar(const Matrix* pm, double v){
    Matrix* new = create_matrix(pm->rows,pm->cols);
    int size = pm->rows * pm->cols;

    for(int i = 0; i<size;i++){
        new->data[i] = v + pm->data[i];
    }

    return new;
}

void print_vec(const double* vec, int size){
    printf("[");
    for(int i=0;i<size;i++){
        if(i<size-1){
            printf("%f, ",vec[i]);
        }
        else{
            printf("%f",vec[i]);
        }
    }
    printf("]");
}

void print_matrix(const Matrix* pm) {
    for (int i = 0; i < pm->rows; i++) {
        int start_index = i * pm->cols;
        print_vec(&pm[start_index], pm->cols);
    }
}

void max_element_in_cols(double* maxdata,const Matrix *pm){
    for(int i=0;i<pm->cols;i++){
        maxdata[i] = pm->data[i];
        for(int j=1;j<pm->rows;j++){
            if(pm->data[i + j*pm->rows]>maxdata[i]){
                maxdata[i] = pm->data[i + j*pm->rows];
            }
        }
    }
}

void fun(double* data){
    double elem = *data;
    *data = elem * elem;
}

void apply_to_elements(const Matrix *pm, void (*elem_fun)(double*)){
    int size = pm->rows * pm->cols;

    for(int i=0;i<size;i++){
        fun(&pm->data[i]);
    }
}

int glob_columns;

int compar(const void *a, const void *b){
    double* A = (double*) a;
    double* B = (double*) b;

    double sumA = 0;
    double sumB = 0;

    for(int i=0;i<glob_columns;i++){
        sumA+= A[i];
        sumB+= B[i];
    }

    return sumA - sumB;
}

qsort(M,M->rows,sizeof(double)*glob_columns,compar);




int main(){
    return 0;
}


