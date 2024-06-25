#ifndef STRUCTURES_H
#define STRUCTURES_H

/*
    This header defines data structures that are necessary when we need a "dynamic" size Array
    for performance reasons (eg computation of Capacity Dimension).

 */

#include "Chaos.h"

/*
    Structures
                */
//  Structures that contain indexes of a 2 or 3 dimensional matrix
struct bin2D {
	short int i;
	short int j;
	struct bin2D *next;
};
struct bin3D {
	short int i;
	short int j;
    short int k;
	struct bin3D *next;
};
//  A python-like dictionary to store the latter
typedef struct {
	struct bin2D *head;
	struct bin2D *tail;
	size_t count;
} list2D;
typedef struct {
	struct bin3D *head;
	struct bin3D *tail;
	size_t count;
} list3D;

/*
    Functions
                */
list2D *allocate2D();
list3D *allocate3D();
void    delete2DList(list2D *list);
void    delete3DList(list3D *list);
void    addTo2DList( list2D *list, short int i, short int j);
void    addTo3DList( list3D *list, short int i, short int j, short int k);




#endif //STRUCTURES_H
