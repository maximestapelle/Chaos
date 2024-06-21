#include "Dynamics.h"

/*
	The attractor function will compute the state of the selected dynamical system,
	and store it in a file. For now, it uses Runge-Kutta 4th order for flows.

	The function is called from ChaosMain.
 */

void attractor() {

	FILE *fp = fopen(dataFile, "w");
	double state[dimension];

	for (size_t d = 0; d < dimension; d++) {
		state[d] = (double) userMapValues.IC[d];
		fprintf(fp, "%.4lf", state[d]);
		if (d < dimension - 1) fprintf(fp, " ");
	}
	fprintf(fp, "\n");


	/*	EXACTLY THE SAME for flows and maps, but waiting for the whole pointers stuff to be implemented.	*/
	switch (isDiscrete) {
		case 1:
		/*
		    Procedure for maps
							    */
			for (size_t n = 1; n <= N; n++) {
				dynamics(state);
				for (size_t d = 0; d < dimension; d++) {
					fprintf(fp, "%.4lf", state[d]);
					if (d < dimension - 1) fprintf(fp, " ");
				}
				fprintf(fp, "\n");
			}
			break;
		case 0: {
			/*
			    Procedure for flows
								    */
			for (size_t n = 1; n <= N; n++) {
				dynamics(state);
				for (size_t d = 0; d < dimension; d++) {
					fprintf(fp, "%.4lf", state[d]);
					if (d < dimension - 1) fprintf(fp, " ");
				}
				fprintf(fp, "\n");
			}
			break;
		}
	}

	printf("Data file created: '%s'.\n", dataFile);
	fclose(fp);
}
