#ifndef __HKLEPSCH_HASHTABLE__
#define __HKLEPSCH_HASHTABLE__

typedef struct Element {
    char * key;
    void * data;
    struct Element * next;
} Element;

typedef struct HashTable {
    Element ** table;
    int numSlots;
    unsigned int (*hashFn)(char * key);
} HashTable;

typedef struct TableStats {
    int numSlots;
    int numElements;
    int numCollisions;
    int mostCollisions;
    int leastCollisions;
} TableStats;

#define EXIT_FAILURE 0
#define EXIT_SUCCESS 1

/**
 * hashStr
 * Given a string, return a simple hash of the string by summing it's
 *  ascii values.
 * IN:
 *  string - the string to hash
 * OUT:
 *  The hash of the string.
 */
unsigned int hashStr(char * string);

/**
 * HashTable
 * Given the number of slots to allocate and a hashing function,
 *  return a new table with those properties.
 * IN:
 *  size - The number of slots to allocate
 *  hashFn - The function to use for hashing a given key
 * OUT:
 *  An initialized hash table
 */
HashTable * initTable(int size, unsigned int (*hashFn)(char * key));

/**
 * insert_hash
 * Given a key and a piece of data, insert the data into the table indexed
 *  on the key.
 * IN:
 *  table - the table to insert into
 *  key - the key to associate with the data
 *  data - the data to store
 */
int insert_hash(HashTable * table, char * key, void * data);

/**
 * get_hash
 * Given a key, return the data associated with that key from the table.
 * IN:
 *  table  - the table to search in
 *  key - the key to search for in the table
 * OUT:
 *  The data associated with key, or NULL if key doesn't exist
 */
void * get_hash(HashTable * table, char * key);

/**
 * makeElement
 * Given a key and it's data, allocate and initialize an Element.
 * IN:
 *  key - the key to assign to the Element
 *  data - the data to assign to the Element
 * OUT:
 *  An element with key and data as it's members
 */
Element * makeElement(char * key, void * data);

/**
 * freeElement
 * Given an Element, free it's contents and return the pointer to the next
 *  Element in the chain, or NULL if there is no next Element.
 * IN:
 *  element - the element to free
 *  freeFn - the function to call on the element's key and data
 * OUT:
 *  The next Element in the chain, or NULL if there is no next element.
 */
void * freeElement(Element * element, void (*freeFn)(char * key, void * data));

/**
 * destroyTable
 * Given a table, free it and it's contents in it's entirety
 * IN:
 *  table - the table to destroy
 *  freeFn - the function used to free each element's key and data members
 */
void destroyTable(HashTable * table, void (*freeFn)(char * key, void * data));

TableStats getTableStats(HashTable * table);

#endif
