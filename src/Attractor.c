#include "Utilities.h"
#include "Structures.h"
#include "Dynamics.h"

/*
	The attractor function will compute the state of the selected dynamical system,
	and store it in a file.

	The computeCapacityDimension will compute an estimation of the capacity dimension.

	The meaning of NMin (from the database) here is the number of iterations needed to consider
	we are passed the transient. It is used for the capacity dimension.

	The function is called from ChaosMain.
 */

static float computeCapacityDimension(double **trajectory, size_t NPoints, double range[][2]);

void attractor(unsigned int NMin) {

	FILE *fp = fopen(dataFile, "w");
	double state[dimension];

	for (size_t d = 0; d < dimension; d++) {
		state[d] = (double) userMapValues.IC[d];
		fprintf(fp, "%.4lf", state[d]);
		if (d < dimension - 1) fprintf(fp, " ");
	}
	fprintf(fp, "\n");

	/*
		EXACTLY THE SAME for flows and maps.
												*/
	/*	Does the user wants an estimation of the Capacity Dimension ?	*/
	switch(requestCapacityDimension) {
		case 0:
			for (size_t n = 1; n <= N; n++) {
				dynamics(state);
				for (size_t d = 0; d < dimension; d++) {
					fprintf(fp, "%.4lf", state[d]);
					if (d < dimension - 1) fprintf(fp, " ");
				}
				fprintf(fp, "\n");
			}
			printf("Data file created: '%s'.\n", dataFile);
			break;
		case 1: {
			double **trajectory = init_2DMatrix(dimension, N - NMin);	/*	We need the whole trajectory  */
			double interval[dimension][2];								/*	Min and max of each variable in the
			 																trajectory  */
			/*  Initialise range  */
			for (size_t d = 0; d < dimension; d++) {
				interval[d][0] = 0.0;
				interval[d][1] = 0.0;
			}
			/*	First, the Transient  */
			for (size_t n = 1; n <= NMin; n++) {
				dynamics(state);
				for (size_t d = 0; d < dimension; d++) {
					fprintf(fp, "%.4lf", state[d]);
					if (d < dimension - 1) fprintf(fp, " ");
				}
				fprintf(fp, "\n");
			}
			/*	We now consider we're on the attractor  */
			for (size_t n = NMin + 1; n <= N; n++) {
				dynamics(state);
				for (size_t d = 0; d < dimension; d++) {
					/*	Fill trajectory  */
					trajectory[d][n - (NMin + 1)] = state[d];
					/*	Update min and max */
					if (state[d] < interval[d][0])
						interval[d][0] = state[d];
					if (state[d] > interval[d][1])
						interval[d][1] = state[d];
					/*	Write in file  */
					fprintf(fp, "%.4lf", state[d]);
					if (d < dimension - 1) fprintf(fp, " ");
				}
				fprintf(fp, "\n");
			}
			printf("Data file created: '%s'.\n", dataFile);
			/*	Capacity dimension  */
			dCap = computeCapacityDimension(trajectory, N - NMin, interval);
			printf(ANSI_COLOR_MAGENTA "The estimated Capacity Dimension is %.2f." ANSI_COLOR_RESET, dCap);
			printf("\n\n");
			/*	Free the mallocated trajectory  */
			free_2DMatrix(trajectory, dimension);
			break;
		}
	}

	fclose(fp);
}

/*	The function takes the trajectory, considered as a strange attractor, and implements a naive
	box counting to compute the slope of log(N(epsilon)) wrt log(1/epsilon). It identifies the
	interesting region by moving a window through the points, taking 1/2 of them and taking the
	slope where the output of least squares fitting is best.
																	*/
