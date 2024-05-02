#include "Utilities.h"

/* Array utilities */

/* Used to init the attractors array for bifurcation and lyapunov to NAN, to differentiate them from 0.00 which can be a real attractor */
void init_row(float * array, unsigned int columns)
{
	for (size_t j = 0; j < columns; j++)
	{
		array[j] = 0;
	}
}

void init_matrix(float matrix[][dimension], unsigned int numberRows, unsigned int dimension) {
	for (size_t i = 0; i < numberRows; i++) {
		for (size_t j = 0; j < dimension; j++) {
			matrix[i][j] = 0;
		}
	}
}

/* Check if a value exists */
bool is_in_row(float value, float * array, unsigned int size)
{
	for (size_t i = 0; i < size; i++)
	{
		if (array[i] == value)
			return true;
	}
	return false;
}
/* Check if a value exists */
bool is_in_matrix(float value[], float array[][dimension], unsigned int size)
{
	for (size_t i = 0; i < size; i++)
	{
		if (array[i][0] == value[0] && array[i][1] == value[1])
			return true;
	}
	return false;
}

/* Matrix product, coded so that the result mp can be set as on of the input matrices */
void dot_product(float m1[][dimension], float m2[][dimension], float mp[][dimension]) {
	float temp[dimension][dimension];
	init_matrix(temp, dimension, dimension);

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
