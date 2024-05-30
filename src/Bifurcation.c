#include "Utilities.h"
#include "Dynamics.h"

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
				i = 0;
				divergence = false;
				/* First we do the minimum number of iterations */
				for (size_t n = 1; n <= minIterations; n++) {
					logistic(trajectory);
				}
				/* We store x to see if we return to it after */
				if (!isnan(trajectory[0]) && !isinf(trajectory[0])) {
					x = trajectory[0];
					attractors[i] = x;
					i++;
				}
				else continue;
				for (size_t n = minIterations + 1; n <= N; n++) {
					logistic(trajectory);
					/* We check the last point of the trajectory */
					if (!isnan(trajectory[0]) && !isinf(trajectory[0])) {
						/* check if we are back to x */
						if (fabsf(x - trajectory[0]) < 1E-3) break;
						attractors[i] = trajectory[0];
						i++;
					}
					else { /* If we record a divergence we just skip this value of s */
						divergence = true;
						break;
					}
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
				i = 0;
				divergence = false;
				/* First we do the minimum number of iterations */
				for (size_t n = 1; n <= minIterations; n++) {
					logisticExp(trajectory);
				}
				/* We store x to see if we return to it after */
				if (!isnan(trajectory[0]) && !isinf(trajectory[0])) {
					x = trajectory[0];
					attractors[i] = x;
					i++;
				}
				else continue;
				for (size_t n = minIterations + 1; n <= N; n++) {
					logisticExp(trajectory);
					/* We check the last point of the trajectory */
					if (!isnan(trajectory[0]) && !isinf(trajectory[0])) {
						/* check if we are back to x */
						if (fabsf(x - trajectory[0]) < 1E-3) break;
						attractors[i] = trajectory[0];
						i++;
					}
					else { /* If we record a divergence we just skip this value of s */
						divergence = true;
						break;
					}
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
				i = 0;
				divergence = false;
				/* First we do the minimum number of iterations */
				for (size_t n = 1; n <= minIterations; n++) {
					gauss(trajectory);
				}
				/* We store x to see if we return to it after */
				if (!isnan(trajectory[0]) && !isinf(trajectory[0])) {
					x = trajectory[0];
					attractors[i] = x;
					i++;
				}
				else continue;
				for (size_t n = minIterations + 1; n <= N; n++) {
					gauss(trajectory);
					/* We check the last point of the trajectory */
					if (!isnan(trajectory[0]) && !isinf(trajectory[0])) {
						/* check if we are back to x */
						if (fabsf(x - trajectory[0]) < 1E-4) break;
						attractors[i] = trajectory[0];
						i++;
					}
					else { /* If we record a divergence we just skip this value of s */
						divergence = true;
						break;
					}
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
				i = 0;
				divergence = false;
				/* First we do the minimum number of iterations */
				for (size_t n = 1; n <= minIterations; n++) {
					tent(trajectory);
				}
				/* We store x to see if we return to it after */
				if (!isnan(trajectory[0]) && !isinf(trajectory[0])) {
					x = trajectory[0];
					attractors[i] = x;
					i++;
				}
				else continue;
				for (size_t n = minIterations + 1; n <= N; n++) {
					tent(trajectory);
					/* We check the last point of the trajectory */
					if (!isnan(trajectory[0]) && !isinf(trajectory[0])) {
						/* check if we are back to x */
						if (fabsf(x - trajectory[0]) < 1E-3) break;
						attractors[i] = trajectory[0];
						i++;
					}
					else { /* If we record a divergence we just skip this value of s */
						divergence = true;
						break;
					}
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
				i = 0;
				divergence = false;
				/* First we do the minimum number of iterations */
				for (size_t n = 1; n <= minIterations; n++) {
					circle(trajectory);
				}
				/* We store x to see if we return to it after */
				if (!isnan(trajectory[0]) && !isinf(trajectory[0])) {
					x = trajectory[0];
					attractors[i] = x;
					i++;
				}
				else continue;
				for (size_t n = minIterations + 1; n <= N; n++) {
					circle(trajectory);
					/* We check the last point of the trajectory */
					if (!isnan(trajectory[0]) && !isinf(trajectory[0])) {
						/* check if we are back to x */
						if (fabsf(x - trajectory[0]) < 1E-4) break;
						attractors[i] = trajectory[0];
						i++;
					}
					else { /* If we record a divergence we just skip this value of s */
						divergence = true;
						break;
					}
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
				i = 0;
				divergence = false;
				/* First we do the minimum number of iterations */
				for (size_t n = 1; n <= minIterations; n++) {
					tinkerbell(trajectory);
				}
				/* We store x to see if we return to it after */
				if (!isnan(trajectory[0]) && !isinf(trajectory[0])) {
					x = trajectory[0];
					attractors[i] = x;
					i++;
				}
				else continue;
				for (size_t n = minIterations + 1; n <= N; n++) {
					tinkerbell(trajectory);
					/* We check the last point of the trajectory */
					if (!isnan(trajectory[0]) && !isinf(trajectory[0])) {
						/* check if we are back to x */
						if (fabsf(x - trajectory[0]) < 1E-3) break;
						attractors[i] = trajectory[0];
						i++;
					}
					else { /* If we record a divergence we just skip this value of s */
						divergence = true;
						break;
					}
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
				i = 0;
				divergence = false;
				/* First we do the minimum number of iterations */
				for (size_t n = 1; n <= minIterations; n++) {
					henon(trajectory);
				}
				/* We store x to see if we return to it after */
				if (!isnan(trajectory[0]) && !isinf(trajectory[0])) {
					x = trajectory[0];
					attractors[i] = x;
					i++;
				}
				else continue;
				for (size_t n = minIterations + 1; n <= N; n++) {
					henon(trajectory);
					/* We check the last point of the trajectory */
					if (!isnan(trajectory[0]) && !isinf(trajectory[0])) {
						/* check if we are back to x */
						if (fabsf(x - trajectory[0]) < 1E-3) break;
						attractors[i] = trajectory[0];
						i++;
					}
					else { /* If we record a divergence we just skip this value of s */
						divergence = true;
						break;
					}
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
			const float h = 0.001F;
			const unsigned int windowSize = 3;
			const unsigned int varPlot = 0;
			float window[windowSize];
			size_t j;
			minIterations = 100000;
			for (size_t s = 0; s <= S; s++) {
				userMapValues.parameters[0] += parameterIncrement;
				for (size_t d = 0; d < dimension; d++) {
					trajectory[d] = userMapValues.IC[d];
				}
				i = 0;
				j = 0;
				/* First we do the minimum number of iterations */
				for (size_t n = 1; n <= minIterations; n++) {
					lorenzRK4(trajectory, h);
					/* Start to populate the window at the last five */
					if (n > minIterations - windowSize) {
						window[j % windowSize] = trajectory[varPlot];
						j++;
					}
				}
				/* First, check if we have a fixed point */
				lorenzRK4(trajectory, h);
				window[j % windowSize] = trajectory[varPlot];
				j++;
				if (fabsf(trajectory[varPlot] - window[(j - 2) % windowSize]) < 1E-4) {
					attractors[i] = trajectory[varPlot];
					i++;
				}
				else {
					/* No fixed point -> check cycles : are we just after a local maximum */
					for (size_t n = minIterations + 1; n <= N; n++) {
						lorenzRK4(trajectory, h);
						window[j % windowSize] = trajectory[varPlot];
						if ((window[j % windowSize] < window[(j - 1) % windowSize]) && (window[(j - 1) % windowSize] >= window[(j - 2) % windowSize])) {
							/* window[(j - 1) % 3] is a local MAXIMUM. We check if the value already exists, and if not, we store it */
							if (fabsf(window[(j - 1) % windowSize] - attractors[0]) < 1E-5) break;
							else {
								attractors[i] = window[(j - 1) % windowSize];
								i++;
								if (i >= maxPoints) break;
							}
						}
						j++;
					}
				}
				for (size_t k = 0; k < i; k++) {
					fprintf(fp, "%f %f\n", userMapValues.parameters[0], attractors[k]);
				}
			}
			break;
		}
		case 9: {
			const float h = 0.01F;
			const unsigned int windowSize = 3;
			const int varPlot = 0;
			float window[windowSize];
			size_t j;
			minIterations = 50000;
			for (size_t s = 0; s <= S; s++) {
				userMapValues.parameters[0] += parameterIncrement;
				for (size_t d = 0; d < dimension; d++) {
					trajectory[d] = userMapValues.IC[d];
				}
				i = 0;
				j = 0;
				/* First we do the minimum number of iterations */
				for (size_t n = 1; n <= minIterations; n++) {
					rosslerRK4(trajectory, h);
					/* Start to populate the window at the last five */
					if (n > minIterations - windowSize) {
						window[j % windowSize] = trajectory[varPlot];
						j++;
					}
				}
				/* First, check if we have a fixed point */
				rosslerRK4(trajectory, h);
				window[j % windowSize] = trajectory[varPlot];
				j++;
				if (fabsf(trajectory[varPlot] - window[(j - 2) % windowSize]) < 1E-5) {
					attractors[i] = trajectory[varPlot];
					i++;
				}
				else {
					/* No fixed point -> check cycles */
					for (size_t n = minIterations + 1; n <= N; n++) {
						rosslerRK4(trajectory, h);
						window[j % windowSize] = trajectory[varPlot];
						if (window[j % windowSize] < window[(j - 1) % windowSize] && window[(j - 1) % windowSize] >= window[(j - 2) % windowSize]) {
							/* window[(j - 1) % 3] is a local EXTREMUM. We check if the value already exists, and if not, we store it */
							if (fabsf(window[(j - 1) % windowSize] - attractors[0]) < 1E-4) break;
							else {
								attractors[i] = window[(j - 1) % 3];
								i++;
								if (i >= maxPoints) break;
							}
						}
						j++;
					}
				}
				for (size_t k = 0; k < i; k++) {
					fprintf(fp, "%f %f\n", userMapValues.parameters[0], attractors[k]);
				}
			}
			break;
		}
	}

	printf("Data file created: '%s'.\n", dataFile);
	fclose(fp);
}
