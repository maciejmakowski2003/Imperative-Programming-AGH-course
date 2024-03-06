#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_REL_SIZE 100
#define MAX_RANGE 100

typedef struct {
    int first;
    int second;
} pair;

// Case 1:

// The relation R is reflexive if xRx for every x in X
int is_reflexive(pair *tab, int n){
    int flag=0;
    int elm;

    for(int i=0;i<n;i++){
        flag=0;
        elm=tab[i].first;
        for(int j=0;j<n;j++){
            if(tab[j].first == elm && tab[j].second == elm){
                flag=1;
                break;
            }
        }

        if(flag == 0){
            return 0;
        }
    }
    return 1;
}

// The relation R on the set X is called irreflexive
// if xRx is false for every x in X
int is_irreflexive(pair *tab, int n){
    for(int i=0;i<n;i++){
        if(tab[i].first == tab[i].second){
            return 0;
        }
    }

    return 1;
}

// A binary relation R over a set X is symmetric if:
// for all x, y in X xRy <=> yRx
int is_symmetric(pair *tab, int n){
    int flag=0;

    for(int i=0;i<n;i++){
        flag=0;

        for(int j=0;j<n;j++){
            if(tab[i].first == tab[j].second && tab[i].second == tab[j].first){
                flag=1;
                break;
            }
        }

        if(flag == 0){
            return 0;
        }
    }

    return 1;
}

// A binary relation R over a set X is antisymmetric if:
// for all x,y in X if xRy and yRx then x=y
int is_antisymmetric(pair *tab, int n){
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++) {
            if (tab[i].first == tab[j].second && tab[i].second == tab[j].first && tab[i].first != tab[i].second) {
                return 0;
            }
        }
    }

    return 1;
}

// A binary relation R over a set X is asymmetric if:
// for all x,y in X if at least one of xRy and yRx is false
int is_asymmetric(pair *tab, int n){
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++) {
            if (tab[i].first == tab[j].second && tab[i].second == tab[j].first) {
                return 0;
            }
        }
    }

    return 1;
}

// A homogeneous relation R on the set X is a transitive relation if:
// for all x, y, z in X, if xRy and yRz, then xRz
int is_transitive(pair *tab, int n){
    int flag;

    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            if(tab[i].second == tab[j].first){
                for(int k=0;k<n;k++){
                    if(tab[k].first == tab[i].first && tab[k].second == tab[j].second){
                        flag=1;
                        break;
                    }
                }

                if(flag == 0){
                    return 0;
                }
            }
        }
    }

    return 1;
}

int is_connected(pair *tab, int n, int *domain, int m);
int get_domain(pair *tab, int n, int *domain);
int cmp_pair (const void *a, const void *b);

// Case 2:

// A partial order relation is a homogeneous relation that is
// reflexive, transitive, and antisymmetric
int is_partial_order(pair *tab, int n){
    return is_reflexive(tab,n) * is_transitive(tab,n) * is_antisymmetric(tab,n);
}

// A total order relation is a partial order relation that is connected
int is_total_order(pair *tab, int n, int *domain, int m){
    return is_partial_order(tab,n) * is_connected(tab,n,domain,m);
}

// Relation R is connected if for each x, y in X:
// xRy or yRx (or both)
int is_connected(pair *tab, int n, int *domain, int m){
    int flag;
    int a,b;

    for(int i=0;i<m;i++){
        a=domain[i];
        for(int j=i;j<m;j++){
            flag=0;
            b=domain[j];
            for(int k=0;k<n;k++){
                if((tab[k].first == a && tab[k].second == b) || (tab[k].first == b && tab[k].second == a)){
                    flag=1;
                }
            }

            if(flag == 0){
                return 0;
            }
        }
        return 1;
    }
}

int get_domain(pair *tab, int n, int *domain){
    int index=0;
    int flag1;
    int flag2;
    int a,b;

    for(int i=0;i<n;i++){
        a=tab[i].first;
        b=tab[i].second;
        flag1=1;
        flag2=1;
        for(int j=0;j<index;j++){
            if(a == domain[j]){
                flag1=0;
            }

            if(b == domain[j]){
                flag2=0;
            }
        }

        if(flag1==1){
            domain[index]=a;
            index++;
        }

        if(flag2==1 && a!=b){
            domain[index]=b;
            index++;
        }
    }

    qsort((int*)domain,index,sizeof(domain[0]),cmp_pair);

    return index;
}

