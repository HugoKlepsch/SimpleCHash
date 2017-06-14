#include <stdio.h>
#include <hash.h>
#include <stdlib.h>

void freeFn(char * key, void * data) {
    /* in our case key is not alloc'd */
    free(data);
}

int main(int argc, char ** argv) {
    HashTable * table = initTable(400, hashStr);

    int * temp;

    temp = malloc(sizeof(*temp));
    *temp = 20;
    insert_hash(table, "asdf", temp);

    temp = malloc(sizeof(*temp));
    *temp = 21;
    insert_hash(table, "fdsa", temp);

    temp = malloc(sizeof(*temp));
    *temp = 22;
    insert_hash(table, "asdf", temp);

    temp = malloc(sizeof(*temp));
    *temp = 23;
    insert_hash(table, "one two three four", temp);


    temp = get_hash(table, "asdf");
    printf("asdf: %d\n", *temp);

    temp = get_hash(table, "fdsa");
    printf("fdsa: %d\n", *temp);

    temp = get_hash(table, "asdf");
    printf("asdf: %d\n", *temp);

    temp = get_hash(table, "one two three four");
    printf("one two three four: %d\n", *temp);

    destroyTable(table, freeFn);

    return 0;
}
