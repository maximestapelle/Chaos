#include "Structures.h"

/*
    Functions on data structures that are necessary when we need a "dynamic" size Array
    for performance reasons (eg computation of Capacity Dimension).

 */

/*
    Allocate lists
                    */
 list2D *allocate2D() {

 	list2D *list = malloc(sizeof (*list));

 	list->head  = NULL;
 	list->tail  = NULL;
 	list->count = 0;

 	return list;
 }
 list3D *allocate3D() {

    list3D *list = malloc(sizeof (*list));

    list->head  = NULL;
    list->tail  = NULL;
    list->count = 0;

    return list;
 }
/*
    Free lists
                */
void delete2DList(list2D *list) {

	struct bin2D *current, *next;
	current = list->head;
	while (current) {
		next = current->next;
		free(current);
		current=next;
	}
	free((void *) list);
}
void delete3DList(list3D *list) {

	struct bin3D *current, *next;
	current = list->head;
	while (current) {
		next = current->next;
		free(current);
		current=next;
	}
	free((void *) list);
}
/*
    Populate lists
                    */
void addTo2DList(list2D *list, short int i, short int j) {

	struct bin2D *current = list->head;

	if (current == NULL) {										/*	Empty list!  */
		list->head = malloc(sizeof (*(list->head)));			/*	Allocated head, tail will be equal to head  */
		list->head->i = i;
		list->head->j = j;
		list->head->next = NULL;
		list->tail = list->head;
		list->count++;
	}
    /*  For now, we just parse the whole list.
        If performance is an issue, add a hashing function.  */
	while (current) {
		if (!((current->i == i) && (current->j == j))) {
			if (current->next)
				current = current->next;
			else {	/*	We're on the tail  */
				current = malloc(sizeof (*current));
				list->tail->next = current;
				current->next = NULL;
				list->tail = current;
				current->i = i;
				current->j = j;
				list->count++;
			}
		}
		else  /*  If the entry already exists, we do nothing -> exit the while loop   */
			break;
	}
}
void addTo3DList(list3D *list, short int i, short int j, short int k) {

	struct bin3D *current = list->head;

	if (current == NULL) {										/*	Empty list!  */
		list->head = malloc(sizeof (*(list->head)));			/*	Allocated head, tail will be equal to head  */
		list->head->i = i;
		list->head->j = j;
        list->head->k = k;
		list->head->next = NULL;
		list->tail = list->head;
		list->count++;
	}
    /*  For now, we just parse the whole list.
        If performance is an issue, add a hashing function.  */
	while (current) {
		if (!((current->i == i) && (current->j == j) && (current->k == k))) {
			if (current->next)
				current = current->next;
			else {	/*	We're on the tail  */
				current = malloc(sizeof (*current));
				list->tail->next = current;
				current->next = NULL;
				list->tail = current;
				current->i = i;
				current->j = j;
                current->k = k;
				list->count++;
			}
		}
		else  /*  If the entry already exists, we do nothing -> exit the while loop   */
			break;
	}
}
