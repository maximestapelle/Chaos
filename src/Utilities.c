#include "Utilities.h"

/*
	The Utilities c file contains function that I considered to be "utilities" :
	array and matrices, linear algebra, check if a value exists in an array...

	Its functions are called from action fuctions : Attracto, Bifurcation, Lyapunov
 */

/*	File utilities  */
void fillFile(FILE *fp, float *parameter, double *array) {
/*
	Attractor and Lyapunov will call a function to print inf the data file.
	It contains a performance upgrade thanks to an explicit switch on the dimension.
																						*/
	if (!parameter) {
		switch (dimension) {
			case 2:
				fprintf(fp, "%.4lf %.4lf\n", array[0], array[1]);
				break;
			case 3:
				fprintf(fp, "%.4lf %.4lf %.4lf\n", array[0], array[1], array[2]);
				break;
		}
	}
	else {
		switch (dimension) {
			case 2:
				fprintf(fp, "%f %.4lf %.4lf\n", *parameter, array[0], array[1]);
				break;
			case 3:
				fprintf(fp, "%f %.4lf %.4lf %.4lf\n", *parameter, array[0], array[1], array[2]);
				break;
		}
	}

}

/*	Array utilities */
double **init_2DMatrix(const unsigned int nRows, const unsigned int nColumns) {

	double **matrix = calloc(nRows, sizeof *matrix);
	for (size_t i = 0; i < nRows; i++) {
		matrix[i] = calloc(nColumns, sizeof **matrix);
	}

	return matrix;
}
void free_2DMatrix(double **matrix, const unsigned int nRows) {

	for (size_t i = 0; i < nRows; i++) {
	free(matrix[i]);
	}
	free(matrix);


}

/*	Check if a value exists, for 2D actions */
bool is_in_matrix(double *value, double **matrix, const unsigned int size) {

	for (size_t i = 0; i < size; i++)
	{
		if (matrix[i][0] == value[0] && matrix[i][1] == value[1])
			return true;
	}
	return false;
}

void dot_product(const double *m1,
				 const double *m2,
	  			 double		  *mp,
				 const size_t dimension)
{
	size_t matrixDimension = dimension * dimension;
	double temp[matrixDimension];
	for (size_t d = 0; d < matrixDimension; d++) {
		temp[d] = 0;
	}

	for (size_t i = 0; i < dimension; i++) {
		for (size_t j = 0; j < dimension; j++) {
			for (size_t k = 0; k < dimension; k++) {
				temp[i * dimension + j] += m1[i * dimension + k] * m2[k * dimension + j];
			}
		}
	}
	/* Copy temp to m2. */
	memcpy(mp, temp, sizeof (temp));
}

/* Matrix product, coded such that the first matrix is the jacobian part of
   the extended state position+jacobian */
void dot_product_state(const double *state,
				 	   const double *m2,
	  				   double  	    *mp,
				 	   const size_t dimension)
{
	size_t matrixDimension = dimension * dimension;
	double temp[matrixDimension];
	for (size_t d = 0; d < matrixDimension; d++) {
		temp[d] = 0;
	}

	for (size_t i = 0; i < dimension; i++) {
		for (size_t j = 0; j < dimension; j++) {
			for (size_t k = 0; k < dimension; k++) {
				temp[i * dimension + j] += state[dimension + i * dimension + k] * m2[k * dimension + j];
			}
		}
	}
	/* Copy temp to m2. */
	memcpy(mp, temp, sizeof (temp));
}
/* Square Matrix transpose */
void matrix_transpose(const double *m,
					  double	   *mt,
					  const size_t dimension)
{
	for (size_t i = 0; i < dimension; i++) {
		for (size_t j = 0; j < dimension; j++) {
			mt[j * dimension + i] = m[i * dimension + j];
		}
	}
}

/*	(OLD) Specific to dimension = 2 !! */
// void eigen_values(double matrix[][dimension], double eigenvalues[]) {
//
// 	double temp1 = matrix[0][0] + matrix[1][1];
// 	double temp2 = pow(temp1, 2) - 4 * (matrix[0][0] * matrix[1][1] - matrix[1][0] * matrix[0][1]);
//
// 	if (temp2 >= 0) {
// 		eigenvalues[0] = (temp1 + sqrtf(temp2)) / 2;
// 		eigenvalues[1] = (temp1 - sqrtf(temp2)) / 2;
// 	}
// 	else {
// 		eigenvalues[0] = cabs(temp1 + csqrtf(temp2)) / 2;
// 		eigenvalues[1] = cabs(temp1 - csqrtf(temp2)) / 2;
// 	}
//
// }

double norm(const double *vector) {

	double square = 0.0;

	for (size_t i = 0; i < dimension; i++) {
		square += pow(vector[i], 2);
	}

	return sqrt(square);
}
double normColumn(const double *vector,
				  const int column) {

	/* Input is a dimension*dimension vector representing a matrix */
	double square = 0.0;

	for (size_t i = 0; i < dimension; i++) {
		square += pow(vector[i * dimension + column], 2);
	}

	return sqrt(square);
}
double normOfDifference(const double *vector1,
						const double *vector2) {

	double square = 0.0;

	for (size_t i = 0; i < dimension; i++) {
		square += pow(vector1[i] - vector2[i], 2);
	}

	return sqrt(square);
}
unsigned int computeStateDimension(const unsigned int dimension) {

	return dimension + pow(dimension, 2);
}