static float computeCapacityDimension(double **trajectory, size_t NPoints, double interval[][2]) {

	float dCap = 0.0F;
	/*	Box sizes  */
	/*	Here, epsilon is actually the log10 of epsilon in the theory. This is to easily have a constant
		step between the different values, in a log scale.  */

	/*	The biggest value of epsilon will such that the box just covers the whole attractor.
		So we set it to the maximum size of coordinate interval	*/
	float epsilonMaxTmp = interval[0][1] - interval[0][0];
	float epsilonMax = epsilonMaxTmp;
	for (size_t d = 1; d < dimension; d++) {
		epsilonMaxTmp = interval[d][1] - interval[d][0];
		if (epsilonMaxTmp > epsilonMax)
			epsilonMax = epsilonMaxTmp;
	}
	epsilonMax = log10(epsilonMax);
	/*	We set the minimum value to a factor of 10^-3.5	*/
	float addToMax;
	if (dimension == 2)
		addToMax = -4.0F;
	else
		addToMax = -3.0F;
	const float epsilonMin = epsilonMax + addToMax;

	const unsigned int numberEpsilon = 20;
	float epsilon;
	size_t range[dimension];
	/*  Array with log(N(epsilon)) versus log(1/espilon)  */
	double toFit[numberEpsilon][2];
	size_t N = 0;

	FILE *fp = fopen("CapDim.dat", "w");

	for (size_t e = 0; e < numberEpsilon; e++) {	/*	No need to fo to epsilonMax included  */
		epsilon = epsilonMin + e * (epsilonMax - epsilonMin) / numberEpsilon;
		toFit[e][0] = pow(10, -epsilon);		/*	The 1/\epsilon of the theory  */
		/* Compute ranges
			Import note : if the range is already integer before taking the ceiling,
			the ceiling is equal to floor and we'll reach an extra index of the bin matrix.
			So, add 1 to the ceiling just to be sure. */
		for (size_t d = 0; d < dimension; d++) {
			range[d] = ceil((interval[d][1] - interval[d][0]) * toFit[e][0]) + 1;
		}
		switch (dimension) {
			case 2: {
				/*
					In d = 2, the "naive" way to create a matrix of bins and fill it
					if there is at least one point in there works well.
																			*/
				bool **binMatrix = malloc(range[0] * sizeof (*binMatrix));
				for (size_t i = 0; i < range[0]; i++) {
					binMatrix[i] = calloc(range[1], sizeof (**binMatrix));
				};
				size_t i, j;
				/* Fill bin matrix from trajectory */
				for (size_t n = 0; n < NPoints; n++) {
					i = floor((trajectory[0][n] - interval[0][0]) * toFit[e][0]);
					j = floor((trajectory[1][n] - interval[1][0]) * toFit[e][0]);
					if (binMatrix[i][j])
						continue;
					else
						binMatrix[i][j] = 1;
				}
				N = 0;
				/* Compute capacity dimension */
				for (size_t i = 0; i < range[0]; i++) {
					for (size_t j = 0; j < range[1]; j++) {
						if (binMatrix[i][j]) N++;
					}
				}
				free(binMatrix);
				break;
			}
			case 3: {
				/*
					In d = 3, the "naive" way to create a matrix of bins and fill it
					if there is at least one point in there is unreasonable because
					if requires GBs or RAM. So instead we'll record the populated bins
					in a linked list in which the total number of elements is kept
					up to date, which will be our N(epsilon).
																			*/
				list3D *binMatrix = allocate3D();
				size_t i, j, k;
				/* Fill bin matrix from trajectory */
				for (size_t n = 0; n < NPoints; n++) {
					i = floor((trajectory[0][n] - interval[0][0]) * toFit[e][0]);
					j = floor((trajectory[1][n] - interval[1][0]) * toFit[e][0]);
					k = floor((trajectory[2][n] - interval[2][0]) * toFit[e][0]);
					addTo3DList(binMatrix, i, j, k);
				}
				N = binMatrix->count;
				delete3DList(binMatrix);
				break;
			}
		}
		fprintf(fp, "%lf %lu\n", toFit[e][0], N);
		toFit[e][0] = log10(toFit[e][0]);		/* Take log(1/epsilon)  */
		toFit[e][1] = log10(N);
	}

	fclose(fp);
	/*	We'll define a Window of 3/5 of the points that will slide through the numberEpsilon points.
			Up to now, this number of points is completely arbitrary and just seems to yield
			good results with this method.
		For each of those windows we do a least squares fit and output the slope and value of
			the sum of squares.  */
	unsigned int windowSize = floor(numberEpsilon * 3 / 5);
	float dCap_current;
	double leastSquares_old = fit_least_squares(toFit, 0, windowSize - 1, &dCap_current);
	double leastSquares_current = leastSquares_old;
	dCap = dCap_current;
	for (size_t n = 1; n <= numberEpsilon - windowSize; n++) {
		leastSquares_current = fit_least_squares(toFit, n, (windowSize - 1) + n, &dCap_current);
		if (leastSquares_current < leastSquares_old) {
			dCap = dCap_current;
			leastSquares_old = leastSquares_current;
		}
	}

	return dCap;
}
