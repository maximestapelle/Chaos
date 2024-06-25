#include "Structures.h"

/*
    Functions on data structures that are necessary when we need a "dynamic" size Array
    for performance reasons (eg computation of Capacity Dimension).

    All this makes use of the uthash library.

 */

void addToHash3D(bin3 **all,
                 unsigned int keyLength,
                 bin3Key *index) {

    bin3 *s = NULL;

    HASH_FIND(hh, *all, &index->i, keyLength, s);
    if (s == NULL) {
        s = malloc(sizeof (*s));
        s->key.i = index->i;
        s->key.j = index->j;
        s->key.k = index->k;
        HASH_ADD(hh, *all, key.i, keyLength, s);
    }
}
void deleteHash3D(bin3 **all) {

    bin3 *current, *tmp;

    HASH_ITER(hh, *all, current, tmp) {
        HASH_DEL(*all, current);         /* delete it (all advances to next) */
        free(current);                  /* free it */
    }
}
