#include "Utilities.h"
#include "Dynamics.h"

/*
	The lyapunov function computes a trajectory and constructs lyapunov exponents.

	The function is called from ChaosMain.
 */

void lyapunov() {

	FILE *fp = fopen(dataFile, "w");
	double parameterIncrement = (userMapValues.parameterRange[1] - userMapValues.parameterRange[0]) / S;
	double trajectory[dimension];	/*	The current point of the trajectory */
	double lyapunovExponents[dimension];


	/*	Set the parameter to its minimum value.
		We subtract parameterIncrement because in the loop, we will add as from s = 0. */
	userMapValues.parameters[0] = userMapValues.parameterRange[0] - parameterIncrement;

	/*	Main process */
	switch (userMap) {
		case 1:
			for (size_t s = 0; s <= S; s++) {
				userMapValues.parameters[0] += parameterIncrement;
				trajectory[0] = userMapValues.IC[0];
				lyapunovExponents[0] = 0;
				for (size_t n = 1; n <= N; n++) {
					lyapunovExponents[0] += logisticLyapunov(trajectory) / N;
					logistic(trajectory);
				}
				fprintf(fp, "%f %.4f\n", userMapValues.parameters[0], lyapunovExponents[0]);
			}
			break;
		case 2:
			for (size_t s = 0; s <= S; s++) {
				userMapValues.parameters[0] += parameterIncrement;
				trajectory[0] = userMapValues.IC[0];
				lyapunovExponents[0] = 0;
				for (size_t n = 1; n <= N; n++) {
					lyapunovExponents[0] += logisticExpLyapunov(trajectory) / N;
					logisticExp(trajectory);
				}
				fprintf(fp, "%f %.4f\n", userMapValues.parameters[0], lyapunovExponents[0]);
			}
			break;
		case 3:
			for (size_t s = 0; s <= S; s++) {
				userMapValues.parameters[0] += parameterIncrement;
				trajectory[0] = userMapValues.IC[0];
				lyapunovExponents[0] = 0;
				for (size_t n = 1; n <= N; n++) {
					lyapunovExponents[0] += gaussLyapunov(trajectory) / N;
					gauss(trajectory);
				}
				fprintf(fp, "%f %.4f\n", userMapValues.parameters[0], lyapunovExponents[0]);
			}
			break;
		case 4:
			for (size_t s = 0; s <= S; s++) {
				userMapValues.parameters[0] += parameterIncrement;
				/*	Particularly easy for the Tent map ! */
				lyapunovExponents[0] = log(userMapValues.parameters[0]);
				fprintf(fp, "%f %.4f\n", userMapValues.parameters[0], lyapunovExponents[0]);
			}
			break;
		case 5:
			for (size_t s = 0; s <= S; s++) {
				userMapValues.parameters[0] += parameterIncrement;
				trajectory[0] = userMapValues.IC[0];
				lyapunovExponents[0] = 0;
				for (size_t n = 1; n <= N; n++) {
					lyapunovExponents[0] += circleLyapunov(trajectory) / N;
					circle(trajectory);
				}
				fprintf(fp, "%f %.4f\n", userMapValues.parameters[0], lyapunovExponents[0]);
			}
			break;
		case 6: {
			double nJacobianProduct[2][2];
			double nthJacobian[2][2];
			double transposednJacobianProduct[2][2];
			double H[2][2];
			for (size_t s = 0; s <= S; s++) {
				userMapValues.parameters[0] += parameterIncrement;
				trajectory[0] = userMapValues.IC[0];
				trajectory[1] = userMapValues.IC[1];
				tinkerbellJacobian(trajectory, nJacobianProduct);
				for (size_t n = 1; n < N; n++) {
					tinkerbell(trajectory);
					tinkerbellJacobian(trajectory, nthJacobian);
					dot_product(nthJacobian, nJacobianProduct, nJacobianProduct);
				}
/**  Old solution based on http://dx.doi.org/10.1016/j.cnsns.2014.06.042 eq 2.6 **/
// 				eigen_values(nJacobianProduct, lyapunovExponents);
// 				lyapunovExponents[0] = log(fabs(lyapunovExponents[0])) / N;
// 				lyapunovExponents[1] = log(fabs(lyapunovExponents[1])) / N;
/** New solution based on Phys 221A Lecture Notes - Lyapunov Exponents
and their Relation to Entropy **/
				matrix_transpose(nJacobianProduct, transposednJacobianProduct);
				dot_product(transposednJacobianProduct, nJacobianProduct, H);
				lyapunovExponents[0] = log(fabs(H[0][0])) / (2 * N);
				lyapunovExponents[1] = log(fabs(H[1][1])) / (2 * N);
				fprintf(fp, "%f %.4f %.4f\n", userMapValues.parameters[0], lyapunovExponents[0], lyapunovExponents[1]);
			}
			break;
		}
		case 7: {
			double nJacobianProduct[2][2];
			double nthJacobian[2][2];
			double transposednJacobianProduct[2][2];
			double H[2][2];
			for (size_t s = 0; s <= S; s++) {
				userMapValues.parameters[0] += parameterIncrement;
				trajectory[0] = userMapValues.IC[0];
				trajectory[1] = userMapValues.IC[1];
				henonJacobian(trajectory, nJacobianProduct);
				for (size_t n = 1; n < N; n++) {
					henon(trajectory);
					henonJacobian(trajectory, nthJacobian);
					dot_product(nthJacobian, nJacobianProduct, nJacobianProduct);
				}
/**  Old solution based on http://dx.doi.org/10.1016/j.cnsns.2014.06.042 eq 2.6 **/
// 				eigen_values(nJacobianProduct, lyapunovExponents);
// 				lyapunovExponents[0] = log(fabs(lyapunovExponents[0])) / N;
// 				lyapunovExponents[1] = log(fabs(lyapunovExponents[1])) / N;
/** New solution based on Phys 221A Lecture Notes - Lyapunov Exponents
and their Relation to Entropy **/
				matrix_transpose(nJacobianProduct, transposednJacobianProduct);
				dot_product(transposednJacobianProduct, nJacobianProduct, H);
				lyapunovExponents[0] = log(fabs(H[0][0])) / (2 * N);
				lyapunovExponents[1] = log(fabs(H[1][1])) / (2 * N);
// 				if (isnan(lyapunovExponents[0]) || isinf(lyapunovExponents[0])) {
// 					if (isnan(lyapunovExponents[1]) || isinf(lyapunovExponents[1])) continue;
// 					else fprintf(fp, "%f  %.4f\n", userMapValues.parameters[0], lyapunovExponents[1]);
// 				}
// 				else if (isnan(lyapunovExponents[1]) || isinf(lyapunovExponents[1])) {
// 					fprintf(fp, "%f %.4f \n", userMapValues.parameters[0], lyapunovExponents[0]);
// 				}
// 				else fprintf(fp, "%f %.4f %.4f\n", userMapValues.parameters[0], lyapunovExponents[0], lyapunovExponents[1]);
				fprintf(fp, "%f %.4f %.4f\n", userMapValues.parameters[0], lyapunovExponents[0], lyapunovExponents[1]);
			}
			break;
		}
		case 8: {
			const float h = 0.001;
			const float T = h * 10;
			const float E_a = 1E-6;
			const float E_r = 1E-6;

			double lyapunovExponentsOld[3];
			double sum[3];
			double *state = (double *) malloc(12 * sizeof (double));	/* 12-dim state */
			double *u = (double *) malloc(9 * sizeof (double));			/* 3x3 matrix is a 9 vector */
			double *v = (double *) malloc(9 * sizeof (double));			/* 3x3 matrix is a 9 vector */

			double prodScal;
			double normV;
			// Main loop
			for (size_t s = 0; s <= S; s++) {
				userMapValues.parameters[0] += parameterIncrement;
				for (size_t i = 0; i < 3; i++) {
					lyapunovExponents[i] = 0.0;
					sum[i] 				 = 0.0;
				}
				for (size_t d = 0; d < 3; d++) {
					state[d] = userMapValues.IC[d];
				}
				for (size_t i = 3; i < 12; i++) {
					// Those are delta's for the variational equation -> initalize to Identity 3x3
					if (i == 3 || i == 7 || i == 11) state[i] = 1.0;
					else 							 state[i] = 0.0;
				}
				for (size_t i = 0; i < 9; i++) {
					u[i] = state[i];
				}

				for (size_t k = 1; k <= N; k++) {
					for (size_t i = 0; i < 3; i++) {
						lyapunovExponentsOld[i] = lyapunovExponents[i];
					}
					float t = 0.0;
					do {
						lorenzRK4Full(state, h);
						t += h;
					} while (t < T);
					for (size_t i = 0; i < 3; i++) {
						// First, set the ith column of v to the ith column of the current state
						for (size_t j = 0; j < 3; j++) {
							v[j * 3 + i] = state[3 + j * 3 + i];
						}
						// Gram-Schmidt orthonormalisation
						for (size_t j = 0; j < i ; j++) {
							// scalar product between ith column of v and jth column of delta's
							prodScal = 0;
							for (size_t l = 0; l < 3; l++) {
								prodScal += v[l * 3 + i] * u[l * 3 + j];
							}
							// Update the ith column of v
							for (size_t l = 0; l < 3; l++) {
								v[l * 3 + i] -= prodScal * u[l * 3 + j];
							}
						}
						normV = normColumn(v, i);
						// Update the delta's
						for (size_t j = 0; j < 3; j++) {
							u[j * 3 + i] = v[j * 3 + i] / normV;
						}

						sum[i] += log(normV);
						lyapunovExponents[i] = sum[i] / (k * T);
					}
					// Update state with the new u
					for (size_t i = 3; i < 12; i++) {
						state[i] = u[i - 3];
					}
					/*	Check convergence */
					if (normOfDifference(lyapunovExponentsOld, lyapunovExponents) < E_r * norm(lyapunovExponents) + E_a)
					{
						fprintf(fp, "%f %f %f %f\n", userMapValues.parameters[0],
									lyapunovExponents[0], lyapunovExponents[1], lyapunovExponents[2]);
						break;
					} /* If no convergence, we do nothing -> skip this value of rho. */
				}
			}
		}
		break;
		case 9: {
			const float h = 0.01;
			const float T = h * 10;
			const float E_a = 1E-6;
			const float E_r = 1E-6;

			double lyapunovExponentsOld[3];
			double sum[3];
			double *state = (double *) malloc(12 * sizeof (double));		/* 12-dim state */
			double *u = (double *) malloc(9 * sizeof (double));		/* 3x3 matrix is a 9 vector */
			double *v = (double *) malloc(9 * sizeof (double));		/* 3x3 matrix is a 9 vector */

			double prodScal;
			double normV;
			// Main loop
			for (size_t s = 0; s <= S; s++) {
				userMapValues.parameters[0] += parameterIncrement;
				for (size_t i = 0; i < 3; i++) {
					lyapunovExponents[i] = 0.0;
					sum[i] 				 = 0.0;
				}
				for (size_t d = 0; d < 3; d++) {
					state[d] = userMapValues.IC[d];
				}
				for (size_t i = 3; i < 12; i++) {
					// Those are delta's for the variational equation -> initalize to Identity 3x3
					if (i == 3 || i == 7 || i == 11) state[i] = 1.0;
					else 							 state[i] = 0.0;
				}
				for (size_t i = 0; i < 9; i++) {
					u[i] = state[i];
				}

				for (size_t k = 1; k <= N; k++) {
					for (size_t i = 0; i < 3; i++) {
						lyapunovExponentsOld[i] = lyapunovExponents[i];
					}
					float t = 0.0;
					do {
						rosslerRK4Full(state, h);
						t += h;
					} while (t < T);
					for (size_t i = 0; i < 3; i++) {
						// First, set the ith column of v to the ith column of the current state
						for (size_t j = 0; j < 3; j++) {
							v[j * 3 + i] = state[3 + j * 3 + i];
						}
						// Gram-Schmidt orthonormalisation
						for (size_t j = 0; j < i ; j++) {
							// scalar product between ith column of v and jth column of delta's
							prodScal = 0;
							for (size_t l = 0; l < 3; l++) {
								prodScal += v[l * 3 + i] * u[l * 3 + j];
							}
							// Update the ith column of v
							for (size_t l = 0; l < 3; l++) {
								v[l * 3 + i] -= prodScal * u[l * 3 + j];
							}
						}
						normV = normColumn(v, i);
						// Update the delta's
						for (size_t j = 0; j < 3; j++) {
							u[j * 3 + i] = v[j * 3 + i] / normV;
						}

						sum[i] += log(normV);
						lyapunovExponents[i] = sum[i] / (k * T);
					}
					// Update state with the new u
					for (size_t i = 3; i < 12; i++) {
						state[i] = u[i - 3];
					}
					/*	Check convergence */
					if (normOfDifference(lyapunovExponentsOld, lyapunovExponents) < E_r * norm(lyapunovExponents) + E_a) {
						fprintf(fp, "%f %f %f %f\n", userMapValues.parameters[0],
									lyapunovExponents[0], lyapunovExponents[1], lyapunovExponents[2]);
						break;
					} /* If no convergence, we do nothing -> skip this value of rho. */
				}
			}

		}
		break;
	}

	printf("Data file created: '%s'.\n", dataFile);
	fclose(fp);
}
