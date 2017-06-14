#include <stdio.h>
#include <hash.h>
#include <stdlib.h>

void freeFn(char * key, void * data) {
    /* In our case neither key nor data is allocated and therefore doesn't
     * need to be free'd */
}

int main(int argc, char ** argv) {
    char * keys[] = {"Jupiter", "Saturn", "Uranus", "October", "Dominick"};
    char * values[] = {
        "Is the largest planet in our solar system.",
        "Is a planet with rings.",
        "Is a blue planet.",
        "Is the 10th month of the Gregorian callendar.",
        "Is going to read this code"};

    HashTable * table = initTable(400, hashStr);

    int i;
    for (i = 0; i < 5; ++i) {
        insert_hash(table, keys[i], values[i]);
    }

    for (i = 0; i < 5; ++i) {
        printf("%s: %s\n", keys[i], (char*)get_hash(table, keys[i]));
    }

    destroyTable(table, freeFn);

    return 0;
}
