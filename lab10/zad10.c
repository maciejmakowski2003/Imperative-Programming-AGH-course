#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX_STR_LEN 64

typedef struct Vector {
    void *data;
    size_t element_size;
    size_t size;
    size_t capacity;
} Vector;

typedef struct Person {
    int age;
    char first_name[MAX_STR_LEN];
    char last_name[MAX_STR_LEN];
} Person;


typedef int(*cmp_ptr)(const void*, const void*);
typedef int(*predicate_ptr)(void*);
typedef void(*read_ptr)(void*);
typedef void(*print_ptr)(const void*);

// Allocate vector to initial capacity (block_size elements),
// Set element_size, size (to 0), capacity
void init_vector(Vector *vector, size_t block_size, size_t element_size) {
    vector->element_size = element_size;
    vector->size = 0;
    vector->capacity = block_size;
    vector->data = malloc(block_size*element_size);
}

// If new_capacity is greater than the current capacity,
// new storage is allocated, otherwise the function does nothing.
void reserve(Vector *vector, size_t new_capacity) {
    if (new_capacity > vector->capacity) {
        vector->capacity = new_capacity;
        vector->data = realloc(vector->data, vector->capacity*vector->element_size);
    }
}

// Resizes the vector to contain new_size elements.
// If the current size is greater than new_size, the container is
// reduced to its first new_size elements.

// If the current size is less than new_size,
// additional zero-initialized elements are appended
void resize(Vector *vector, size_t new_size) {
    if (vector->capacity < new_size) reserve(vector, 2*vector->capacity);

    if (new_size > vector->size)
        memset(vector->data+vector->size*vector->element_size, 0, (new_size-vector->size)*vector->element_size);
    vector->size = new_size;
}

// Add element to the end of the vector
void push_back(Vector *vector, void *value) {
    resize(vector, vector->size+1);
    memcpy(vector->data+(vector->size-1)*vector->element_size, value, vector->element_size);
}

// Remove all elements from the vector
void clear(Vector *vector) {
    resize(vector, 0);
}

// Insert new element at index (0 <= index <= size) position
void insert(Vector *vector, size_t index, void *value) {
    resize(vector, vector->size+1);
    memmove(vector->data+(index+1)*vector->element_size, vector->data+index*vector->element_size, (vector->size-index-1)*vector->element_size);
    memcpy(vector->data+index*vector->element_size, value, vector->element_size);
}

// Erase element at position index
void erase(Vector *vector, size_t index) {
    memmove(vector->data+index*vector->element_size, vector->data+(index+1)*vector->element_size, (vector->size-index)*vector->element_size);
    resize(vector, vector->size-1);
}

// Erase all elements that compare equal to value from the container
void erase_value(Vector *vector, void *value, cmp_ptr cmp) {
    for (int i = 0; i < vector->size; i++) {
        if (!cmp(vector->data+i*vector->element_size, value)) {
            erase(vector, i);
            i--;
        }
    }
}

// Erase all elements that satisfy the predicate from the vector
void erase_if(Vector *vector, int (*predicate)(void *)) {
    for (int i = 0; i < vector->size; i++) {
        if (predicate(vector->data+i*vector->element_size)) {
            erase(vector, i);
            i--;
        }
    }
}

// Request the removal of unused capacity
void shrink_to_fit(Vector *vector) {
    vector->data = realloc(vector->data, vector->size*vector->element_size);
    vector->capacity = vector->size;
}

// integer comparator
int int_cmp(const void *v1, const void *v2) {
    int a = *((int*)v1);
    int b = *((int*)v2);
    return a-b;
}

// char comparator
int char_cmp(const void *v1, const void *v2) {
    char *a = (char*)v1;
    char *b = (char*)v2;
    return *a-*b;
}

// Person comparator:
// Sort according to age (decreasing)
// When ages equal compare first name and then last name
int person_cmp(const void *p1, const void *p2) {
    Person *a = (Person*)p1;
    Person *b = (Person*)p2;

    if (a->age != b->age) return b->age-a->age;

    int cmp = strcmp(a->first_name, b->first_name);

    if (cmp != 0) return cmp;

    return  strcmp(a->last_name, b->last_name);
}

