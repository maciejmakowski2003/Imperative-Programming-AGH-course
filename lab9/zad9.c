#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <limits.h>

#define TAB_SIZE  1000
#define BUF_SIZE  1000

int get(int cols, int row, int col, const int *A) {
    return A[(row)*cols + col];
}

void set(int cols, int row, int col, int *A, int value) {
    A[(row)*cols + col] = value;
}

void prod_mat(int rowsA, int colsA, int colsB, int *A, int *B, int *AB) {
    int tmp;
    for(int i=0;i<rowsA;i++){
        for(int j=0;j<colsB;j++){
            tmp=0;
            for(int k=0;k<colsA;k++){
                tmp+=get(colsA,i,k,A)*get(colsB,k,j,B);
            }
            set(colsB,i,j,AB,tmp);
        }
    }
}

void read_mat(int rows, int cols, int *t) {
    for(int i=0;i<rows*cols;i++){
        scanf("%d",&t[i]);
    }
}

void print_mat(int rows, int cols, int *t) {
    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            printf("%d ",t[rows*i + j]);
        }
        printf("\n");
    }
}

int read_char_lines(char *array[]) {
    char buff[BUF_SIZE];
    int lines=0;
    int letters;
    fgets(buff, BUF_SIZE, stdin);


    do{
        letters=0;

        for(int i=0;i<BUF_SIZE;i++){
            if(buff[i] != '\n'){
                letters++;
            }
            else{
                array[lines]=malloc(letters * sizeof(char));
                strcpy(array[lines++],buff);
                break;
            }
        }

    }
    while(fgets(buff, BUF_SIZE, stdin)!=NULL);
    return lines;
}

void write_char_line(char *array[], int n) {
    printf("%s",array[n]);
}

void delete_lines(char *array[]) {
    int idx=0;
    while(array[idx] != NULL){
        free(array[idx++]);
    }
}

int read_int_lines_cont(int *ptr_array[]) {
    char buff[BUF_SIZE];
    int lines=0;

    while(lines<TAB_SIZE && fgets(buff, sizeof(buff), stdin) != NULL) {
        if (buff[0] == '\n') {
            break;
        }

        char *token = strtok(buff, " \n");
        int nums_in_line = 0;

        ptr_array[lines] = malloc(sizeof(int) * TAB_SIZE);

        while (token != NULL ) {
            ptr_array[lines][nums_in_line] = atoi(token);
            nums_in_line++;
            token = strtok(NULL, " \n");
        }
        ptr_array[lines][nums_in_line] = INT_MAX;
        lines++;
    }

    return lines;
}

void write_int_line_cont(int *ptr_array[], int n) {
    int *ptr = ptr_array[n];
    while (ptr != NULL && *ptr<INT_MAX) {
        printf("%d ", *ptr);
        ptr++;
    }
}

typedef struct {
    int *values;
    int len;
    double average;
} line_type;

int read_int_lines(line_type lines_array[]) {
    int *ptr_array;
    char buff[BUF_SIZE];
    int lines=0;

    while(lines<TAB_SIZE && fgets(buff, sizeof(buff), stdin) != NULL) {
        if (buff[0] == '\n') {
            break;
        }

        char *token = strtok(buff, " \n");
        int nums_in_line = 0;
        double sum=0;

        ptr_array = malloc(sizeof(int) * TAB_SIZE);

        while (token != NULL ) {
            ptr_array[nums_in_line] = atoi(token);
            sum+=atoi(token);
            nums_in_line++;
            token = strtok(NULL, " \n");
        }
        lines_array[lines].values=ptr_array;
        lines_array[lines].len=nums_in_line;
        lines_array[lines].average=sum/nums_in_line;
        lines++;
    }

    return lines;
}

void write_int_line(line_type lines_array[], int n){
    for(int i=0;i<lines_array[n].len;i++){
        printf("%d ",*lines_array[n].values);
        *lines_array[n].values++;
    }
    printf("\n%.2f",lines_array[n].average);
}

void delete_int_lines(line_type array[], int line_count) {
    for(int i=0;i<line_count;i++){
        free(array[i].values);
    }
}

int cmp (const void *a, const void *b) {
    line_type A = *((line_type*) a );
    line_type B = *((line_type*) b );

    if(A.average<B.average){
        return -1;
    }

    return 1;
}

void sort_by_average(line_type lines_array[], int line_count) {
    qsort(lines_array,line_count,sizeof(line_type),cmp);
}

