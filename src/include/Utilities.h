#ifndef UTILITIES_H
#define UTILITIES_H

/*
	This header declares utilities functions, used by various actions.
 */

#include "Chaos.h"
/*	Include complex numbers for eigen values	*/
// #include <complex.h>


double **init_2DMatrix(
		const unsigned int nRows,
		const unsigned int nColumns);
void free_2DMatrix(
		double **matrix,
		const unsigned int nRows);

bool is_in_matrix(
		double *value,
		double **matrix,
		const unsigned int size);

void dot_product(const double *m1,
				 const double *m2,
	  			 double		  *mp,
				 const size_t dimension);
void dot_product_state(const double *state,
				 	   const double *m2,
	  				   double  	    *mp,
				 	   const size_t dimension);
void matrix_transpose(const double *m,
					  double	   *mt,
					  const size_t dimension);
// void eigen_values(
// 		double matrix[][dimension],
// 		double eigenvalues[]);

double norm(const double *vector);
double normColumn(
		const double *vector,
		const int column);
double normOfDifference(
		const double *vector1,
		const double *vector2);

unsigned int computeStateDimension(const unsigned int dimension);

double fit_least_squares(const double toFit[][2],
						 const size_t start,
						 const size_t end,
						 float *slope);


#endif