int find_max_elements(pair *tab, int n, int *max, int *domain, int m){
    int index=0;
    int flag;

    for(int i=0;i<m;i++){
        flag=1;
        for(int j=0;j<n;j++){
            if(tab[j].first == domain[i] && tab[j].first<tab[j].second){
                flag=0;
                break;
            }
        }
        if(flag==1){
            max[index]=domain[i];
            index++;
        }
    }
    return index;
}
int find_min_elements(pair *tab, int n, int *min, int *domain, int m){
    int index=0;
    int flag;

    for(int i=0;i<m;i++){
        flag=1;
        for(int j=0;j<n;j++){
            if(tab[j].second == domain[i] && tab[j].first<tab[j].second){
                flag=0;
                break;
            }
        }
        if(flag==1){
            min[index]=domain[i];
            index++;
            continue;
        }
    }
    return index;
}

// Case 3:

int composition (pair *tab1, int n, pair *tab2, int m, pair *tab3){
    int index=0;
    int x;
    int y;
    int z;
    int flag;

    for(int i=0;i<m;i++){
        x=tab2[i].first;
        y=tab2[i].second;

        for(int j=0;j<n;j++){
            if(tab1[j].first == y){
                z=tab1[j].second;
                flag=1;

                for(int k=0;k<index;k++){
                    if(tab3[k].first==x && tab3[k].second==z){
                        flag=0;
                        break;
                    }
                }

                if(flag==1){
                    pair new_pair;
                    new_pair.first=x;
                    new_pair.second=z;
                    tab3[index]=new_pair;
                    index++;
                }
            }
        }
    }
    return index;
}

// Comparator for pair
int cmp_pair (const void *a, const void *b) {
    int l = *(const int *)a;
    int r = *(const int *)b;

    if(l>r){
        return 1;
    }

    return 0;
}

int insert_int (int *tab, int n, int new_element) {
    return 0;
}

// Add pair to existing relation if not already there
int add_relation (pair *tab, int n, pair new_pair) {
    for(int i=0;i<n;i++){
        if(tab[i].first == new_pair.first && tab[i].second == new_pair.second){
            return 0;
        }
    }

    return 1;
}

// Read number of pairs, n, and then n pairs of ints
int read_relation(pair *relation) {
    int n;
    scanf("%d",&n);

    int i=0;
    int index=0;
    int a,b;

    while(i<n){
        scanf("%d %d",&a,&b);
        pair new_pair;
        new_pair.first = a;
        new_pair.second = b;

        if(add_relation(relation,index,new_pair)==1){
            relation[index] = new_pair;
            index++;
        }

        i++;
    }

    return index;
}

void print_int_array(int *array, int n) {
    printf("%d\n",n);
    for(int i=0;i<n;i++){
        printf("%d ",array[i]);
    }
    printf("\n");
}

int main6(void) {
    int to_do;
    pair relation[MAX_REL_SIZE];
    pair relation_2[MAX_REL_SIZE];
    pair comp_relation[MAX_REL_SIZE];
    int domain[MAX_REL_SIZE];
    int max_elements[MAX_REL_SIZE];
    int min_elements[MAX_REL_SIZE];

    scanf("%d",&to_do);
    int size = read_relation(relation);
    int ordered, size_2, size_3, n_domain;

    switch (to_do) {
        case 1:
            printf("%d ", is_reflexive(relation, size));
            printf("%d ", is_irreflexive(relation, size));
            printf("%d ", is_symmetric(relation, size));
            printf("%d ", is_antisymmetric(relation, size));
            printf("%d ", is_asymmetric(relation, size));
            printf("%d\n", is_transitive(relation, size));
            break;
        case 2:
            ordered = is_partial_order(relation, size);
            n_domain = get_domain(relation, size, domain);
            printf("%d %d\n", ordered, is_total_order(relation, size,domain,n_domain));
            print_int_array(domain, n_domain);
            if (!ordered) break;
            int no_max_elements = find_max_elements(relation, size, max_elements,domain,n_domain);
            int no_min_elements = find_min_elements(relation, size, min_elements,domain,n_domain);
            print_int_array(max_elements, no_max_elements);
            print_int_array(min_elements, no_min_elements);
            break;
        case 3:
            size_2 = read_relation(relation_2);
            size_3 = composition(relation_2, size_2, relation, size, comp_relation);
            printf("%d\n", size_3);
            break;
        default:
            printf("NOTHING TO DO FOR %d\n", to_do);
            break;
    }

    return 0;
}

