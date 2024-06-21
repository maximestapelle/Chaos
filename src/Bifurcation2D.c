#include "Utilities.h"
#include "Dynamics.h"

/*	UNDER CONSTRUCTION
 *	******************
	The bifurcation2D function computes a trajectory and waits for a transient period. After that :
	- for maps, it records all different points of the trajectory (considered as attractors);
	- for flows, it records n points of the n-cycles encountered.
	For now, it uses Runge-Kutta 4th order for flows. Also, it records only the two first components
	of the trajectory. For simplicity, it does that only for 2D maps.

	The function is called from ChaosMain.
 */

void bifurcation2D(unsigned int NMin) {

	FILE *fp = fopen(dataFile, "w");

	const float parameterIncrement = (userMapValues.parameterRange[1] - userMapValues.parameterRange[0]) / S;

	maxPoints = 1000;				/* Redefine maxPoints : we should have a big number
									   of values of each attractor ! */
	double xy[dimension];
	double powerof10; 											/* For rounding of the state */
	int significantDigits = 4;

	double state[dimension]; 									/* The current point of the state */
	double **attractors = init_2DMatrix(maxPoints, dimension);  /* We need to store, for each value of s,
																   all encountered attractors. */
	size_t i;													/* Attractors counter */
	bool divergence;

	/*  Set the parameter to its minimum value.
		We subtract parameterIncrement because in the loop, we will add as from s = 0.  */
	userMapValues.parameters[0] = userMapValues.parameterRange[0] - parameterIncrement;

	switch (isDiscrete) {
		case 1:
			/*
				Procedure for maps
									 */
			for (size_t s = 0; s <= S; s++) {
				userMapValues.parameters[0] += parameterIncrement;
				for (size_t d = 0; d < dimension; d++) {
					state[d] = (double) userMapValues.IC[d];
				}
				i = 0;
				divergence = false;
				/* First we do the minimum number of iterations */
				for (size_t n = 1; n <= NMin; n++) {
					dynamics(state);
				}
				for (size_t n = NMin + 1; n <= N; n++) {
					/* We check the last point of the state */
					if (!isnan(state[0]) && !isinf(state[0]) && !isnan(state[1]) && !isinf(state[1])) {
						// Round x to significantDigits significant digits
						powerof10 = pow(10, significantDigits - ceil(log10f(fabs(state[0]))));
						xy[0] = round(state[0] * powerof10) / powerof10;
						// Round y to significantDigits significant digits
						powerof10 = pow(10, significantDigits - ceil(log10f(fabs(state[1]))));
						xy[1] = round(state[1] * powerof10) / powerof10;
						/* check if it already exists */
						if (is_in_matrix(xy, attractors, maxPoints)) break;
						attractors[i][0] = xy[0];
						attractors[i][1] = xy[1];
						i++;
					}
					else { /* If we record a divergence we just skip this value of s */
						divergence = true;
						break;
					}
					dynamics(state);
				}
				if (!divergence) {
					for (size_t j = 0; j < i; j++) {
						fprintf(fp, "%lf %lf %f\n", attractors[j][1], attractors[j][0], userMapValues.parameters[0]);
					}
				}
			}
			break;
		default:
			/*
				Procedure for flows : to be implemented.
									 */

			break;
	}

	// Free the attractors matrix
	free_2DMatrix(attractors, maxPoints);

	printf("Data file created: '%s'.\n", dataFile);
	fclose(fp);
}
