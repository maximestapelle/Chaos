#include "Utilities.h"
#include "Dynamics.h"

/*
	The bifurcation function computes a trajectory and waits for a transient period. After that :
	- for maps, it records all different points of the trajectory (considered as attractors);
	- for flows, it records n points of the n-cycles encountered.
	It records only the first component
	of the trajectory.

	The function is called from ChaosMain.
 */

void bifurcation(unsigned int NMin) {

	FILE *fp = fopen(dataFile, "w");
	double tolerance = 1E-4;
	const float parameterIncrement = (userMapValues.parameterRange[1] - userMapValues.parameterRange[0]) / S;
	double x;
	double state[dimension]; 		/*	The current point of the state */
	double attractors[maxPoints]; 	/*	We need to store, for each value of s, all encountered attractors. */
	size_t i;						/*	Attractors counter */
	bool divergence;

	/*  Set the parameter to its minimum value.
		We subtract parameterIncrement because in the loop, we will add as from s = 0.  */
	userMapValues.parameters[0] = userMapValues.parameterRange[0] - parameterIncrement;

	switch (isDiscrete) {
		case 1:
		/*
		    Procedure for maps
							    */
			for (size_t s = 0; s < S; s++) {
				userMapValues.parameters[0] += parameterIncrement;
				for (size_t d = 0; d < dimension; d++) {
					state[d] = (double) userMapValues.IC[d];
				}
				i = 0;
				divergence = false;
				/*	First we do the minimum number of iterations */
				for (size_t n = 1; n <= NMin; n++) {
					dynamics(state);
				}
				/*	We store x to see if we return to it after */
				if (!isnan(state[0]) && !isinf(state[0])) {
					x = state[0];
					attractors[i] = x;
					i++;
				}
				else continue;
				for (size_t n = NMin + 1; n <= N; n++) {
					dynamics(state);
					/*	We check the last point of the state */
					if (!isnan(state[0]) && !isinf(state[0])) {
						/*	Check if we are back to x */
						if (fabs(x - state[0]) < tolerance) break;
						attractors[i] = state[0];
						i++;
						if (i == maxPoints) break;
					}
					else {	/*	If we record a divergence we just skip this value of s */
						divergence = true;
						break;
					}
				}
				if (!divergence) {
					for (size_t j = 0; j < i; j++) {
						fprintf(fp, "%f %lf\n", userMapValues.parameters[0], attractors[j]);
					}
				}
			}
			break;
		case 0: {
		/*
		    Procedure for flows
							     */
		/*  The situation is completely different. A stable period-3 orbit is not chaotic, for example.
		    To make the difference between chaos (infinity of different cycles) and not chaos (eg 3 orbits in the
		    Rössler map when b = 0.040) we will create a array of size three (nothing to do with my example of 3
		    cycle orbit, but we need three numbers to see if we have a local maximum) which will be a "window" to
		    check when we get to a local maximum, and register the maximum. For eg a 3 orbit, there will be three
		    different maxima.
		    				   */
			const size_t windowSize = 3;
			const size_t varPlot = 0;			/* All checks are on the first variable */
			double window[windowSize];
			size_t j;
			tolerance /= 10;
			/*	Loop on values of the bifurcation parameter */
			for (size_t s = 0; s <= S; s++) {
				userMapValues.parameters[0] += parameterIncrement;
				for (size_t d = 0; d < dimension; d++) {
					state[d] = (double) userMapValues.IC[d];
				}
				i = 0;
				j = 0;
				/*	First we do the minimum number of iterations */
				for (size_t n = 1; n <= NMin; n++) {
					dynamics(state);
					/*	Start to populate the window at the last windowSize iterations */
					if (n > NMin - windowSize) {
						window[j % windowSize] = state[varPlot];
						j++;
					}
				}
				/*	First, check if we have a fixed point */
				if (fabs(window[j % windowSize] - window[(j - 1) % windowSize]) < tolerance) {
					attractors[i] = state[varPlot];
					i++;
				}
				else {
					/*	No fixed point -> check cycles : are we just after a local maximum */
					for (size_t n = NMin + 1; n <= N; n++) {
						dynamics(state);
						window[j % windowSize] = state[varPlot];
						if ((window[j % windowSize] < window[(j - 1) % windowSize])
						 && (window[(j - 1) % windowSize] >= window[(j - 2) % windowSize]) ) {
							/*	window[(j - 1) % 3] is a local MAXIMUM.
							   	We check if the value already exists, and if not, we store it */
							if (fabs(window[(j - 1) % windowSize] - attractors[0]) < tolerance) break;
							else {
								attractors[i] = window[(j - 1) % windowSize];
								i++;
								if (i == maxPoints) break;
							}
						}
						j++;
					}
				}
				for (size_t k = 0; k < i; k++) {
					fprintf(fp, "%f %lf\n", userMapValues.parameters[0], attractors[k]);
				}
			}
			break;
		}
	}

	printf("Data file created: '%s'.\n", dataFile);
	fclose(fp);
}
