#include "Chaos.h"

/*
 * The attractor function will compute the trajectory of the selected dynamical system,
 * and store it in a file. For now, it uses Runge-Kutta 4th order for flows.
 *
 * The function is called from ChaosMain. 
 */

void attractor() {
	
	FILE * fp;
	float trajectory[dimension];

	fp = fopen(dataFile, "w");
		
	for (size_t d = 0; d < dimension; d++) {
		trajectory[d] = userMapValues.IC[d];
		fprintf(fp, "%.4f", trajectory[d]);
		if (d < dimension - 1) fprintf(fp, " ");
	}
	fprintf(fp, "\n");
	
	switch (userMap) {
		case 6:
			for (size_t n = 1; n <= N; n++) {
				tinkerbell(trajectory);
				for (size_t d = 0; d < dimension; d++) {
					fprintf(fp, "%.4f", trajectory[d]);
					if (d < dimension - 1) fprintf(fp, " ");
				}
				fprintf(fp, "\n");
			}
			break;
		case 7:
			for (size_t n = 1; n <= N; n++) {
				henon(trajectory);
				for (size_t d = 0; d < dimension; d++) {
					fprintf(fp, "%.4f", trajectory[d]);
					if (d < dimension - 1) fprintf(fp, " ");
				}
				fprintf(fp, "\n");
			}
			break;
		case 8: {
// 			float dt = 0.01F;
			float h = 0.02F;
			for (size_t n = 1; n <= N; n++) {
				lorenzRK4(trajectory, h);
				for (size_t d = 0; d < dimension; d++) {
					fprintf(fp, "%.4f", trajectory[d]);
					if (d < dimension - 1) fprintf(fp, " ");
				}
				fprintf(fp, "\n");
			}
			break;
		}
		case 9: {
// 			float dt = 0.01F;
			float h = 0.02F;
			for (size_t n = 1; n <= N; n++) {
				rosslerRK4(trajectory, h);
				for (size_t d = 0; d < dimension; d++) {
					fprintf(fp, "%.4f", trajectory[d]);
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
