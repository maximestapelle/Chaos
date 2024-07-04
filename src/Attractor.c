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
	}
	fillFile(fp, NULL, state);

	/*
		EXACTLY THE SAME for flows and maps.

		Switch for the choice of the user to request Capacity dimension.
																				*/
	switch(requestCapacityDimension) {
		case 0:
			for (size_t n = 1; n <= N; n++) {
				dynamics(state);
				fillFile(fp, NULL, state);
			}
			fclose(fp);
			printf("Data file created: '%s'.\n", dataFile);
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
				fillFile(fp, NULL, state);
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
				}
				fillFile(fp, NULL, state);
			}
			fclose(fp);
			printf("Data file created: '%s'.\n", dataFile);

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
  	interesting region by moving a window through the points, taking a subset of them and taking the
  	slope where the output of least squares fitting is best.
 */

	float dCap = 0.0F;

	/*
		Shift and normalize the trajectory (huge performance improvement)
		All points will be in [0, 1]. 1 should be excluded most of the time,
		EXCEPT if the length of an interval is exactly an integer.
																				*/
	size_t intervalLength[dimension];
	for (size_t d = 0; d < dimension; d++) {
		intervalLength[d] = ceil(interval[d][1] - interval[d][0]);
	}
	for (size_t n = 0; n < NPoints; n++) {
		for (size_t d = 0; d < dimension; d++) {
			trajectory[d][n] = (trajectory[d][n] - interval[d][0]) / intervalLength[d];
		}
	}
	/*
		Box sizes
					*/
	/*	- Here, epsilon is actually the log10 of epsilon in the theory. This is to easily have a constant
		  step between the different values, in a log scale.
		- The maximum value of epsilon is 1: the whole attractor is covered.
		  																	*/
	float epsilonMax = 0.0F;
	/*	The minimum value of epsilon will depend on the dimension.
			This is a fine-tuning based on what I've remarked worked best.	*/
	float epsilonMin = - (log10(NPoints) + 1) / 2;
	if (dimension == 3)
		epsilonMin /= 2;

	const unsigned int numberEpsilon = 20;
	float epsilon;

	size_t N = 0;						/*	N(epsilon)  */
	double toFit[numberEpsilon][2];		/*  Array with log(N(epsilon)) versus log(1/epsilon)  */

/*
	PART I
			Fill the toFit array
 */
	for (size_t e = 0; e < numberEpsilon; e++) {	/*	No need to go to epsilonMax included (N(epsilonMax) = 1)  */
		epsilon = epsilonMin + e * (epsilonMax - epsilonMin) / numberEpsilon;
		toFit[e][0] = pow(10, -epsilon);			/*	This is 1/epsilon  */
		switch (dimension) {
			case 2: {
				/*
					In d = 2, the "naive" way to create a matrix of bins and fill it
					if there is at least one point in there works well.
					The number of elements in the matrix is (toFit[e][0] + 1) ^ d. Indeed, at this point
					toFit[e][0] is 1/epsilon, ie approximatly the number of bins needed to cover
					the interval. We take the ceil and add 1 in case it is integer and a point is exactly at 1.
																													*/
				size_t range = ceil(toFit[e][0]) + 1;
				bool **binMatrix = malloc(range * sizeof (*binMatrix));
				for (size_t i = 0; i < range; i++) {
					/*	Default state of each bin is "not filled" -> false -> calloc  */
					binMatrix[i] = calloc(range, sizeof (**binMatrix));
				}

				/*	Fill binMatrix  */
				size_t i, j;
				N = 0;
				for (size_t n = 0; n < NPoints; n++) {
					i = floor(trajectory[0][n] * toFit[e][0]);
					j = floor(trajectory[1][n] * toFit[e][0]);
					if (binMatrix[i][j])
						continue;
					else {
						binMatrix[i][j] = 1;
						N++;
					}
				}
				/*	Free binMatrix  */
				for (size_t i = 0; i < range; i++) {
					free(binMatrix[i]);
				}
				free(binMatrix);
				break;
			}
			case 3: {
			/*
				In d = 3, the "naive" way used above for d=2 is unreasonable because
				if requires hundreds of GBs of RAM.
				Instead, record the bins that are populated in a HASH table
				with uthash.h. This is crazy fast :-)
																						*/
				unsigned int keyLength = sizeof (bin3Key);
				bin3 *binMatrix = NULL;
				bin3Key index;
				/*	Fill bin matrix from trajectory  */
				for (size_t n = 0; n < NPoints; n++) {
					index.i = floor(trajectory[0][n] * toFit[e][0]);
					index.j = floor(trajectory[1][n] * toFit[e][0]);
					index.k = floor(trajectory[2][n] * toFit[e][0]);
					addToHash3D(&binMatrix, keyLength, &index);
				}
				N = HASH_COUNT(binMatrix);
				deleteHash3D(&binMatrix);
				break;
			}
		}
		toFit[e][0] = log10(toFit[e][0]);		/*	Take log(1/epsilon)  */
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
			We then output the slope corresponding to the best value of least squares.
 */
	const unsigned int windowSize = floor(numberEpsilon * 3 / 5);
	double leastSquares = fit_least_squares(toFit, 0, windowSize - 1, &dCap);
	double leastSquaresTmp;
	float dCapTmp;
	for (size_t n = 1; n <= numberEpsilon - windowSize; n++) {
		leastSquaresTmp = fit_least_squares(toFit, n, (windowSize - 1) + n, &dCapTmp);
		if (leastSquaresTmp < leastSquares) {
			dCap = dCapTmp;
			leastSquares = leastSquaresTmp;
		}
	}

	return dCap;
}
