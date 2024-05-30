#ifndef UTILITIES_H
#define UTILITIES_H

/*
 * This header is used by utilities functions.
 */

#include "Chaos.h"
#include <complex.h>


double **init_2DMatrix(const unsigned int nRows, const unsigned int nColumns);
void free_2DMatrix(double **matrix, const unsigned int nRows);
bool is_in_row(double value, double *array, const unsigned int size);
bool is_in_matrix(double *value, double **matrix, const unsigned int size);
void dot_product(double m1[][dimension], double m2[][dimension], double mp[][dimension]);
void matrix_transpose(double m[][dimension], double mt[][dimension]);
void eigen_values(double matrix[][dimension], double eigenvalues[]);

#endif
