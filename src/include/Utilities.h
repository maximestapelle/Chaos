#ifndef UTILITIES_H
#define UTILITIES_H

/*
 * This header is used by utilities functions.
 */

#include "Chaos.h"
#include <complex.h>


float **init_2DMatrix(const unsigned int nRows, const unsigned int nColumns);
void free_2DMatrix(float **matrix, const unsigned int nRows);
bool is_in_row(float value, float *array, const unsigned int size);
bool is_in_matrix(float *value, float **matrix, const unsigned int size);
void dot_product(float m1[][dimension], float m2[][dimension], float mp[][dimension]);
void matrix_transpose(float m[][dimension], float mt[][dimension]);
void eigen_values(float matrix[][dimension], float eigenvalues[]);

#endif
