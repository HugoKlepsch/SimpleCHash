#ifndef __HKLEPSCH_HASHTABLE__
#define __HKLEPSCH_HASHTABLE__

typedef struct Element {
    char * key;
    void * data;
    struct Element * next;
} Element;

typedef struct HashTable {
    Element ** table;
    int numElements;
    int (*hashFn)(char * key);
} HashTable;

#define EXIT_FAILURE 0
#define EXIT_SUCCESS 1

int hashStr(char * string);

HashTable * initTable(int size, int (*hashFn)(char * key));

int insert_hash(HashTable * table, char * key, void * data);

void * get_hash(HashTable * table, char * key);

Element * makeElement(char * key, void * data);

void * freeElement(Element * element, void (*freeFn)(char * key, void * data));

void destroyTable(HashTable * table, void (*freeFn)(char * key, void * data));


#endif
