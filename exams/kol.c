#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

//1 zad
typedef struct _matrix{
    int rows;
    int cols;
    double** data;
}Matrix;

int create_matrix(Matrix* pmatrix, int rows, int cols){
    pmatrix->rows = rows;
    pmatrix->cols = cols;

    pmatrix->data = malloc(rows*sizeof(double*));

    if(pmatrix->data == NULL) return 0;

    for(int i=0;i<rows;i++){
        pmatrix->data[i] = calloc(cols,sizeof(double));
        if(pmatrix->data[i] == NULL) return 0;
    }
    return 1;
}

int set(Matrix* pmatrix,int row,int col, double value){
    if(pmatrix->rows<row || row<=0 || pmatrix->cols<col || col<=0) return 0;
    pmatrix->data[row][col] = value;
    return 1;
}

int get(Matrix* pmatrix,int row,int col, double* value){
    if(pmatrix->rows<row || row<=0 || pmatrix->cols<col || col<=0) return 0;
    *value = pmatrix->data[row][col];
    return 1;
}

void create_id_matrix(Matrix* pmatrix, int size){
    create_matrix(pmatrix,size,size);
    for(int i=0;i<size;i++){
        pmatrix->data[i][i] = 1;
    }
}

double dot_prod_vec(const double* vec1, const double* vec2,int size){
    double tmp = 0;

    for(int i=0;i<size;i++){
        tmp+=vec1[i]*vec2[i];
    }
    return tmp;
}

void mul_vec(double *result, const Matrix* pm, const double* vec){
    for(int i=0;i<pm->rows;i++){
        double tmp = 0;
        tmp = dot_prod_vec(pm->data[i],vec,pm->cols);
        result[i] = tmp;
    }
}

void max_element_in_rows(double* maxdata,Matrix* pm){
    for(int i=0;i<pm->rows;i++){
        double tmp = pm->data[i][0];
        for(int j=1;j<pm->cols;j++){
            if(pm->data[i][j]>tmp){
                tmp = pm->data[i][j];
            }
        }
        maxdata[i] = tmp;
    }
}

//2 zad

typedef struct{
    double *results;
    int len;
}Data;

typedef struct tagQUEUEelement{
    Data data;
    struct tagQUEUEelement* next;
}QUEUEelement;

typedef struct Queue{
    QUEUEelement *head;
    QUEUEelement *tail;
}Queue;

void free_queue(Queue* pq){
    QUEUEelement* tmp = pq->head;
    while(tmp){
        QUEUEelement* to_delete = tmp;
        tmp = tmp->next;

        free(to_delete);
    }
}

void push(Queue* pq, const Data* data){
    QUEUEelement* new;
    new->data = *data;
    new->next = 0;
    pq->tail->next = new;
    pq->tail = new;
}

int peek(const Queue* pq, Data* data){
    if(!pq->head) return 0;

    *data = pq->head->data;
    return 1;
}

int pop(Queue* pq, Data* data){
    if(!pq->head) return -1;
    *data = pq->head->data;
    QUEUEelement* to_delete = pq->head;
    pq->head = pq->head->next;
    free(to_delete);

    if(!pq->head) return 0;
    return 1;
}

int get_total_count(const Queue* pq){
    int tmp = 0;
    QUEUEelement* curr = pq->head;
    while(curr){
        tmp+=sizeof(curr->data.results)/sizeof(double);
    }
    return tmp;
}

//3 zad

enum VehicleType{
    car,
    bus,
    truck
};

enum EngineType{
    electric,
    hybrid,
    combustion
};

struct Vehicle{
    char* owner;
    int* inspectionDate;
    enum VehicleType type;
    enum EngineType engine;

    union{
        struct{
            int capacity;
            float power;
        }carData;

        struct{
            int seatingCapacity;
            int standingCapacity;
        }busData;

        struct{
            float engineCapacity;
            float maxAxleLoad;
        }truckData;
    };
};

void new_owner(struct Vehicle* pv){
    char new[50];
    fgets(new,sizeof(new),stdin);
    new[strcspn(new, "\n")] = '\0';//ususniecie znaku nowej lini z konca

    free(pv->owner);
    pv->owner = malloc((strlen(new)+1)*sizeof(char));
    strcpy(pv->owner,new);
}

int cmp(int *v,int *date){
    if(v[2]<date[2]) return 1;

    else if(v[2] == date[2]){
        if(v[1]<date[1]) return 1;

        else if(v[1] == date[1]){
            if(v[0]<date[0]) return 1;
        }
    }
    return 0;
}

int delayed(struct Vehicle* pv, int size, int* base_date, char ***pdelayed_owners){
    char** delayed_owners = malloc(sizeof(char*)*size);
    int counter=0;
    for(int i=0;i<size;i++){
        if(cmp(pv[i].inspectionDate,base_date)){
            delayed_owners[counter] = pv[i].owner;
            counter++;
        }
    }

    if(counter>0){
        delayed_owners = realloc(delayed_owners,counter*sizeof(char*));
        pdelayed_owners = &delayed_owners;
        return counter;
    }
    else{
        pdelayed_owners = 0;
        return 0;
    }
}

void print_pojazd(struct Vehicle* pv){
    printf("%s\n",pv->owner);
    printf("%d %d %d\n",pv->inspectionDate[0],pv->inspectionDate[1],pv->inspectionDate[2]);
    printf("%s\n", pv->type);
    printf("%s",pv->engine);


}

int main(){
    struct Vehicle pv;
    pv.owner = "ANNA";
    int date[3] = {10,12,2023};
    pv.inspectionDate = date;
    pv.type = bus;
    pv.engine = electric;

    print_pojazd(&pv);
    return 0;
}
