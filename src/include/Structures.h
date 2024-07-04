#ifndef STRUCTURES_H
#define STRUCTURES_H

/*
    This header defines data structures that are necessary when we need a "dynamic" size Array
    for performance reasons (eg computation of Capacity Dimension).

    All this makes use of the uthash library.

 */

#include "Chaos.h"
#include "uthash.h"

/*
    Structures
                */
typedef struct {                /*  3D bin  */
    short unsigned int i;
    short unsigned int j;
    short unsigned int k;
} bin3Key;
typedef struct {                /*  A structure containing the 3D bin and its corresponding hash value  */
    bin3Key key;
    UT_hash_handle hh;
} bin3;

/*
    Functions
                */
void addToHash3D(bin3 **all,
                 unsigned int keyLength,
                 bin3Key *index);
void deleteHash3D(bin3 **all);



#endif //STRUCTURES_H
