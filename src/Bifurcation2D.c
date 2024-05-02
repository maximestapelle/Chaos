#include "Utilities.h"

/* Bifurcation2D is only for dimension = 2 maps, for now. We could adapt it for 3D and choose the two variables to plot. */
void bifurcation2D() {
	
	FILE * fp;
	float parameterIncrement = (userMapValues.parameterRange[1] - userMapValues.parameterRange[0]) / S;
	/* Redefine maxPoints : we should have a big number of values of each attractor ! */
	maxPoints = 1000;
	float xy[dimension];
	float powerof10; /* for rounding of the trajectory */
	int significantDigits = 4;
	float trajectory[dimension]; /* The current point of the trajectory */
	float attractors[maxPoints][dimension]; /* We need to store, for each value of s, the points to see if the current one is already recorded. */
	unsigned int i;
	bool divergence;

	fp = fopen(dataFile, "w");
		
	
	/* Set the parameter to its minimum value. We subtract parameterIncrement because in the loop, we will add as from s = 0. */
	userMapValues.parameters[0] = userMapValues.parameterRange[0] - parameterIncrement;

	
	switch (userMap) {
		case 6:
			for (size_t s = 0; s <= S; s++) {
				userMapValues.parameters[0] += parameterIncrement;
				for (size_t d = 0; d < dimension; d++) {
					trajectory[d] = userMapValues.IC[d];
				}
				init_matrix(attractors, maxPoints, dimension);
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
						powerof10 = pow(10, significantDigits - ceil(log10f(fabsf(trajectory[0]))));
						xy[0] = round(trajectory[0] * powerof10) / powerof10;
						// Round y to significantDigits significant digits
						powerof10 = pow(10, significantDigits - ceil(log10f(fabsf(trajectory[1]))));
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
				for (size_t d = 0; d < dimension; d++) {
					trajectory[d] = userMapValues.IC[d];
				}
				init_matrix(attractors, maxPoints, dimension);
				i = 0;
				divergence = false;
				/* First we do the minimum number of iterations */
				for (size_t n = 1; n <= minIterations; n++) {
					hénon(trajectory);
				}
				for (size_t n = minIterations + 1; n <= N; n++) {
					/* We check the last point of the trajectory */
					if (!isnan(trajectory[0]) && !isinf(trajectory[0]) && !isnan(trajectory[1]) && !isinf(trajectory[1])) {
						// Round x to significantDigits significant digits
						powerof10 = pow(10, significantDigits - ceil(log10f(fabsf(trajectory[0]))));
						xy[0] = round(trajectory[0] * powerof10) / powerof10;
						// Round y to significantDigits significant digits
						powerof10 = pow(10, significantDigits - ceil(log10f(fabsf(trajectory[1]))));
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
					hénon(trajectory);
				}
				if (!divergence) {
					for (size_t j = 0; j < i; j++) {
						fprintf(fp, "%f %f %f\n", attractors[j][1], attractors[j][0], userMapValues.parameters[0]);
					}
				}
			}
			break;
	}

	printf("Data file created: '%s'.\n", dataFile);
	fclose(fp);
}
