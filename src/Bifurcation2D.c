#include "Utilities.h"
#include "Dynamics.h"

/*
 * The bifurcation2D function computes a trajectory and waits for a transient period. After that :
 * - for maps, it records all different points of the trajectory (considered as attractors);
 * - for flows, it records n points of the n-cycles encountered.
 * For now, it uses Runge-Kutta 4th order for flows. Also, it records only the two first components
 * of the trajectory. For simplicity, it does that only for 2D maps.
 *
 * The function is called from ChaosMain.
 */

void bifurcation2D() {

	FILE * fp;
	double parameterIncrement = (userMapValues.parameterRange[1] - userMapValues.parameterRange[0]) / S;
	/* Redefine maxPoints : we should have a big number of values of each attractor ! */
	maxPoints = 1000;
	double xy[dimension];
	double powerof10; /* for rounding of the trajectory */
	int significantDigits = 4;
	double trajectory[dimension]; /* The current point of the trajectory */
	double **attractors = init_2DMatrix(maxPoints, dimension); /* We need to store, for each value of s, the points to see if the current one is already recorded. */

	unsigned int i;
	bool divergence;

	fp = fopen(dataFile, "w");

	/* Set the parameter to its minimum value. We subtract parameterIncrement because in the loop, we will add as from s = 0. */
	userMapValues.parameters[0] = userMapValues.parameterRange[0] - parameterIncrement;

	switch (userMap) {
		case 6:
			for (size_t s = 0; s <= S; s++) {
				userMapValues.parameters[0] += parameterIncrement;
				trajectory[0] = userMapValues.IC[0];
				trajectory[1] = userMapValues.IC[1];
				i = 0;
				divergence = false;
				/* First we do the minimum number of iterations */
				for (size_t n = 1; n <= minIterations; n++) {
					tinkerbell(trajectory);
				}
				for (size_t n = minIterations + 1; n <= N; n++) {
					/* We check the last point of the trajectory */
					if (!isnan(trajectory[0]) && !isinf(trajectory[0]) && !isnan(trajectory[1]) && !isinf(trajectory[1])) {
						// Round x to significantDigits significant digits
						powerof10 = pow(10, significantDigits - ceil(log10f(fabs(trajectory[0]))));
						xy[0] = round(trajectory[0] * powerof10) / powerof10;
						// Round y to significantDigits significant digits
						powerof10 = pow(10, significantDigits - ceil(log10f(fabs(trajectory[1]))));
						xy[1] = round(trajectory[1] * powerof10) / powerof10;
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
					tinkerbell(trajectory);
				}
				if (!divergence) {
					for (size_t j = 0; j < i; j++) {
						fprintf(fp, "%f %f %f\n", attractors[j][1], attractors[j][0], userMapValues.parameters[0]);
					}
				}
			}
			break;
		case 7:
			for (size_t s = 0; s <= S; s++) {
				userMapValues.parameters[0] += parameterIncrement;
				trajectory[0] = userMapValues.IC[0];
				trajectory[1] = userMapValues.IC[1];
				i = 0;
				divergence = false;
				/* First we do the minimum number of iterations */
				for (size_t n = 1; n <= minIterations; n++) {
					henon(trajectory);
				}
				for (size_t n = minIterations + 1; n <= N; n++) {
					/* We check the last point of the trajectory */
					if (!isnan(trajectory[0]) && !isinf(trajectory[0]) && !isnan(trajectory[1]) && !isinf(trajectory[1])) {
						// Round x to significantDigits significant digits
						powerof10 = pow(10, significantDigits - ceil(log10f(fabs(trajectory[0]))));
						xy[0] = round(trajectory[0] * powerof10) / powerof10;
						// Round y to significantDigits significant digits
						powerof10 = pow(10, significantDigits - ceil(log10f(fabs(trajectory[1]))));
						xy[1] = round(trajectory[1] * powerof10) / powerof10;
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
					henon(trajectory);
				}
				if (!divergence) {
					for (size_t j = 0; j < i; j++) {
						fprintf(fp, "%f %f %f\n", attractors[j][1], attractors[j][0], userMapValues.parameters[0]);
					}
				}
			}
			break;
	}

	// Free the attractors matrix
	free_2DMatrix(attractors, maxPoints);

	printf("Data file created: '%s'.\n", dataFile);
	fclose(fp);
}
