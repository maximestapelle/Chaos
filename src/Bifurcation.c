#include "Utilities.h"

/*
 * The bifurcation function computes a trajectory and waits for a transient period. After that :
 * - for maps, it records all different points of the trajectory (considered as attractors);
 * - for flows, it records n points of the n-cycles encountered.
 * For now, it uses Runge-Kutta 4th order for flows. Also, it records only the first component
 * of the trajectory.
 *
 * The function is called from ChaosMain. 
 */

void bifurcation() {
	
	FILE * fp;
	float parameterIncrement = (userMapValues.parameterRange[1] - userMapValues.parameterRange[0]) / S;
	float x;
	float powerof10; /* for rounding of the trajectory */
	int significantDigits = 4;
	float trajectory[dimension]; /* The current point of the trajectory */
	float attractors[maxPoints]; /* We need to store, for each value of s, all encountered attractors. */
	unsigned int i;
	bool divergence;

	fp = fopen(dataFile, "w");
		
	
	/* Set the parameter to its minimum value. We subtract parameterIncrement because in the loop, we will add as from s = 0. */
	userMapValues.parameters[0] = userMapValues.parameterRange[0] - parameterIncrement;

	
	switch (userMap) {
		case 1:
			for (size_t s = 0; s <= S; s++) {
				userMapValues.parameters[0] += parameterIncrement;
				for (size_t d = 0; d < dimension; d++) {
					trajectory[d] = userMapValues.IC[d];
				}
				memset(attractors, 0, sizeof attractors); // Initialize to 0 to be sure
				i = 0;
				divergence = false;
				/* First we do the minimum number of iterations */
				for (size_t n = 1; n <= minIterations; n++) {
					logistic(trajectory);
				}
				for (size_t n = minIterations + 1; n <= N; n++) {
					/* We check the last point of the trajectory */
					if (!isnan(trajectory[0]) && !isinf(trajectory[0])) {
						// Round x to significantDigits significant digits
						powerof10 = pow(10, significantDigits - ceil(log10f(fabsf(trajectory[0]))));
						x = round(trajectory[0] * powerof10) / powerof10;
						/* check if it already exists */
						if (is_in_row(x, attractors, maxPoints)) break;
						attractors[i] = x;
						i++;
					}
					else { /* If we record a divergence we just skip this value of s */
						divergence = true;
						break;
					}
					logistic(trajectory);
				}
				if (!divergence) {
					for (size_t j = 0; j < i; j++) {
						fprintf(fp, "%f %f\n", userMapValues.parameters[0], attractors[j]);
					}
				}
			}
			break;
		case 2:
			for (size_t s = 0; s <= S; s++) {
				userMapValues.parameters[0] += parameterIncrement;
				for (size_t d = 0; d < dimension; d++) {
					trajectory[d] = userMapValues.IC[d];
				}
				memset(attractors, 0, sizeof attractors); // Initialize to 0 to be sure
				i = 0;
				divergence = false;
				/* First we do the minimum number of iterations */
				for (size_t n = 1; n <= minIterations; n++) {
					logisticExp(trajectory);
				}
				for (size_t n = minIterations + 1; n <= N; n++) {
					/* We check the last point of the trajectory */
					if (!isnan(trajectory[0]) && !isinf(trajectory[0])) {
						// Round x to significantDigits significant digits
						powerof10 = pow(10, significantDigits - ceil(log10f(fabsf(trajectory[0]))));
						x = round(trajectory[0] * powerof10) / powerof10;
						/* check if it already exists */
						if (is_in_row(x, attractors, maxPoints)) break;
						attractors[i] = x;
						i++;
					}
					else { /* If we record a divergence we just skip this value of s */
						divergence = true;
						break;
					}
					logisticExp(trajectory);
				}
				if (!divergence) {
					for (size_t j = 0; j < i; j++) {
						fprintf(fp, "%f %f\n", userMapValues.parameters[0], attractors[j]);
					}
				}
			}
			break;
		case 3:
			for (size_t s = 0; s <= S; s++) {
				userMapValues.parameters[0] += parameterIncrement;
				for (size_t d = 0; d < dimension; d++) {
					trajectory[d] = userMapValues.IC[d];
				}
				memset(attractors, 0, sizeof attractors); // Initialize to 0 to be sure
				i = 0;
				divergence = false;
				/* First we do the minimum number of iterations */
				for (size_t n = 1; n <= minIterations; n++) {
					gauss(trajectory);
				}
				for (size_t n = minIterations + 1; n <= N; n++) {
					/* We check the last point of the trajectory */
					if (!isnan(trajectory[0]) && !isinf(trajectory[0])) {
						// Round x to significantDigits significant digits
							powerof10 = pow(10, significantDigits - ceil(log10f(fabsf(trajectory[0]))));
							x = round(trajectory[0] * powerof10) / powerof10;
						/* check if it already exists */
						if (is_in_row(x, attractors, maxPoints)) break;
						attractors[i] = x;
						i++;
					}
					else { /* If we record a divergence we just skip this value of s */
						divergence = true;
						break;
					}
					gauss(trajectory);
				}
				if (!divergence) {
					for (size_t j = 0; j < i; j++) {
						fprintf(fp, "%f %f\n", userMapValues.parameters[0], attractors[j]);
					}
				}
			}
			break;
		case 4:
			for (size_t s = 0; s <= S; s++) {
				userMapValues.parameters[0] += parameterIncrement;
				for (size_t d = 0; d < dimension; d++) {
					trajectory[d] = userMapValues.IC[d];
				}
				memset(attractors, 0, sizeof attractors); // Initialize to 0 to be sure
				i = 0;
				divergence = false;
				/* First we do the minimum number of iterations */
				for (size_t n = 1; n <= minIterations; n++) {
					tent(trajectory);
				}
				for (size_t n = minIterations + 1; n <= N; n++) {
					/* We check the last point of the trajectory */
					if (!isnan(trajectory[0]) && !isinf(trajectory[0])) {
						// Round x to significantDigits significant digits
						powerof10 = pow(10, significantDigits - ceil(log10f(fabsf(trajectory[0]))));
						x = round(trajectory[0] * powerof10) / powerof10;
						/* check if it already exists */
						if (is_in_row(x, attractors, maxPoints)) break;
						attractors[i] = x;
						i++;
					}
					else { /* If we record a divergence we just skip this value of s */
						divergence = true;
						break;
					}
					tent(trajectory);
				}
				if (!divergence) {
					for (size_t j = 0; j < i; j++) {
						fprintf(fp, "%f %f\n", userMapValues.parameters[0], attractors[j]);
					}
				}
			}
			break;
		case 5:
			for (size_t s = 0; s <= S; s++) {
				userMapValues.parameters[0] += parameterIncrement;
				for (size_t d = 0; d < dimension; d++) {
					trajectory[d] = userMapValues.IC[d];
				}
				memset(attractors, 0, sizeof attractors); // Initialize to 0 to be sure
				i = 0;
				divergence = false;
				/* First we do the minimum number of iterations */
				for (size_t n = 1; n <= minIterations; n++) {
					circle(trajectory);
				}
				for (size_t n = minIterations + 1; n <= N; n++) {
					/* We check the last point of the trajectory */
					if (!isnan(trajectory[0]) && !isinf(trajectory[0])) {
						// Round x to significantDigits significant digits
						powerof10 = pow(10, significantDigits - ceil(log10f(fabsf(trajectory[0]))));
						x = round(trajectory[0] * powerof10) / powerof10;
						/* check if it already exists */
						if (is_in_row(x, attractors, maxPoints)) break;
						attractors[i] = x;
						i++;
					}
					else { /* If we record a divergence we just skip this value of s */
						divergence = true;
						break;
					}
					circle(trajectory);
				}
				if (!divergence) {
					for (size_t j = 0; j < i; j++) {
						fprintf(fp, "%f %f\n", userMapValues.parameters[0], attractors[j]);
					}
				}
			}
			break;
		case 6:
			for (size_t s = 0; s <= S; s++) {
				userMapValues.parameters[0] += parameterIncrement;
				for (size_t d = 0; d < dimension; d++) {
					trajectory[d] = userMapValues.IC[d];
				}
				memset(attractors, 0, sizeof attractors); // Initialize to 0 to be sure
				i = 0;
				divergence = false;
				/* First we do the minimum number of iterations */
				for (size_t n = 1; n <= minIterations; n++) {
					tinkerbell(trajectory);
				}
				for (size_t n = minIterations + 1; n <= N; n++) {
					/* We check the last point of the trajectory */
					if (!isnan(trajectory[0]) && !isinf(trajectory[0])) {
						// Round x to significantDigits significant digits
						powerof10 = pow(10, significantDigits - ceil(log10f(fabsf(trajectory[0]))));
						x = round(trajectory[0] * powerof10) / powerof10;
						/* check if it already exists */
						if (is_in_row(x, attractors, maxPoints)) break;
						attractors[i] = x;
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
						fprintf(fp, "%f %f\n", userMapValues.parameters[0], attractors[j]);
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
				memset(attractors, 0, sizeof attractors); // Initialize to 0 to be sure
				i = 0;
				divergence = false;
				/* First we do the minimum number of iterations */
				for (size_t n = 1; n <= minIterations; n++) {
					henon(trajectory);
				}
				for (size_t n = minIterations + 1; n <= N; n++) {
					/* We check the last point of the trajectory */
					if (!isnan(trajectory[0]) && !isinf(trajectory[0])) {
						// Round x to significantDigits significant digits
						powerof10 = pow(10, significantDigits - ceil(log10f(fabsf(trajectory[0]))));
						x = round(trajectory[0] * powerof10) / powerof10;
						/* check if it already exists */
						if (is_in_row(x, attractors, maxPoints)) break;
						attractors[i] = x;
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
						fprintf(fp, "%f %f\n", userMapValues.parameters[0], attractors[j]);
					}
				}
			}
			break;
/* The situation is completely different if there exist cycles. A stable period-3 orbit is not chaotic, for example. To make the difference between chaos (infinity of different cycles) and not chaos (eg 3 orbits in the rossler map when b = 0.040) we will create a array of size three (nothing to do with my example of 3 cycle orbit, but we need three numbers to see if we have a local maximum) which will be a "window" to check when we get to a local maximum, and register the maximum. For eg a 3 orbit, there will be three different maxima. */
		case 8: {
			float h = 0.02F;
			float window[3];
			size_t j;
			significantDigits = 5;
			minIterations = 10000;
			for (size_t s = 0; s <= S; s++) {
				userMapValues.parameters[0] += parameterIncrement;
				for (size_t d = 0; d < dimension; d++) {
					trajectory[d] = userMapValues.IC[d];
				}
				memset(attractors, 0, sizeof attractors); // Initialize to 0 to be sure
				i = 0;
				j = 0;
				divergence = false;
				/* First we do the minimum number of iterations */
				for (size_t n = 1; n <= minIterations; n++) {
					lorenzRK4(trajectory, h);
					/* Start to populate the window at the last five */
					if (n > minIterations - 3) {
						window[i % 5] = trajectory[0];
						i++;
					}
				}
				for (size_t n = minIterations + 1; n <= N; n++) {
					lorenzRK4(trajectory, h);
					if (!isnan(trajectory[0]) && !isinf(trajectory[0])) {
						window[i % 3] = trajectory[0];
						/* Check if just after a local maximum */
						if (window[i % 3] < window[(i - 1) % 3] && window[(i - 1) % 3] >= window[(i - 2) % 3]) {
							/* window[(i - 1) % 3] is a local MAXIMUM. We check if the value already exists, and if not, we store it */
							// Round it to significantDigits significant digits
							powerof10 = pow(10, significantDigits - ceil(log10f(fabsf(window[(i - 1) % 3]))));
							x = round(window[(i - 1) % 3] * powerof10) / powerof10;
							if (is_in_row(x, attractors, maxPoints)) break;
							else {
								attractors[j] = x;
								j++;
								if (j >= maxPoints) break;
							}
						}
						i++;
					}
					else { /* If we record a divergence we just skip this value of s */
						divergence = true;
						break;
					}
				}
				if (!divergence) {
					for (size_t k = 0; k < j; k++) {
						fprintf(fp, "%f %f\n", userMapValues.parameters[0], attractors[k]);
					}
				}
			}
			break;
		}
		case 9: {
// 			float dt = 0.01F;
			float h = 0.02F;
			float window[3];
			size_t j;
			significantDigits = 5;
			minIterations = 15000;
			for (size_t s = 0; s <= S; s++) {
				userMapValues.parameters[0] += parameterIncrement;
				for (size_t d = 0; d < dimension; d++) {
					trajectory[d] = userMapValues.IC[d];
				}
				memset(attractors, 0, sizeof attractors); // Initialize to 0 to be sure
				i = 0;
				j = 0;
				divergence = false;
				/* First we do the minimum number of iterations */
				for (size_t n = 1; n <= minIterations; n++) {
// 					rossler(trajectory, dt);
					rosslerRK4(trajectory, h);
					/* Start to populate the window at the last five */
					if (n > minIterations - 3) {
						window[i % 5] = trajectory[0];
						i++;
					}
				}
				for (size_t n = minIterations + 1; n <= N; n++) {
// 					rossler(trajectory, dt);
					rosslerRK4(trajectory, h);
					if (!isnan(trajectory[0]) && !isinf(trajectory[0])) {
						window[i % 3] = trajectory[0];
						/* Check if just after a local maximum */
						if (window[i % 3] < window[(i - 1) % 3] && window[(i - 1) % 3] >= window[(i - 2) % 3]) {
							/* window[(i - 1) % 3] is a local MAXIMUM. We check if the value already exists, and if not, we store it */
							// Round it to significantDigits significant digits
							powerof10 = pow(10, significantDigits - ceil(log10f(fabsf(window[(i - 1) % 3]))));
							x = round(window[(i - 1) % 3] * powerof10) / powerof10;
							if (is_in_row(x, attractors, maxPoints)) break;
							else {
								attractors[j] = x;
								j++;
								if (j >= maxPoints) break;
							}
						}
						i++;
					}
					else { /* If we record a divergence we just skip this value of s */
						divergence = true;
						break;
					}
				}
				if (!divergence) {
					for (size_t k = 0; k < j; k++) {
						fprintf(fp, "%f %f\n", userMapValues.parameters[0], attractors[k]);
					}
				}
			}
			break;
		}
	}

	printf("Data file created: '%s'.\n", dataFile);
	fclose(fp);
}
