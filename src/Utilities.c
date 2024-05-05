#include "Utilities.h"

/*
 * The Utilities c file contains function that I considered to be "utilities" :
 * array and matrices, linear algebra, check if a value exists in an array...
 *
 * Its functions are called from physics fuctions : Attracto, Bifurcation, Lyapunov
 */

/* Array utilities */
float **init_2DMatrix(const unsigned int nRows, const unsigned int nColumns)
{
	float **matrix = calloc(nRows, sizeof *matrix); 
	for (size_t i = 0; i < nRows; i++) {
		matrix[i] = calloc(nColumns, sizeof **matrix);
	}
	
	return matrix;
}
void free_2DMatrix(float **matrix, const unsigned int nRows)
{
	for (size_t i = 0; i < nRows; i++) {
	free(matrix[i]);
	}
	free(matrix);


}

/* Check if a value exists */
bool is_in_row(float value, float *array, const unsigned int size)
{
	for (size_t i = 0; i < size; i++)
	{
		if (array[i] == value)
			return true;
	}
	return false;
}
/* Check if a value exists, for 2D actions */
bool is_in_matrix(float *value, float **matrix, const unsigned int size)
{
	for (size_t i = 0; i < size; i++)
	{
		if (matrix[i][0] == value[0] && matrix[i][1] == value[1])
			return true;
	}
	return false;
}

/* Matrix product, coded so that the result mp can be set as one of the input matrices */
void dot_product(float m1[][dimension], float m2[][dimension], float mp[][dimension]) {
	float temp[dimension][dimension];
	memset(temp, 0, sizeof temp);

	for (size_t i = 0; i < dimension; i++) {
		for (size_t j = 0; j < dimension; j++) {
			for (size_t k = 0; k < dimension; k++) {
				temp[i][j] += m1[i][k] * m2[k][j];
			}
		}
	}
	/* Copy temp to m2. */
	memcpy(mp, temp, sizeof(temp));
}
/* Square Matrix transpose */
void matrix_transpose(float m[][dimension], float mt[][dimension]) {
	for (size_t i = 0; i < dimension; i++) {
		for (size_t j = 0; j < dimension; j++) {
			mt[j][i] = m[i][j];
		}
	}
}

/* Specific to dimension = 2 !! */
void eigen_values(float matrix[][dimension], float eigenvalues[]) {
	float temp1 = matrix[0][0] + matrix[1][1];
	float temp2 = pow(temp1, 2) - 4 * (matrix[0][0] * matrix[1][1] - matrix[1][0] * matrix[0][1]);
	
	if (temp2 >= 0) {
		eigenvalues[0] = (temp1 + sqrtf(temp2)) / 2;
		eigenvalues[1] = (temp1 - sqrtf(temp2)) / 2;
	}
	else {
		eigenvalues[0] = cabsf(temp1 + csqrtf(temp2)) / 2;
		eigenvalues[1] = cabsf(temp1 - csqrtf(temp2)) / 2;
	}

}