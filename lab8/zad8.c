#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TEST 1

typedef struct {
    int day;
    int month;
    int year;
} Date;

/////////////////////////////////////////////////////////////////
// 8.1 funkcja bsearch2

#define FOOD_MAX  30   // max. liczba artykułów
#define ART_MAX   15   // max. długość nazwy artykułu +1
#define RECORD_MAX 40  // max. długość rekordu (linii) w pliku

typedef struct {
    char art[ART_MAX];
    float price;
    float amount;
    Date valid_date;
} Food;

typedef int (*ComparFp)(const void *, const void *);

int compare_price(const void* a, const void* b){
    Food A = *((Food*)a);
    Food B = *((Food*)b);

    if(A.price<B.price){
        return 0;
    }

    else if(A.price>B.price){
        return 2;
    }

    return 1;
}

int compare_date1(const void* a, const void* b){
    Date A = *((Date*)a);
    Date B = *((Date*)b);

    if(A.year<B.year){
        return 0;//A jest wczesniej
    }

    else if(A.year>B.year){
        return 2;//B jest wczesniej
    }

    else{
        if(A.month<B.month){
            return 0;
        }

        else if(A.month>B.month){
            return 2;
        }

        else{
            if(A.day<B.day){
                return 0;
            }

            else if(A.day>B.day){
                return 2;
            }

            return 1;//daty rowne
        }
    }
}

int compare_date(const void* a, const void* b){
    Food A = *((Food*)a);
    Food B = *((Food*)b);

    return compare_date1(&A.valid_date,&B.valid_date);
}

int compare_food(const void* a, const void* b){
    Food A = *((Food*)a);
    Food B = *((Food*)b);

    int x = strcmp(A.art,B.art);//ktory pierwszy alfabetycznie

    if(x < 0){
        return 0;
    }

    else if(x > 0){
        return 2;
    }

    else{
        int price = compare_price(a,b);

        if(price != 1){
           return price;
        }

        else{
            int date = compare_date(a,b);
            return date;
        }
    }
}

void *bsearch2(const void *key, void *base, size_t nitems, size_t size, ComparFp compar, int *result) {
    if(nitems == 0){
        *result = 0;
        return base;
    }

    size_t mid = nitems/2;
    void* mid_ptr = (void*)base + size*mid;

    int compare_art = compar(key,mid_ptr);

    if(compare_art == 1){
        *result = 1;
        return mid_ptr;
    }

    else if(compare_art == 0){
        return bsearch2(key, base, mid, size, compar, result);
    }

    else{
        return bsearch2(key, mid_ptr + size, nitems - mid - 1, size, compar, result);
    }
}

void print_art(Food *p, size_t n, char *art) {
    for(int i=0;i<n;i++){
        if(strcmp(p[i].art,art) == 0){
        printf("%.2f %.2f %i %i %i \n", p[i].price, p[i].amount, p[i].valid_date.day,
                   p[i].valid_date.month, p[i].valid_date.year);
        }
    }
}

Food* add_record(Food *tab, size_t tab_size, int *np, ComparFp compar, Food *new) {
    int result=0;
    Food* adr = bsearch2(new,tab,*np,sizeof(Food),compar,&result);

    if(result == 1){
        adr->amount+=new->amount;//zwiekszenie ilosci jesli te same artykuly
    }

    else{
        if(*np == tab_size){
            return NULL;
        }

        int idx = (int)(adr - tab);
        (*np)++;

        for(int i = *np - 1; i > idx; i--){
            tab[i] = tab[i-1]; // przesuwanie elementów w celu zrobienia miejsca dla new w tab
        }

        tab[idx] = *new;
    }

    return adr;
}

int read_stream(Food *tab, size_t size, int no, FILE *stream) {
    int np=0;

    for(int i = 0; i < no; i++){
        Food F;
        memset(F.art, 0, 15);
        fscanf(stream, "%s", F.art);
        fscanf(stream, "%f", &F.price);
        fscanf(stream, "%f", &F.amount);
        fscanf(stream, "%i %i %i", &F.valid_date.day, &F.valid_date.month, &F.valid_date.year);
        add_record(tab,size,&np,compare_food,&F);

    }

    return np;
}


