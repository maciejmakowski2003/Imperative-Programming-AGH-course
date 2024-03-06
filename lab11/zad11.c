#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

#define BUFFER_SIZE 1000
#define MEMORY_ALLOCATION_ERROR  -1
#define FILE_OPEN_ERROR          -2
#define LIST_ERROR               -3
#define TEST 0               // 1 dla testowania, 0 dla automatycznej oceny

typedef struct tagListElement {
    struct tagListElement *next;
    void                  *data;
} ListElement;

typedef void (*ConstDataFp)(const void*);
typedef void (*DataFp)(void*);
typedef  int (*CompareDataFp)(const void*,const void*);

typedef struct tagList {
    ListElement *head;
    ListElement *tail;
    int         size;
    ConstDataFp dumpData;
    DataFp      freeData;
    CompareDataFp compareData;
    DataFp      modifyData;
} List;


////////////////////////////////////////////////

// Funkcje pomocnicze
void *safe_malloc(size_t size) {
    void *ptr = malloc(size);
    if (ptr) return ptr;
    printf("malloc error\n");
    exit(MEMORY_ALLOCATION_ERROR);
}
char *safe_strdup(char *str) {
    char *ptr = strdup(str);
    if (ptr) return ptr;
    printf("safe_strdup error\n");
    exit(MEMORY_ALLOCATION_ERROR);
}

////////////////////////////////////////////////////////////////
// Funkcje uniwersalne (ogolnego przeznaczenia) obsługi listy

// Inicjuje listę
void init_List(List *list, ConstDataFp dump, DataFp free, CompareDataFp cmp, DataFp modify) {
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    list->dumpData = dump;
    list->freeData = free;
    list->compareData = cmp;
    list->modifyData = modify;
}

// Wypisuje dane elemntów listy
void dumpList(const List* list) {
    if(list->dumpData == NULL) return;
    ListElement *i;
    for(i=list->head; i!=0; i=i->next) list->dumpData(i->data);
}

