#ifndef __HKLEPSCH_HASHTABLE__
#define __HKLEPSCH_HASHTABLE__

/**
 * struct Element
 * Each item inserted gets assigned to one Element.
 * key - The key associated with the item.
 * data - The data associated with the item.
 * next - The next item in the chain, if two items hash to the same slot.
 */
typedef struct Element {
    char * key;
    void * data;
    struct Element * next;
} Element;

/**
 * struct HashTable
 * dataStore - Where all the data is actually stored.
 * numSlots - The number of slots in the table.
 * hashFn - The function to use for hashing keys.
 */
typedef struct HashTable {
    Element ** dataStore;
    int numSlots;
    unsigned int (*hashFn)(char * key);
} HashTable;

/**
 * TableStats
 * numSlots - The number of slots in the table, not a hard limit on the number
 *  of elements a table can hold.
 * numElements - The number of elements inserted into the table.
 * numCollisions - The number of elements that hash to the same slot as other
 *  elements in the table.
 * mostCollisions - The slot with the most number of colliding elements.
 * leastCollisions - The slot with the least number of colliding elements.
 */
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
 * simpleHashStr
 * Given a string, return a simple hash of the string by summing it's
 *  ascii values.
 * IN:
 *  string - The string to hash.
 * OUT:
 *  The hash of the string.
 */
unsigned int simpleHashStr(char * string);

/**
 * sdbmStringHash
 * Given a string, return a sdbm hash of the string.
 * IN:
 *  string - The string to hash.
 * OUT:
 *  The hash of the string.
 */
unsigned int sdbmStringHash(char * key);

/**
 * hashStr
 * Given a string, return a djb2 hash of the string.
 * IN:
 *  string - The string to hash.
 * OUT:
 *  The hash of the string.
 */
unsigned int djb2StringHash(char * key);

/**
 * initTable
 * Given the number of slots to allocate and a hashing function,
 *  return a new table with those properties.
 * IN:
 *  size - The number of slots to allocate.
 *  hashFn - The function to use for hashing a given key.
 * OUT:
 *  An initialized hash table
 */
HashTable * initTable(int size, unsigned int (*hashFn)(char * key));

/**
 * insert_hash
 * Given a key and a piece of data, insert the data into the table indexed
 *  on the key.
 * IN:
 *  table - The table to insert into.
 *  key - The key to associate with the data.
 *  data - The data to store.
 */
int insert_hash(HashTable * table, char * key, void * data);

/**
 * get_hash
 * Given a key, return the data associated with that key from the table.
 * IN:
 *  table  - The table to search in.
 *  key - The key to search for in the table.
 * OUT:
 *  The data associated with key, or NULL if key doesn't exist
 */
void * get_hash(HashTable * table, char * key);

/**
 * makeElement
 * Given a key and it's data, allocate and initialize an Element.
 * IN:
 *  key - The key to assign to the Element.
 *  data - The data to assign to the Element.
 * OUT:
 *  An element with key and data as it's members
 */
Element * makeElement(char * key, void * data);

/**
 * freeElement
 * Given an Element, free it's contents and return the pointer to the next
 *  Element in the chain, or NULL if there is no next Element.
 * IN:
 *  element - The element to free.
 *  freeFn - The function to call on the element's key and data.
 * OUT:
 *  The next Element in the chain, or NULL if there is no next element.
 */
void * freeElement(Element * element, void (*freeFn)(char * key, void * data));

/**
 * destroyTable
 * Given a table, free it and it's contents in it's entirety
 * IN:
 *  table - The table to destroy.
 *  freeFn - The function used to free each element's key and data members.
 */
void destroyTable(HashTable * table, void (*freeFn)(char * key, void * data));

/**
 * getTableStats
 * Given a table, generate some statistics on the table.
 * IN:
 *  table - The table to analyze.
 * OUT:
 *  Stats on the table, described in the TableStats struct
 */
TableStats getTableStats(HashTable * table);

#endif