int read_stream0(Food *tab, size_t size, int no, FILE *stream) {
    for(int i = 0; i < no; i++){
        Food F;
        memset(F.art, 0, 15);
        fscanf(stream, "%s", F.art);
        fscanf(stream, "%f", &F.price);
        fscanf(stream, "%f", &F.amount);
        fscanf(stream, "%i %i %i", &F.valid_date.day, &F.valid_date.month, &F.valid_date.year);
        tab[i] = F;
    }

    return no;
}

int compare1(const void* a, const void* b){
    Date A = *((Date*)a);
    Date B = *((Date*)b);

    if(A.year<B.year){
        return -1;//A jest wczesniej
    }

    else if(A.year>B.year){
        return 1;//B jest wczesniej
    }

    else{
        if(A.month<B.month){
            return -1;
        }

        else if(A.month>B.month){
            return 1;
        }

        else{
            if(A.day<B.day){
                return -1;
            }

            else if(A.day>B.day){
                return 1;
            }

            return 0;//daty rowne
        }
    }
}

int compare(const void* a, const void* b){
    Food A = *((Food*)a);
    Food B = *((Food*)b);

    return compare1(&A.valid_date,&B.valid_date);
}

Date adjust(Date curr_date, int anticip){
    Date date;
    date.day = curr_date.day + anticip;
    date.month = curr_date.month;
    date.year = curr_date.year;

    while(date.day>31){
        if((date.month<=7 && date.month%2 == 1) || (date.month>7 && date.month%2 == 0)){
            date.day-=31;
        }

        else if((date.month<=7 && date.month%2 == 0 && date.month!=2) || (date.month>7 && date.month%2 == 1)){
            date.day-=30;
        }

        else{
            date.day-=28;
        }

        date.month+=1;

    }

    if(date.month>12){
        int x = (date.month-1)/12;
        date.year+=x;
        date.month -= x*12;
    }

    return date;
}

float value(Food *food_tab, size_t n, Date curr_date, int anticip) {
    qsort(food_tab,n,sizeof(Food),compare);
    float sum=0;

    curr_date=adjust(curr_date,anticip);

    for(int i=0;i<n;i++){
        if(food_tab[i].valid_date.day == curr_date.day && food_tab[i].valid_date.month == curr_date.month && food_tab[i].valid_date.year == curr_date.year ){
            sum+=food_tab[i].amount * food_tab[i].price;
        }
    }

    return sum;
}



/////////////////////////////////////////////////////////////////
// 8.3 Sukcesja

#define CHILD_MAX  20   // maksymalna liczba przesuwanej w tablicy grupy osób (elementów tablicy)

enum Sex {F, M};
enum BOOL {no, yes};

struct Bit_data {
    enum Sex sex:1;
    enum BOOL pretendent:1;   // =no - nie pretenduje (panuje albo nie żyje) ale jest w zbiorze dla spójności relacji.
};

typedef struct {
    char *name;
    struct Bit_data bits;
    Date born;
    char *parent;
} Person;

typedef struct {
    char *par_name;
    int index;
} Parent;    // strukturę można rozbudować o pole liczby dzieci

const Date primo_date = { 28, 10, 2011 }; // Data zmiany kolejności sukcesji

int fill_indices_tab(Parent *idx_tab, Person *pers_tab, int size) {
}

void persons_shiftings(Person *person_tab, int size, Parent *idx_tab, int no_parents) {
}

int cleaning(Person *person_tab,int n) {
}

void print_person(const Person *p) {
    printf("%s\n", p->name);
}

void print_persons(const Person *person_tab, int n) {
    for(int i=1; i<=n; ++i, ++person_tab) printf("%2d %12s %s\n", i, person_tab->name, person_tab->parent);
    return;
}

int create_list(Person *person_tab, int n) {
}

////////////////////////////////////////////////////////////////

