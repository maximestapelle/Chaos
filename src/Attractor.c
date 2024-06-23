#include "Dynamics.h"

/*
	The attractor function will compute the state of the selected dynamical system,
	and store it in a file.

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


	/*	EXACTLY THE SAME for flows and maps.  */
	for (size_t n = 1; n <= N; n++) {
		dynamics(state);
		for (size_t d = 0; d < dimension; d++) {
			fprintf(fp, "%.4lf", state[d]);
			if (d < dimension - 1) fprintf(fp, " ");
		}
		fprintf(fp, "\n");
	}

	printf("Data file created: '%s'.\n", dataFile);
	fclose(fp);
}
