#include "Utilities.h"
#include "Structures.h"
#include "Dynamics.h"

/*
	The attractor function will compute the state of the selected dynamical system,
	and store it in a file.

	computeCapacityDimension() will compute an estimation of the capacity dimension.

	The meaning of NMin (from the database) here is the number of iterations needed to consider
	we are passed the transient. It is used for the capacity dimension.

	The function is called from ChaosMain.
 */

static float computeCapacityDimension(double **trajectory,
									  const	size_t NPoints,
									  const	double interval[][2]);

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

		Switch for the choice of the user to request Capacity dimension.
																			*/
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
			fclose(fp);
			break;
		case 1: {
		/*
			Capacity dimension -> we need the whole trajectory (after the transient)
																						*/
			double **trajectory = init_2DMatrix(dimension, N - NMin);

			double interval[dimension][2];			/*	Min and Max of each variable in the trajectory  */
			/*  Initialise Min and Max  */
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
					/*	Update Min and Max */
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
			fclose(fp);

			/*	Capacity dimension  */
			dCap = computeCapacityDimension(trajectory, N - NMin, interval);
			free_2DMatrix(trajectory, dimension);			/*	Free the mallocated trajectory  */

			printf(ANSI_COLOR_MAGENTA "The estimated Capacity Dimension is %.2f." ANSI_COLOR_RESET, dCap);
			printf("\n\n");

			break;
		}
	}

}

static float computeCapacityDimension(double **trajectory,
									  const size_t NPoints,
									  const double interval[][2]) {
/*	The function takes the trajectory, considered as a strange attractor, and implements a naive
  	box counting to compute the slope of log(N(epsilon)) wrt log(1/epsilon). It identifies the
  	interesting region by moving a window through the points, taking 1/2 of them and taking the
  	slope where the output of least squares fitting is best.
 */

	float dCap = 0.0F;

	/*
		Box sizes
					*/
	/*	- Here, epsilon is actually the log10 of epsilon in the theory. This is to easily have a constant
		  step between the different values, in a log scale.
		- The biggest value of epsilon will such that the box just covers the whole attractor.
		  So we set it to the maximum size of coordinate interval
		  																	*/
	float epsilonMax = interval[0][1] - interval[0][0];
	float epsilonMaxTmp;
	for (size_t d = 1; d < dimension; d++) {
		epsilonMaxTmp = interval[d][1] - interval[d][0];
		if (epsilonMaxTmp > epsilonMax)
			epsilonMax = epsilonMaxTmp;
	}
	epsilonMax = log10(epsilonMax);
	/*	The minimum value of epsilon will depend on the dimension.
			This is a fine-tuning of what I've remarked worked best.	*/
	float addToMax;
	if (dimension == 2)
		addToMax = -4.0F;
	else
		addToMax = -3.0F;
	const float epsilonMin = epsilonMax + addToMax;

	const unsigned int numberEpsilon = 20;
	float epsilon;

	size_t N = 0;						/*	N(epsilon)  */
	double toFit[numberEpsilon][2];		/*  Array with log(N(epsilon)) versus log(1/espilon)  */

/*
	PART I
			Fill the toFit array
 */
	for (size_t e = 0; e < numberEpsilon; e++) {	/*	No need to go to epsilonMax included (N(epsilonMax) = 1)  */
		epsilon = epsilonMin + e * (epsilonMax - epsilonMin) / numberEpsilon;
		toFit[e][0] = pow(10, -epsilon);
		switch (dimension) {
			case 2: {
				/*
					In d = 2, the "naive" way to create a matrix of bins and fill it
					if there is at least one point in there works well.
					The number of rows and columns are stored in the variable "range".
																						*/
				size_t range[dimension];
				/* 	Compute ranges
						Import note : if the range is already integer before taking the ceiling,
						the ceiling is equal to floor and we'll reach an extra index of the bin matrix.
						So, add 1 to the ceiling just to be sure. */
				for (size_t d = 0; d < dimension; d++) {
					range[d] = ceil((interval[d][1] - interval[d][0]) * toFit[e][0]) + 1;
				}

				bool **binMatrix = malloc(range[0] * sizeof (*binMatrix));
				for (size_t i = 0; i < range[0]; i++) {
					/*	Default state of each bin is "not filled" -> false -> calloc  */
					binMatrix[i] = calloc(range[1], sizeof (**binMatrix));
				}
				size_t i, j;
				/*
					Fill bin matrix from trajectory
					Compute capacity Dimension
													*/
				N = 0;
				for (size_t n = 0; n < NPoints; n++) {
					i = floor((trajectory[0][n] - interval[0][0]) * toFit[e][0]);
					j = floor((trajectory[1][n] - interval[1][0]) * toFit[e][0]);
					if (binMatrix[i][j])
						continue;
					else {
						binMatrix[i][j] = 1;
						N++;
					}
				}
				free(binMatrix);
				break;
			}
			case 3: {
			/*
				In d = 3, the "naive" way to create a matrix of bins and fill it
				if there is at least one point in there is unreasonable because
				if requires GBs of RAM.
				Instead, record the bins that are populated in a HASH table
				with utstash.h. This is crazy fast :-)
																		*/
				unsigned int keyLength = sizeof (bin3Key);
				bin3 *binMatrix = NULL;
				bin3Key index;
				/*	Fill bin matrix from trajectory  */
				for (size_t n = 0; n < NPoints; n++) {
					index.i = floor((trajectory[0][n] - interval[0][0]) * toFit[e][0]);
					index.j = floor((trajectory[1][n] - interval[1][0]) * toFit[e][0]);
					index.k = floor((trajectory[2][n] - interval[2][0]) * toFit[e][0]);
					addToHash3D(&binMatrix, keyLength, &index);
				}
				N = HASH_COUNT(binMatrix);
				deleteHash3D(&binMatrix);
				break;
			}
		}

		toFit[e][0] = log10(toFit[e][0]);		/* Take log(1/epsilon)  */
		toFit[e][1] = log10(N);
	}

/*
	PART II
			Compute the best dCap with a least Squares fit on successive subsets of ToFit:

			We'll define a Window of 3/5 of the points that will slide through the numberEpsilon points.
				Up to now, this number of points is completely arbitrary and just seems to yield
				good results with this method.
			For each of those windows we do a least squares fit and output the slope and value of
				the sum of squares.
 */
	unsigned int windowSize = floor(numberEpsilon * 3 / 5);
	double leastSquares = fit_least_squares(toFit, 0, windowSize - 1, &dCap);
	double leastSquaresTmp = leastSquares;
	float dCapTmp = dCap;
	for (size_t n = 1; n <= numberEpsilon - windowSize; n++) {
		leastSquaresTmp = fit_least_squares(toFit, n, (windowSize - 1) + n, &dCapTmp);
		if (leastSquaresTmp < leastSquares) {
			dCap = dCapTmp;
			leastSquares = leastSquaresTmp;
		}
	}

	return dCap;
}