int main8(void)
{
    // Wszyscy potomkowie Jerzego VI (w kolejności przypadkowej):
    Person person_tab[33]={
            {"Charles III",M,no,14,11,1948,"Elizabeth II"},
            {"Anne",F,yes,15,8,1950,"Elizabeth II"},
            {"Andrew",M,yes,19,2,1960,"Elizabeth II"},
            {"Edward",M,yes,10,3,1964,"Elizabeth II"},
            {"David",M,yes,3,11,1961,"Margaret"},
            {"Sarah",F,yes,1,5,1964,"Margaret"},
            {"William",M,yes,21,6,1982,"Charles III"},
            {"Henry",M,yes,15,9,1984,"Charles III"},
            {"Peter",M,yes,15,11,1977,"Anne"},
            {"Zara",F,yes,15,5,1981,"Anne"},
            {"Beatrice",F,yes,8,8,1988,"Andrew"},
            {"Eugenie",F,yes,23,3,1990,"Andrew"},
            {"James",M,yes,17,12,2007,"Edward"},
            {"Louise",F,yes,8,11,2003,"Edward"},
            {"Charles",M,yes,1,7,1999,"David"},
            {"Margarita",F,yes,14,5,2002,"David"},
            {"Samuel",M,yes,28,7,1996,"Sarah"},
            {"Arthur",M,yes,6,5,2019,"Sarah"},
            {"Georg",M,yes,22,7,2013,"William"},
            {"George VI",M,no,14,12,1895,NULL},
            {"Charlotte",F,yes,22,5,2015,"William"},
            {"Louis",M,yes,23,4,2018,"William"},
            {"Archie",M,yes,6,5,2019,"Henry"},
            {"Lilibet",F,yes,4,6,2021,"Henry"},
            {"Savannah",F,yes,29,12,2010,"Peter"},
            {"Isla",F,yes,29,3,2012,"Peter"},
            {"Mia",F,yes,17,1,2014,"Zara"},
            {"Lena",F,yes,18,6,2018,"Zara"},
            {"Elizabeth II",F,no,21,4,1925,"George VI"},
            {"Margaret",F,no,21,8,1930,"George VI"},
            {"Lucas",M,yes,21,3,2021,"Zara"},
            {"Sienna",F,yes,18,9,2021,"Beatrice"},
            {"August",M,yes,9,2,2021,"Eugenie"}
    };

    int to_do, no;
    size_t size, n;
    Food food_tab[FOOD_MAX];
    char buff[ART_MAX];
    FILE *file;
    if(TEST) printf("Wpisz nr zadania (1 - 3) ");
    scanf("%d", &to_do);

    switch (to_do) {
        case 1:  // bsearch2
            if (TEST) printf("Wpisz liczbe linii danych: ");
            scanf("%d",&no); getchar();
            if(TEST) file = stdin;
            else {
                scanf("%s",buff);
                file = fopen(buff,"r");
//            file = fopen("foods0.txt","r");
                if(file==NULL) { printf("Error 1\n"); break; }
            }
            if (TEST) printf("W %d liniach wpisuj dane: nazwa cena ilosc dd mm yyyy: \n",no);
            n = read_stream(food_tab,FOOD_MAX,no,file);
            if (TEST) printf("Wpisz nazwe artykulu: ");
            scanf("%s",buff);
            print_art(food_tab,n,buff);
            break;
        case 2: // proste sortowanie struktur
            if (TEST) printf("Wpisz liczbe linii danych: ");
            scanf("%d",&no); getchar();
            if(TEST) file = stdin;
            else {
                scanf("%s",buff);
                file = fopen(buff,"r");
//            file = fopen("foods0.txt","r");
                if(file==NULL) { printf("Error 1\n"); break; }
            }
            if (TEST) printf("W %d liniach wpisuj dane: nazwa cena ilosc dd mm yyyy: \n",no);
            n = read_stream0(food_tab,FOOD_MAX,no,file);
            Date curr_date;
            int anticip;
            if (TEST) printf("Wpisz date odniesienia dd mm yyyy: ");
            scanf("%d %d %d",&curr_date.day,&curr_date.month,&curr_date.year);
            if (TEST) printf("Wpisz roznice dni: ");
            scanf("%d",&anticip);
            printf("%.2f\n",value(food_tab,n,curr_date,anticip));
            break;
        case 3: // sukcesja
            if(TEST==1) printf("Wpisz pozycję w kolejce (liczona od 1): ");
            scanf("%d",&no);
            int no_persons = sizeof(person_tab) / sizeof(Person);
            no_persons = create_list(person_tab,no_persons);
            if(TEST) print_persons(person_tab,no_persons);
            else print_person(person_tab+no-1);
            break;
        default:
            printf("NOTHING TO DO FOR %d\n", to_do);
    }
    return 0;
}

