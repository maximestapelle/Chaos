#include "Chaos.h"
#include "Utilities.h"

/* Bifurcation will only check and return the first variable. Could be changed in the future. */
void lyapunov() {
	
	FILE * fp;
	float parameterIncrement = (userMapValues.parameterRange[1] - userMapValues.parameterRange[0]) / S;
	float trajectory[dimension]; /* The current point of the trajectory */
	float lyapunovExponents[dimension]; 

	fp = fopen(dataFile, "w");
		
	
	/* Set the parameter to its minimum value. We subtract parameterIncrement because in the loop, we will add as from s = 0. */
	userMapValues.parameters[0] = userMapValues.parameterRange[0] - parameterIncrement;

	
	switch (userMap) {
		case 1:
			for (size_t s = 0; s <= S; s++) {
				userMapValues.parameters[0] += parameterIncrement;
				for (size_t d = 0; d < dimension; d++) {
					trajectory[d] = userMapValues.IC[d];
				}
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
				for (size_t d = 0; d < dimension; d++) {
					trajectory[d] = userMapValues.IC[d];
				}
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
				for (size_t d = 0; d < dimension; d++) {
					trajectory[d] = userMapValues.IC[d];
				}
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
				/* particularly easy for the Tent map ! */
				lyapunovExponents[0] = log(userMapValues.parameters[0]);
				fprintf(fp, "%f %.4f\n", userMapValues.parameters[0], lyapunovExponents[0]);
			}
			break;
		case 5:
			for (size_t s = 0; s <= S; s++) {
				userMapValues.parameters[0] += parameterIncrement;
				for (size_t d = 0; d < dimension; d++) {
					trajectory[d] = userMapValues.IC[d];
				}
				lyapunovExponents[0] = 0;
				for (size_t n = 1; n <= N; n++) {
					lyapunovExponents[0] += circleLyapunov(trajectory) / N;
					circle(trajectory);
				}
				fprintf(fp, "%f %.4f\n", userMapValues.parameters[0], lyapunovExponents[0]);
			}
			break;
		case 6: {
			float nJacobianProduct[dimension][dimension];
			float nthJacobian[dimension][dimension];
			float transposednJacobianProduct[dimension][dimension];
			float H[dimension][dimension];
			for (size_t s = 0; s <= S; s++) {
				userMapValues.parameters[0] += parameterIncrement;
				for (size_t d = 0; d < dimension; d++) {
					trajectory[d] = userMapValues.IC[d];
				}
				tinkerbellJacobian(trajectory, nJacobianProduct);
				for (size_t n = 1; n < N; n++) {
					tinkerbell(trajectory);
					tinkerbellJacobian(trajectory, nthJacobian);
					dot_product(nthJacobian, nJacobianProduct, nJacobianProduct);
				}
/**  Old solution based on http://dx.doi.org/10.1016/j.cnsns.2014.06.042 eq 2.6 **/
// 				eigen_values(nJacobianProduct, lyapunovExponents);
// 				lyapunovExponents[0] = log(fabsf(lyapunovExponents[0])) / N;
// 				lyapunovExponents[1] = log(fabsf(lyapunovExponents[1])) / N;
/** New solution based on Phys 221A Lecture Notes - Lyapunov Exponents
and their Relation to Entropy **/ 
				matrix_transpose(nJacobianProduct, transposednJacobianProduct);
				dot_product(transposednJacobianProduct, nJacobianProduct, H);
				lyapunovExponents[0] = log(fabsf(H[0][0])) / (2 * N);
				lyapunovExponents[1] = log(fabsf(H[1][1])) / (2 * N);
				fprintf(fp, "%f %.4f %.4f\n", userMapValues.parameters[0], lyapunovExponents[0], lyapunovExponents[1]);
			}
			break;
		}
		case 7: {
			float nJacobianProduct[dimension][dimension];
			float nthJacobian[dimension][dimension];
			float transposednJacobianProduct[dimension][dimension];
			float H[dimension][dimension];
			for (size_t s = 0; s <= S; s++) {
				userMapValues.parameters[0] += parameterIncrement;
				for (size_t d = 0; d < dimension; d++) {
					trajectory[d] = userMapValues.IC[d];
				}
				hénonJacobian(trajectory, nJacobianProduct);
				for (size_t n = 1; n < N; n++) {
					hénon(trajectory);
					hénonJacobian(trajectory, nthJacobian);
					dot_product(nthJacobian, nJacobianProduct, nJacobianProduct);
				}
/**  Old solution based on http://dx.doi.org/10.1016/j.cnsns.2014.06.042 eq 2.6 **/
// 				eigen_values(nJacobianProduct, lyapunovExponents);
// 				lyapunovExponents[0] = log(fabsf(lyapunovExponents[0])) / N;
// 				lyapunovExponents[1] = log(fabsf(lyapunovExponents[1])) / N;
/** New solution based on Phys 221A Lecture Notes - Lyapunov Exponents
and their Relation to Entropy **/ 
				matrix_transpose(nJacobianProduct, transposednJacobianProduct);
				dot_product(transposednJacobianProduct, nJacobianProduct, H);
				lyapunovExponents[0] = log(fabsf(H[0][0])) / (2 * N);
				lyapunovExponents[1] = log(fabsf(H[1][1])) / (2 * N);
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
	}

	printf("Data file created: '%s'.\n", dataFile);
	fclose(fp);
}