// Zwalnia pamięć elementów listy i danych
void freeList(List *list) {
    ListElement *current;
    current = list->head;
    while(current) {
        ListElement *todelete = current;
        current = current->next;
        if(list->freeData) list->freeData(todelete->data); // zwolnienie pamięci struktury data
        free(todelete);              // zwolnienie pamięci elementu
    }
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

// Uniwersalne funkcje obsługi elementów listy

// Dodaje element na początku listy
void pushFront(List *list, void *data) {
    ListElement *elem = safe_malloc(sizeof(ListElement));
    elem->next = list->head;
    elem->data = data;
    if(list->head != 0) list->head = elem;
    else list->head = list->tail = elem;
    list->size++;
}

// Dodaje element na końcu listy
void pushBack(List *list, void *data) {
    ListElement *elem = safe_malloc(sizeof(ListElement));
    elem->next = 0;
    elem->data = data;
    if(list->tail != 0){
        list->tail->next = elem;
        list->tail = elem;
    }
    else list->head = list->tail = elem;
    list->size++;
}

// Usuwa pierwszy element listy
void popFront(List *list) {
    ListElement* todelete;
    if(list->head == 0) return;
    todelete = list->head;
    list->head = list->head->next;
    if(list->head == 0) list->tail = 0;
    list->freeData(todelete->data);
    free(todelete);
    list->size--;
}

// Odwraca kolejność elementów listy
void reverse(List *list) {
    if(list->head == 0) return;
    ListElement *first = list->head;
    ListElement *second = list->head->next;
    ListElement *tmp;


    list->tail = first;
    first->next = 0;

    while(second != 0){
        tmp = second->next;
        second->next = first;
        first = second;
        second = tmp;
    }

    list->head = first;
}

// Funkcje pomocnicze dla list uporzadkowanych
// (porządek wyznacza funkcja wskazana polem compareData)

// Zwraca element w posortowanej liście, a jeżeli nie ma, to element poprzedzający (nie ma, to NULL)
ListElement* findInsertionPoint(const List *list, void *element) {
    ListElement* curr = list->head;
    if(!curr || list->compareData(curr->data,element)>0) return NULL;
    if (!list->compareData(curr->data, element)) return curr;

    while (curr->next && list->compareData(curr->next->data, element) < 0) {
        curr = curr->next;
    }

    if (curr->next && !list->compareData(curr->next->data, element)) return curr->next;
    return curr;
}


// Drugi parametr przekazuje do funkcji adres przydzielonej dla "nowych" danych pamięci.
// Jeżeli takie dane są już w elemencie listy, to dane tego elementu są modyfikowane
// funkcją wskazaną w polu modifyData, a pamięć "nowych" danych jest zwalniana.
// Jeżeli w liście takiego elementu nie ma, to nowy element jest tworzony
// i dopisany do listy zachowując uporządkowanie listy.
void insertInOrder(List *list, void *a) {
    ListElement* insert_pt = findInsertionPoint(list,a);

    if(!insert_pt){
        pushFront(list, a);
        return;
    }

    if (!list->compareData(insert_pt->data, a)) {
        list->freeData(a);
        if (list->modifyData) list->modifyData(insert_pt->data);
        return;
    }

    ListElement* elem = safe_malloc(sizeof(ListElement));
    ListElement* tmp = insert_pt->next;
    elem->data = a;

    insert_pt->next = elem;
    elem->next = tmp;

    if (list->tail == insert_pt) list->tail = elem;
}


/////////////////// Funkcje do zadania 11.1.2

// Dodaje element na końcu listy bez korzystania z pola tail
void pushBack_v0(List *list, void *data) {
    ListElement* elem = safe_malloc(sizeof(ListElement));
    elem->next=0;
    elem->data=data;

    if(list->head == 0) list->head = list->tail = elem;
    else{
        ListElement* curr = list->head;
        while(curr->next!=0){
            curr = curr->next;
        }
        curr->next = elem;
        list->tail = elem;
    }
    list->size++;
}


// Funkcje warstwy obsługi danych wskazywanych w polach struktury List


//////  Dla zadania 11.1.1 i 11.1.2
// 1. Dana jest liczbą typu int

typedef int DataInt;

void dump_int(const void *d) {
    int data = *(int*)d;
    printf("%d ", data);
}

void free_int(void *d) {
    free(d);
}

int cmp_int(const void *a, const void *b) {
    return ( *(int*)a - *(int*)b );
}

// Przydziela pamięć i zapisuje w niej daną o wartości v
void *create_data_int(int v) {
    int* elem = safe_malloc(sizeof(int));
    *elem = v;
    return elem;
}

//////  Dla zadania 11.1.3 i 11.1.4
// 2. Dana jest zapisana w  strukturze typu DataWord

typedef struct DataWord {
    char *word;
    int counter;
} DataWord;

void dump_word (const void *d) {
    DataWord* elem = (DataWord*)d;
    printf("%s ", elem->word);
}

void free_word(void *d) {
    DataWord* elem = (DataWord*)d;
    free(elem->word);
    free(elem);
}

char *to_lower(char *s){
    for (char* i = s; *i; ++i) *i = tolower(*i);
    return s;
}

int cmp_word_alphabet(const void *a, const void *b) {
    DataWord *elema = (DataWord*)a, *elemb = (DataWord*)b;

    return strcmp(elema->word, elemb->word);
}

int cmp_word_counter(const void *a, const void *b) {
    DataWord* elem = (DataWord*) a;
    int val = *(int*)b;

    return elem->counter==val;
}

void modify_word(void *a) {
    DataWord* elem = (DataWord*)a;
    elem->counter++;
}

// Wypisz dane elementów spełniających warunek równości sprawdzany funkcją
// wskazywaną w polu compareData nagłówka listy
void dumpList_word_if(List *plist, int n) {
    ListElement* curr;
    for (curr = plist->head; curr != 0; curr = curr->next) {
        if (plist->compareData(curr->data, &n)) {
            plist->dumpData(curr->data);
        }
    }
}

// Przydziela pamięć dla łańcucha string i struktury typu DataWord.
// Do przydzielonej pamięci wpisuje odpowiednie dane.
// Zwraca adres struktury.
void *create_data_word(char *string, int counter) {
    DataWord* elem = safe_malloc(sizeof(DataWord));

    elem->counter = counter;
    elem->word = safe_malloc(sizeof(char)*strlen(string));
    memcpy(elem->word, string, sizeof(char)*strlen(string));
    return elem;
}

//////////////////////////////////////////////////

/////////////////// Funkcje pomocnicze dla zadan 11.1.3 i 11.1.4

// Wyrazy ze strumienia wejściowego zapisuje w elementach listy
// w kolejności określonej ostatnim parametrem funkcji (cmp)
// - adresem funkcji porównującej typu CompareDataFp. Adres zerowy oznacza,
// że kolejność elementów listy jest zgodna z kolejnością
void stream_to_list(List *list, FILE *stream, CompareDataFp cmp) {
    DataWord *data;
    char *p, buf[BUFFER_SIZE];
    char delimits[] = " \r\t\n.,?!-";
    if(cmp) list->compareData = cmp;
    while(fgets(buf,BUFFER_SIZE,stream)) {
        p = strtok(buf, delimits);
        while(p) {
            data = create_data_word(p, 1);
            if(cmp) {
                data->word = to_lower(data->word);
                insertInOrder(list,data);
            } else pushBack(list, data);
            p = strtok(NULL,delimits);
        }

    }
}

void list_test(List *plist, int n) {
    char op[2];
    int v;
    ListElement element, *pelement;
    DataInt data;
    for (int i = 0; i < n; ++i) {
        if(TEST) printf("Wpisz kod polecenia ");
        scanf("%s", op);
        switch (op[0]) {
            case 'f': // dodaj na początku listy
                if(TEST) printf("Wpisz wartość ");
                scanf("%d", &v);
                pushFront(plist,create_data_int(v));
                break;
            case 'b': // dodaj na końcu listy
                if(TEST) printf("Wpisz wartość ");
                scanf("%d", &v);
                pushBack(plist,create_data_int(v));
                break;
            case 'd': // usuń pierwszy element
                popFront(plist);
                break;
            case 'r': // odwróć kolejność
                reverse(plist);
                break;
            case 'i': // dodaj wg porządku rosnącego (int) lub alfabetycznego (char),
                // jeżeli element już istnieje, to akcja zależna od typu danych
                if(TEST) printf("Wpisz wartość ");
                scanf("%d",&v);
                insertInOrder(plist,create_data_int(v));
                break;
            default:
                printf("No such operation: %s\n", op);
                break;
        }
    }
}

int main11(void) {
    int to_do, n, size;
    char buf[257];
    clock_t t_start, t_end;
    double t_total;
    FILE *stream;
    List list;
    ListElement *head;

    if(TEST) printf("Wpisz nr zadania ");
    scanf ("%d", &to_do);
    switch (to_do) {
        case 1:
            if(TEST) printf("Wpisz liczbę poleceń ");
            scanf("%d",&n);
            head = NULL;
            init_List(&list, dump_int, free_int, cmp_int, NULL);
//        list_test(&list, n, int (cmp)(const void*, const void*))
            list_test(&list, n);
            dumpList(&list);
            freeList(&list);
            break;
        case 2:
            if(TEST) printf("Wpisz liczbę elementów ");
            scanf("%d",&size);
            init_List(&list, dump_int, free_int, cmp_int, NULL);
            t_start = clock();
            for(int i=0; i<size; ++i) pushBack_v0(&list,create_data_int(i));
            t_end = clock();
            t_total = (double)(t_end - t_start) / CLOCKS_PER_SEC;
            printf("n = %d. Back bez tail.  Czas = %g s.\n",size,t_total);
            freeList(&list);

            init_List(&list, dump_int, free_int, cmp_int, NULL);
            t_start = clock();
            for(int i=0; i<size; ++i) pushFront(&list,create_data_int(i));
            reverse(&list);
            t_end = clock();
            t_total = (double)(t_end - t_start) / CLOCKS_PER_SEC;
            printf("n = %d. Front + revers. Czas = %g s.\n",size,t_total);
            freeList(&list);

            init_List(&list, dump_int, free_int, cmp_int, NULL);
            t_start = clock();
            for(int i=0; i<size; ++i) pushBack(&list,create_data_int(i));
            t_end = clock();
            t_total = (double)(t_end - t_start) / CLOCKS_PER_SEC;
            printf("n = %d. Back z tail.    Czas = %g s.\n",size,t_total);
            freeList(&list);
            break;
        case 3:      // wypisz wszystkie słowa wczytane do elementów listy
            init_List(&list,dump_word,free_word,NULL,NULL);
            if(TEST) {
                stream = stdin;
                printf("Wpisz linie tekstu\n");
                stream_to_list(&list, stream, NULL);
                rewind(stream);
            }
            else {
                stream = fopen("Tekst_1.txt","r");
                if(stream == NULL) exit(FILE_OPEN_ERROR);
                stream_to_list(&list, stream, NULL);
                fclose(stream);
            }
            dumpList(&list);
            freeList(&list);
            break;
        case 4:      // wypisz w porządku alfabetycznym słowa o zadanej krotności
            if(TEST) printf("Wpisz krotność słowa ");
            scanf("%d",&n);
            init_List(&list,dump_word,free_word,NULL,modify_word);
            if(TEST) {
                stream = stdin;
                printf("Wpisz linie tekstu\n");
                stream_to_list(&list, stream, cmp_word_alphabet);
                rewind(stream);
            }
            else {
                stream = fopen("Tekst_C.txt","r");
                if(stream == NULL) exit(FILE_OPEN_ERROR);
                stream_to_list(&list, stream, cmp_word_alphabet);
                fclose(stream);
            }
            list.compareData = cmp_word_counter;
            dumpList_word_if(&list,n); printf("\n");
            freeList(&list);
            break;
        default:
            printf("NOTHING TO DO FOR %d\n", to_do);
    }
    return 0;
}

