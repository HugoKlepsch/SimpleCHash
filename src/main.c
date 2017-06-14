#include <stdio.h>
#include <hash.h>
#include <stdlib.h>
#include <string.h>

void noFree(char * key, void * data) {
    /* In our case neither key nor data is allocated and therefore doesn't
     * need to be free'd */
}

void freeKey(char * key, void * data) {
    free(key);
}

int main(int argc, char ** argv) {
    HashTable * table = initTable(400, hashStr);
    TableStats stats;

    if (argc == 1) {
        char * keys[] = {"Jupiter", "Saturn", "Uranus", "October", "Dominick"};
        char * values[] = {
            "Is the largest planet in our solar system.",
            "Is a planet with rings.",
            "Is a blue planet.",
            "Is the 10th month of the Gregorian callendar.",
            "Is going to read this code"};

        int i;
        for (i = 0; i < 5; ++i) {
            insert_hash(table, keys[i], values[i]);
        }

        for (i = 0; i < 5; ++i) {
            printf("%s: %s\n", keys[i], (char*)get_hash(table, keys[i]));
        }

        stats = getTableStats(table);
        destroyTable(table, noFree);

    } else {
        char * filename = argv[1];
        FILE * fp = fopen(filename, "r");
        if (!fp) {
            puts("Could not open file");
            return 1;
        }

        char line[100];

        while(fgets(line, 100, fp)){
            char * word = malloc(100);
            memcpy(word, line, 100);
            insert_hash(table, word, NULL);
        }

        fclose(fp);

        stats = getTableStats(table);
        destroyTable(table, freeKey);
    }

    printf("numSlots:\t%d\nnumElements:\t%d\nnumCollisions:\t%d\nmostCollisions:\t%d\nleastCollisions:\t%d\n",\
            stats.numSlots, stats.numElements, stats.numCollisions,\
            stats.mostCollisions, stats.leastCollisions);

    return 0;
}
