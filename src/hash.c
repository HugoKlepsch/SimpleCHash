#include <hash.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

unsigned int simpleHashStr(char * string) {
    unsigned int hashAddress = 0;

    /* sum each letter of the string */
    while (*string != '\0') {
        hashAddress += *string;
        string++;
    }
    return hashAddress;
}

unsigned int sdbmStringHash(char * key) {
    unsigned int hashAddress = 0;
    int i;

    for (i = 0; key[i] != '\0'; ++i) {
        hashAddress = key[i] + (hashAddress << 6) + (hashAddress << 16) - hashAddress;
    }
    return hashAddress;
}

unsigned int djb2StringHash(char * key) {
    unsigned int hashAddress = 5381;
    int i;

    for (i = 0; key[i] != '\0'; ++i) {
        hashAddress = ((hashAddress << 5) + hashAddress) + key[i];
    }
    return hashAddress;
}

HashTable * initTable(int size, unsigned int (*hashFn)(char * key)) {
    HashTable * table = malloc(sizeof(*table));

    if (!table) {
        return NULL;
    }

    table->dataStore = malloc(sizeof(*table->dataStore) * size);

    if (!table->dataStore) {
        free(table);
        return NULL;
    }

    table->numSlots = size;
    table->hashFn = hashFn;

    int i;
    for (i = 0; i < table->numSlots; ++i) {
        table->dataStore[i] = NULL;
    }

    return table;
}

int insert_hash(HashTable * table, char * key, void * data) {
    unsigned int index;

    /* We don't really care if the data itself is NULL because the user might
     * be storing it specifically */
    if (!table || !key) {
        return EXIT_FAILURE;
    }

    index = table->hashFn(key) % table->numSlots;

    Element * elementToInsertAt = makeElement(key, data);
    elementToInsertAt->next = table->dataStore[index];
    table->dataStore[index] = elementToInsertAt;
}

void * get_hash(HashTable * table, char * key) {
    unsigned int index;

    if (!table || !key) {
        return NULL;
    }

    index = table->hashFn(key) % table->numSlots;

    /* if it's NULL, that means there's no element with that key */
    if (table->dataStore[index] == NULL) {
        return NULL;
    } else {
        Element * t = table->dataStore[index];
        do {
            if (strcmp(key, t->key) == 0) {
                return t->data;
            }
            t = t->next;
        } while(t->next != NULL);
        return NULL;
    }
}

Element * makeElement(char * key, void * data) {
    Element * element = malloc(sizeof(*element));

    if (!element) {
        return NULL;
    }

    element->key = key;
    element->data = data;
    element->next = NULL;
    return element;
}

void * freeElement(Element * element, void (*freeFn)(char * key, void * data)) {
    if (element == NULL) {
        return NULL;
    }

    freeFn(element->key, element->data);

    Element * temp = element->next;
    free(element);

    return temp;
}

void destroyTable(HashTable * table, void (*freeFn)(char * key, void * data)) {
    int i;
    /* for every slot */
    for (i = 0; i < table->numSlots; ++i) {

        /*free each element in the chain */
        Element * temp = freeElement(table->dataStore[i], freeFn);

        while (temp != NULL) {
            temp = freeElement(temp, freeFn);
        }
    }

    free(table->dataStore);
    free(table);
}

TableStats getTableStats(HashTable * table) {
    TableStats stats;
    stats.numSlots = table->numSlots;
    stats.numElements = 0;
    stats.numCollisions = 0;
    stats.mostCollisions = 0;
    stats.leastCollisions = INT_MAX;

    int i;
    for (i = 0; i < table->numSlots; ++i) {
        Element * t = table->dataStore[i];

        if (t == NULL) {
            stats.leastCollisions = 0;
        } else {
            stats.numElements += 1;

            int chainLength = 0;

            while (t->next != NULL) {
                stats.numElements += 1;
                chainLength += 1;

                t = t->next;
            }

            if (chainLength != 0) {
                /* If there was a collision, also count the first element
                 * as having collided */
                chainLength += 1;
            }

            stats.numCollisions += chainLength;
            if (chainLength > stats.mostCollisions) {
                stats.mostCollisions = chainLength;
            }

            if (chainLength < stats.leastCollisions) {
                stats.leastCollisions = chainLength;
            }
        }
    }

    return stats;
}
