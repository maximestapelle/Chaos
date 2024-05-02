#ifndef UTILITIES_H
#define UTILITIES_H

/*
 * This header is used by utilities function.
 */

#include "Chaos.h"
#include <complex.h>


void init_row(float * array, unsigned int columns);
void init_matrix(float matrix[][dimension], unsigned int numberRows, unsigned int dimension);
bool is_in_row(float value, float * array, unsigned int size);
bool is_in_matrix(float value[], float array[][dimension], unsigned int size);
void dot_product(float m1[][dimension], float m2[][dimension], float mp[][dimension]);
void matrix_transpose(float m[][dimension], float mt[][dimension]);
void eigen_values(float matrix[][dimension], float eigenvalues[]);

#endif
