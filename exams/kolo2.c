#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>


enum Vehicle_type{truck,car,v_ndef};

typedef struct{
    int d;
    int m;
    int y;
}Date;

typedef struct{
    char owner[30];
    Date valid_date;
    enum Vehicle_type v_type;

    union{
        struct{
            char vehicle_ID[15];
            unsigned int places: 7;
            unsigned int driving_wheels: 4;
            unsigned int towbar: 1;
        }cs;

        struct{
            float load;
            short axes;
            int trailer_mass;
        }ts;

    }veh_union;
}Vehicle;

int cmp(void *v,int *date){
    Vehicle* V = (Vehicle*) v;
    if(V->valid_date.y<date[2]) return 1;

    else if(V->valid_date.y == date[2]){
        if(V->valid_date.m<date[1]) return 1;

        else if(V->valid_date.m == date[1]){
            if(V->valid_date.d<date[0]) return 1;
        }
    }
    return 0;
}

int delayed(const Vehicle* tv,int size,const Date *base_date,int **pdelayed_owners){
    char* delayed_owners = malloc(sizeof(char*)*size);
    int counter=0;
    for(int i=0;i<size;i++){
        if(cmp(&tv[i],base_date)){
            delayed_owners[counter] = i;
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

void print_owners(const Vehicle* tv,int* t_ind,size_t size){
    for(int i=0;i<size;i){
        printf("%s",tv[t_ind[i]].owner);
    }
}

Vehicle* create_vehicle(void){
    Vehicle* new = (Vehicle*)malloc(sizeof(Vehicle));
    strcpy(new->owner,"");
    new->valid_date.d = 1;
    new->valid_date.m = 1;
    new->valid_date.y = 1970;
    new->v_type = v_ndef;

    return new;
}


void move_date(Vehicle* v, int months) {
    int y = months / 12;
    int months_left = months % 12;

    v->valid_date.y += y;
    v->valid_date.m += months_left;

    if(v->valid_date.m>12){
        v->valid_date.y++;
        v->valid_date.m-=12;
    }
}


void print_v_data(const Vehicle* v){
    switch(v->v_type){
        case car:
            printf("Car: ");
            printf("ID: %s ",v->veh_union.cs.vehicle_ID);
            printf("miejsc: %u ",v->veh_union.cs.places);
            printf("naped na: %u ",v->veh_union.cs.driving_wheels);
            if(v->veh_union.cs.towbar == 1){
                printf("towbar: TAK");
            }

            else{
                printf("towbar: NIE");
            }
            break;

        case truck:
            printf("Truck: ");
            printf("Load: %f ",v->veh_union.ts.load);
            printf("Axes: %f ",v->veh_union.ts.axes);
            printf("Mass: %f ",v->veh_union.ts.trailer_mass);
            break;

        case v_ndef:
            break;

    }
}


