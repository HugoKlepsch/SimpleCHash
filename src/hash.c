#include <hash.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

unsigned int hashStr(char * string) {
    if (!string) {
        return EXIT_FAILURE;
    }

    unsigned int accumulator = 0;

    /* sum each letter of the string */
    while (*string != '\0') {
        accumulator += *string;
        string++;
    }
    return accumulator;
}

HashTable * initTable(int size, unsigned int (*hashFn)(char * key)) {
    HashTable * table = malloc(sizeof(*table));

    if (!table) {
        return NULL;
    }

    table->table = malloc(sizeof(*table->table) * size);

    if (!table->table) {
        free(table);
        return NULL;
    }

    table->numSlots = size;
    table->hashFn = hashFn;

    int i;
    for (i = 0; i < table->numSlots; ++i) {
        table->table[i] = NULL;
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
    /* That index will not be NULL if we had a collision.
     * We resolve collisions by chaining.
     * The following ensures that elementToInsertAt is linked in */
    if (table->table[index] == NULL) {
        table->table[index] = elementToInsertAt;
    } else {
        Element * t = table->table[index];
        while (t->next != NULL) {
            t = t->next;
        }
        t->next = elementToInsertAt;
    }
}

void * get_hash(HashTable * table, char * key) {
    unsigned int index;

    /* We don't really care if the data itself is NULL because the user might
     * be storing it specifically */
    if (!table || !key) {
        return NULL;
    }

    index = table->hashFn(key) % table->numSlots;

    /* if it's NULL, that means there's no element with that key */
    if (table->table[index] == NULL) {
        return NULL;
    } else {
        Element * t = table->table[index];
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
        Element * temp = freeElement(table->table[i], freeFn);

        while (temp != NULL) {
            temp = freeElement(temp, freeFn);
        }
    }

    free(table->table);
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
        Element * t = table->table[i];

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