// predicate: check if number is even
int is_even(void *value) {
    int a = *(int*)value;
    return 1-a%2;
}

// predicate: check if char is a vowel
int is_vowel(void *value) {
    char vowels[12] = {'a', 'e', 'i', 'o', 'u', 'y', 'A', 'E', 'I', 'O', 'U', 'Y'};
    char c = *(char*)value;

    for (int i = 0; i < 12; ++i)
        if (vowels[i] == c) return 1;
    return 0;
}

// predicate: check if person is older than 25
int is_older_than_25(void *person) {
    Person p = *((Person*)person);
    return p.age > 25;
}

// print integer value
void print_int10(const void *v) {
    int a = *(int*)v;
    printf("%d ", a);
}

// print char value
void print_char10(const void *v) {
    char c = *(char*)v;
    printf("%c ", c);
}

// print structure Person
void print_person10(const void *v) {
    Person *p = (Person*)v;
    printf("%d %s %s\n", p->age, p->first_name, p->last_name);
}

// print capacity of the vector and its elements
void print_vector10(Vector *vector, print_ptr print) {
    printf("%d\n", vector->capacity);
    for (int i = 0; i < vector->size; ++i) {
        print(vector->data+i*vector->element_size);
    }
    printf("\n");
}

// read int value
void read_int10(void* value) {
    int a;
    scanf("%d", &a);
    memcpy(value, &a, sizeof(int));
}

// read char value
void read_char10(void* value) {
    char c;
    scanf(" %c", &c);
    memcpy(value, &c, sizeof(char));
}

// read struct Person
void read_person10(void* value) {
    Person p;
    scanf(" %d %s %s", &p.age, p.first_name, p.last_name);
    memcpy(value, &p, sizeof(Person));
}

void vector_test(Vector *vector, size_t block_size, size_t elem_size, int n, read_ptr read,
                 cmp_ptr cmp, predicate_ptr predicate, print_ptr print) {
    init_vector(vector, block_size, elem_size);
    void *v = malloc(vector->element_size);
    size_t index, size;
    for (int i = 0; i < n; ++i) {
        char op;
        scanf(" %c", &op);
        switch (op) {
            case 'p': // push_back
                read(v);
                push_back(vector, v);
                break;
            case 'i': // insert
                scanf("%zu", &index);
                read(v);
                insert(vector, index, v);
                break;
            case 'e': // erase
                scanf("%zu", &index);
                erase(vector, index);
                break;
            case 'v': // erase
                read(v);
                erase_value(vector, v, cmp);
                break;
            case 'd': // erase (predicate)
                erase_if(vector, predicate);
                break;
            case 'r': // resize
                scanf("%zu", &size);
                resize(vector, size);
                break;
            case 'c': // clear
                clear(vector);
                break;
            case 'f': // shrink
                shrink_to_fit(vector);
                break;
            case 's': // sort
                qsort(vector->data, vector->size,
                      vector->element_size, cmp);
                break;
            default:
                printf("No such operation: %c\n", op);
                break;
        }
    }
    print_vector10(vector, print);
    free(vector->data);
    free(v);
}

int main10(void) {
    int to_do, n;
    Vector vector_int, vector_char, vector_person;

    scanf("%d%d", &to_do, &n);

    switch (to_do) {
        case 1:
            vector_test(&vector_int, 4, sizeof(int), n, read_int10, int_cmp,
                        is_even, print_int10);
            break;
        case 2:
            vector_test(&vector_char, 2, sizeof(char), n, read_char10, char_cmp,
                        is_vowel, print_char10);
            break;
        case 3:
            vector_test(&vector_person, 2, sizeof(Person), n, read_person10,
                        person_cmp, is_older_than_25, print_person10);
            break;
        default:
            printf("Nothing to do for %d\n", to_do);
            break;
    }

    return 0;
}


