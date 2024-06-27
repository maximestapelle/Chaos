#include "Utilities.h"
#include "Dynamics.h"

/*
 * The lyapunov function computes a state and constructs lyapunov exponents.
 *
 * The function is called from ChaosMain.
 */

void lyapunov() {

	FILE *fp = fopen(dataFile, "w");
	const float parameterIncrement = (userMapValues.parameterRange[1] - userMapValues.parameterRange[0]) / S;
	double lambda[dimension]; 							/*	Store the Lyapunov exponents */
	double lambdaOld[dimension];						/*	Store the last Lyapunov exponents estimates
														  	to check convergence */
	const float E_a = 1E-6;								/*	Absolute bound for convergence test in in flows */
	const float E_r = 1E-6;								/*	Relative bound for convergence test in in flows */

	size_t stateDimension = computeStateDimension(dimension);
	double state[stateDimension];								/* The current point of the state */
	size_t jacobianDimension = stateDimension - dimension;

	/*  Set the parameter to its minimum value.
	    We subtract parameterIncrement because in the loop, we will add as from s = 0.   */
	userMapValues.parameters[0] = userMapValues.parameterRange[0] - parameterIncrement;

	switch (isDiscrete) {
		case 1:
		/*
			Procedure for Maps
								*/
			switch (dimension) {
				case 1:							/*	Easy for dimension 1 */
					if (userMap == 4) {			/*	Particularly easy for the Tent map */
						for (size_t s = 0; s <= S; s++) {
							userMapValues.parameters[0] += parameterIncrement;
							lambda[0] = log((double) userMapValues.parameters[0]);
							fprintf(fp, "%f %.4lf\n", userMapValues.parameters[0], lambda[0]);
						}
					}
					else {
						for (size_t s = 0; s <= S; s++) {
							userMapValues.parameters[0] += parameterIncrement;
							state[0] = (double) userMapValues.IC[0];
							lambda[0] = 0;
							for (size_t n = 1; n <= N; n++) {
								dynamics(state);
								lambda[0] += log(fabs(state[1])) / N;
							}
							fprintf(fp, "%f %.4lf\n", userMapValues.parameters[0], lambda[0]);
						}
					}

					break;
				default: {						/* Trickier for dimension > 1 */
				/*  Solution based on Phys 221A Lecture Notes - Lyapunov Exponents
				    and their Relation to Entropy   */
					double nJacobianProduct[jacobianDimension];
					double transposednJacobianProduct[jacobianDimension];
					double H[jacobianDimension];
					for (size_t s = 0; s <= S; s++) {
						userMapValues.parameters[0] += parameterIncrement;
						for (size_t d = 0; d < dimension; d++) {
							state[d] = (double) userMapValues.IC[d];
						}
						dynamics(state);
						for (size_t d = 0; d < jacobianDimension; d++) {
							/* Copy the first jacobian into nJacobianProduct */
							nJacobianProduct[d] = state[dimension + d];
						}
						for (size_t n = 1; n < N; n++) {
							dynamics(state);
							dot_product_state(state, nJacobianProduct, nJacobianProduct, dimension);
						}
						/**  Old solution based on http://dx.doi.org/10.1016/j.cnsns.2014.06.042 eq 2.6 **/
						// 				eigen_values(nJacobianProduct, lyapunovExponents);
						// 				lyapunovExponents[0] = log(fabs(lyapunovExponents[0])) / N;
						// 				lyapunovExponents[1] = log(fabs(lyapunovExponents[1])) / N;
						/** New solution based on Phys 221A Lecture Notes - Lyapunov Exponents
						and their Relation to Entropy **/
						matrix_transpose(nJacobianProduct, transposednJacobianProduct, dimension);
						dot_product(transposednJacobianProduct, nJacobianProduct, H, dimension);
						for (size_t d = 0; d < dimension; d++) {
							/* Lambda's are the diagonal of H */
							lambda[d] = log(fabs(H[dimension * d + d])) / (2 * N);
						}
						fillFile(fp, userMapValues.parameters, lambda);
					}
					break;
				}
			}
			break;
		case 0: {
		/*
			Procedure for Flows
								*/
			double sum[dimension];
			double u[jacobianDimension];		/* 3x3 matrix is a 9 vector */
			double v[jacobianDimension];		/* 3x3 matrix is a 9 vector */

			double prodScal;
			double normV;
			// Main loop
			for (size_t s = 0; s <= S; s++) {
				userMapValues.parameters[0] += parameterIncrement;
				for (size_t i = 0; i < dimension; i++) {
					lambda[i] = 0.0;
					sum[i]	  = 0.0;
				}
				for (size_t d = 0; d < dimension; d++) {
					state[d] = (double) userMapValues.IC[d];
				}
				for (size_t i = dimension; i < stateDimension; i++) {
					// Those are delta's for the variational equation -> initalize to Identity 3x3
					if (i == 3 || i == 7 || i == 11) state[i] = 1.0;
					else 							 state[i] = 0.0;
				}
				for (size_t i = 0; i < jacobianDimension; i++) {
					u[i] = state[i];
				}

				for (size_t k = 1; k <= N; k++) {
					for (size_t i = 0; i < dimension; i++) {
						lambdaOld[i] = lambda[i];
					}
					unsigned int t = 0;
					do {
						dynamics(state);
						t += 1;
					} while (t < T);
					for (size_t i = 0; i < dimension; i++) {
						// First, set the ith column of v to the ith column of the current state
						for (size_t j = 0; j < dimension; j++) {
							v[j * dimension + i] = state[dimension + j * dimension + i];
						}
						// Gram-Schmidt orthonormalisation
						for (size_t j = 0; j < i ; j++) {
							// scalar product between ith column of v and jth column of delta's
							prodScal = 0;
							for (size_t l = 0; l < dimension; l++) {
								prodScal += v[l * dimension + i] * u[l * dimension + j];
							}
							// Update the ith column of v
							for (size_t l = 0; l < dimension; l++) {
								v[l * dimension + i] -= prodScal * u[l * dimension + j];
							}
						}
						normV = normColumn(v, i);
						// Update the delta's
						for (size_t j = 0; j < dimension; j++) {
							u[j * dimension + i] = v[j * dimension + i] / normV;
						}

						sum[i] += log(normV);
						lambda[i] = sum[i] / (k * (h * T));
					}
					// Update state with the new u
					for (size_t i = 3; i < stateDimension; i++) {
						state[i] = u[i - dimension];
					}
					/* Check convergence */
					if (normOfDifference(lambdaOld, lambda) < E_r * norm(lambda) + E_a) {
						fillFile(fp, userMapValues.parameters, lambda);
						break;
					} /* If no convergence, we do nothing -> skip this value of rho. */
				}
			}
			break;
		}
	}

	printf("Data file created: '%s'.\n", dataFile);
	fclose(fp);
}