typedef struct {
    int r, c, v;
} triplet;

int read_sparse(triplet *triplet_array, int n_triplets) {
    for(int i=0;i<n_triplets;i++){
        triplet t;
        scanf("%d",&t.r);
        scanf("%d",&t.c);
        scanf("%d",&t.v);
        triplet_array[i] = t;
    }

}

int cmp_triplets(const void *t1, const void *t2) {
    triplet T1=*((triplet*) t1);
    triplet T2=*((triplet*) t2);

    if(T1.r<T2.r){
        return -1;
    }

    else if(T1.r>T2.r){
        return 1;
    }

    else{
        if(T1.c<T2.c){
            return -1;
        }

        else if(T1.c>T2.c){
            return 1;
        }

        return 1;
    }
}

void make_CSR(triplet *triplet_array, int n_triplets, int rows, int *V, int *C, int *R) {
    qsort(triplet_array,n_triplets,sizeof(triplet),cmp_triplets);
    int row=0;
    R[0]=0;

    for(int i=0;i<n_triplets;i++){
        V[i]=triplet_array[i].v;
        C[i]=triplet_array[i].c;
        if(triplet_array[i].r>row){
            row++;
            R[row]=i;
        }
    }

    for(int i=row+1;i<rows+1;i++){
        R[i]=n_triplets;
    }
}

void multiply_by_vector(int rows, const int *V, const int *C, const int *R, const int *x, int *y) {
    int idx=0;
    for(int i=0;i<rows;i++){
        int yi=0;
        for(int j=0;j<R[i+1]-R[i];j++){
            yi+=V[idx]*x[C[idx]];
            idx++;
        }
        y[i]=yi;
    }
}

void read_vector(int *v, int n) {
    for(int i=0;i<n;i++){
        scanf("%d",&v[i]);
    }
}

void write_vector(int *v, int n) {
    for(int i=0;i<n;i++){
        printf("%d ",v[i]);
    }
    printf("\n");
}

int read_int() {
    char c_buf[BUF_SIZE];
    fgets(c_buf, BUF_SIZE, stdin);
    return (int)strtol(c_buf, NULL, 10);
}

int main9(void) {
    int to_do = read_int();

    int A[TAB_SIZE], B[TAB_SIZE], AB[TAB_SIZE];
    int n, lines_counter, rowsA, colsA, rowsB, colsB;
    int rows, cols, n_triplets;
    char *char_lines_array[TAB_SIZE] = { NULL };
    int continuous_array[TAB_SIZE];
    int *ptr_array[TAB_SIZE];
    triplet triplet_array[TAB_SIZE];
    int V[TAB_SIZE], C[TAB_SIZE], R[TAB_SIZE];
    int x[TAB_SIZE], y[TAB_SIZE];
    line_type int_lines_array[TAB_SIZE];

    switch (to_do) {
        case 1:
            scanf("%d %d", &rowsA, &colsA);
            read_mat(rowsA, colsA, A);
            scanf("%d %d", &rowsB, &colsB);
            read_mat(rowsB, colsB, B);
            prod_mat(rowsA, colsA, colsB, A, B, AB);
            print_mat(rowsA, colsB, AB);
            break;
        case 2:
            n = read_int() - 1; // we count from 1 :)
            ptr_array[0] = continuous_array;
            read_int_lines_cont(ptr_array);
            write_int_line_cont(ptr_array, n);
            break;
        case 3:
            n = read_int() - 1;
            read_char_lines(char_lines_array);
            write_char_line(char_lines_array, n);
            delete_lines(char_lines_array);
            break;
        case 4:
            n = read_int() - 1;
            lines_counter = read_int_lines(int_lines_array);
            sort_by_average(int_lines_array, lines_counter);
            write_int_line(int_lines_array, n);
            delete_int_lines(int_lines_array, lines_counter);
            break;
        case 5:
            scanf("%d %d %d", &rows, &cols, &n_triplets);
            n_triplets = read_sparse(triplet_array, n_triplets);
            read_vector(x, cols);
            make_CSR(triplet_array, n_triplets, rows, V, C, R);
            multiply_by_vector(rows, V, C, R, x, y);
            write_vector(V, n_triplets);
            write_vector(C, n_triplets);
            write_vector(R, rows + 1);
            write_vector(y, rows);
            break;
        default:
            printf("NOTHING TO DO FOR %d\n", to_do);
            break;
    }
    return 0;
}



