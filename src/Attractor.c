#include "Dynamics.h"

/*
	The attractor function will compute the trajectory of the selected dynamical system,
	and store it in a file. For now, it uses Runge-Kutta 4th order for flows.

	The function is called from ChaosMain.
 */

void attractor() {

	FILE *fp = fopen(dataFile, "w");
	double trajectory[dimension];

	/*	Initial condition  */
	for (size_t d = 0; d < dimension; d++) {
		trajectory[d] = userMapValues.IC[d];
		fprintf(fp, "%.4f", trajectory[d]);
		if (d < dimension - 1) fprintf(fp, " ");
	}
	fprintf(fp, "\n");

	/*	Main process  */
	switch (userMap) {
		case 6:
			for (size_t n = 1; n <= N; n++) {
				tinkerbell(trajectory);
				fprintf(fp, "%.4f ",  trajectory[0]);
				fprintf(fp, "%.4f\n", trajectory[1]);
			}
			break;
		case 7:
			for (size_t n = 1; n <= N; n++) {
				henon(trajectory);
				fprintf(fp, "%.4f ",  trajectory[0]);
				fprintf(fp, "%.4f\n", trajectory[1]);
			}
			break;
		case 8: {
// 			double dt = 0.01F;	In case of Euler
			float h = 0.02F;
			for (size_t n = 1; n <= N; n++) {
				lorenzRK4(trajectory, h);
				fprintf(fp, "%.4f ",  trajectory[0]);
				fprintf(fp, "%.4f ",  trajectory[1]);
				fprintf(fp, "%.4f\n", trajectory[2]);
			}
			break;
		}
		case 9: {
// 			double dt = 0.01F;	In case of Euler
			float h = 0.02F;
			for (size_t n = 1; n <= N; n++) {
				rosslerRK4(trajectory, h);
				fprintf(fp, "%.4f ",  trajectory[0]);
				fprintf(fp, "%.4f ",  trajectory[1]);
				fprintf(fp, "%.4f\n", trajectory[2]);
			}
			break;
		}
	}

	printf("Data file created: '%s'.\n", dataFile);
	fclose(fp);
}
