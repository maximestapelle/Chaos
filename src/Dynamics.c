#include "Utilities.h"
#include "Dynamics.h"

/*	Contains the dynamics of each dynamical system.

	All parameters are on the global variable userMapValues.

	For flows, we also have here the derivatives (Jacobian) and implementation of the
	Euler and Runge-Kutta 4th order methods. This version is not very elegant and has
	a LOT of hardcoding (Euler and RK4 and specific to the chosen dynamical system) to allow
	for better performance.

	Dynamics are used by all actions
 */


/*	1 - The logistic map */
void logistic(double trajectory[]) {
	trajectory[0] =  userMapValues.parameters[0] * trajectory[0] * (1 - trajectory[0]);
}
double logisticDerivative(double trajectory[]) {
	return userMapValues.parameters[0] * (1 - 2 * trajectory[0]);
}
double logisticLyapunov(double trajectory[]) {
	return log(fabs(userMapValues.parameters[0])) + log(fabs(1 - 2 * trajectory[0]));
}

/*	2 - The exponential logistic map */
void logisticExp(double trajectory[]) {
	trajectory[0] = userMapValues.parameters[0] * trajectory[0] * exp(-trajectory[0]);
}
double logisticExpDerivative(double trajectory[]) {
	return userMapValues.parameters[0] * (1 - trajectory[0]) * exp(-trajectory[0]);
}
double logisticExpLyapunov(double trajectory[]) {
	return log(fabs(userMapValues.parameters[0])) + log(fabs(1 - trajectory[0])) - trajectory[0];
}

/*	3 - The gauss map */
void gauss(double trajectory[]) {
	 trajectory[0] = exp(-userMapValues.parameters[1] * pow(trajectory[0], 2)) + userMapValues.parameters[0];
}
double gaussDerivative(double trajectory[]) {
	return 	-2 * userMapValues.parameters[1] * trajectory[0] *
			exp(- userMapValues.parameters[1] * pow(trajectory[0], 2));
}
double gaussLyapunov(double trajectory[]) {
	return  -userMapValues.parameters[1] * pow(trajectory[0], 2) +
			log(fabs(2 * userMapValues.parameters[1] * trajectory[0]));
}

/*	4 - The tent map */
void tent(double trajectory[]) {
	if (trajectory[0] < 0.5) {
		trajectory[0] =  userMapValues.parameters[0] * trajectory[0];
	}
	else {
		trajectory[0] =  userMapValues.parameters[0] * (1 - trajectory[0]);
	}
}

/*	5 - The Circle map */
void circle(double trajectory[]) {
	double result;

	result = trajectory[0] + userMapValues.parameters[0] / (2 * M_PI) *
			 ((double) sin(2 * M_PI * trajectory[0])) + userMapValues.parameters[1];
	// Result is modulo 1
	result -= floor(result);

	trajectory[0] = result;
}
double circleDerivative(double trajectory[]) {
	return 1 + userMapValues.parameters[0] * cos(2 * M_PI * trajectory[0]);
}
double circleLyapunov(double trajectory[]) {
	return log(fabs(circleDerivative(trajectory)));
}

/*	6 - The tinkerbell map */
void tinkerbell(double trajectory[]) {
	double x, y;
	x = pow(trajectory[0], 2) - pow(trajectory[1], 2) + userMapValues.parameters[1] * trajectory[0] +
		userMapValues.parameters[0] * trajectory[1];
	y = 2 * trajectory[0] * trajectory[1] + userMapValues.parameters[2] * trajectory[0] +
		userMapValues.parameters[3] * trajectory[1];

	trajectory[0] = x;
	trajectory[1] = y;
}
void tinkerbellJacobian(double trajectory[], double jacobian[][2]) {
	jacobian[0][0] = 2 * trajectory[0] + userMapValues.parameters[1];
	jacobian[0][1] = -2 * trajectory[1] + userMapValues.parameters[0];
	jacobian[1][0] = 2 * trajectory[1] + userMapValues.parameters[2];
	jacobian[1][1] = 2 * trajectory[0] + userMapValues.parameters[3];

}

/*	7 - The Hénon map */
void henon(double trajectory[]) {
	double x, y;
	x = 1 - userMapValues.parameters[0] * pow(trajectory[0], 2) + trajectory[1];
	y = userMapValues.parameters[1] * trajectory[0];

	trajectory[0] = x;
	trajectory[1] = y;
}
void henonJacobian(double trajectory[], double jacobian[][2]) {
	jacobian[0][0] = -2 * userMapValues.parameters[0] * trajectory[0];
	jacobian[0][1] = 1;
	jacobian[1][0] = userMapValues.parameters[1];
	jacobian[1][1] = 0;

}

/*	8 - The Lorenz system */
static void lorenzEvolution(double input[], double fgh[]) {
	fgh[0] = userMapValues.parameters[1] * (input[1] - input[0]);
	fgh[1] = input[0] * (userMapValues.parameters[0] - input[2]) - input[1];
	fgh[2] = input[0] * input[1] - userMapValues.parameters[2] * input[2];

}
static void lorenzEvolutionFull(const double *state,
									  double stateEvolution[])
{
	/* Vector of size DIMENSION + DIMENSION^2 for the evolution of extended state */
	/* This is a f, g and h functions of xdot, ydot, zdot in indices 0, 1 and 2 */
	/* And a hardcoding of the 3x3 evolution of variational equations (delta or Phi_t),
	   written as a 9-vector. Its evolution is given by the Jacobian times delta itself. */

	stateEvolution[0]  = userMapValues.parameters[1] * (state[1] - state[0]);
	stateEvolution[1]  = state[0] * (userMapValues.parameters[0] - state[2]) - state[1];
	stateEvolution[2]  = state[0] * state[1] - userMapValues.parameters[2] * state[2];

	stateEvolution[3]  = userMapValues.parameters[1] * (- state[3] + state[6]);
	stateEvolution[4]  = userMapValues.parameters[1] * (- state[4] + state[7]);
	stateEvolution[5]  = userMapValues.parameters[1] * (- state[5] + state[8]);
	stateEvolution[6]  = (userMapValues.parameters[0] - state[2]) * state[3] - state[6] - state[0] * state[9];
	stateEvolution[7]  = (userMapValues.parameters[0] - state[2]) * state[4] - state[7] - state[0] * state[10];
	stateEvolution[8]  = (userMapValues.parameters[0] - state[2]) * state[5] - state[8] - state[0] * state[11];
	stateEvolution[9]  = state[1] * state[3] + state[0] * state[6] - userMapValues.parameters[2] * state[9];
	stateEvolution[10] = state[1] * state[4] + state[0] * state[7] - userMapValues.parameters[2] * state[10];
	stateEvolution[11] = state[1] * state[5] + state[0] * state[8] - userMapValues.parameters[2] * state[11];
}
/*	This is Newton/Euler method for Lorenz (so the easiest one, based on 1st order Taylor series) */
void lorenzEuler(double trajectory[], const float dt) {
	double x, y, z;
	double fgh[3];

	lorenzEvolution(trajectory, fgh);

	x = trajectory[0] + dt * fgh[0];
	y = trajectory[1] + dt * fgh[1];
	z = trajectory[2] + dt * fgh[2];

	trajectory[0] = x;
	trajectory[1] = y;
	trajectory[2] = z;
}
/*	This is Runge-Kutta 4th order */
void lorenzRK4(double trajectory[], const float h) {
	double array[3];
	double k1[3], k2[3], k3[3], k4[3];

	lorenzEvolution(trajectory, k1);

	array[0] = trajectory[0] + h / 2 * k1[0];
	array[1] = trajectory[1] + h / 2 * k1[1];
	array[2] = trajectory[2] + h / 2 * k1[2];

	lorenzEvolution(array, k2);

	array[0] = trajectory[0] + h / 2 * k2[0];
	array[1] = trajectory[1] + h / 2 * k2[1];
	array[2] = trajectory[2] + h / 2 * k2[2];

	lorenzEvolution(array, k3); /* Now fgh = k3 */

	array[0] = trajectory[0] + h * k3[0];
	array[1] = trajectory[1] + h * k3[1];
	array[2] = trajectory[2] + h * k3[2];

	lorenzEvolution(array, k4); /* Now fgh = k4 */

	trajectory[0] = trajectory[0] + h / 6 * (k1[0] + 2 * k2[0] + 2 * k3[0] + k4[0]);
	trajectory[1] = trajectory[1] + h / 6 * (k1[1] + 2 * k2[1] + 2 * k3[1] + k4[1]);
	trajectory[2] = trajectory[2] + h / 6 * (k1[2] + 2 * k2[2] + 2 * k3[2] + k4[2]);
}
/*	This is Runge-Kutta 4th order for the full state, when variation equations are needed (Lyapunov) */
void lorenzRK4Full(double *state, const float h)
{
	double array[12];
	double k1[12], k2[12], k3[12], k4[12];

	lorenzEvolutionFull(state, k1);

	for (size_t i = 0; i < 12; i++) {
		array[i] = state[i] + h / 2 * k1[i];
	}

	lorenzEvolutionFull(array, k2);

	for (size_t i = 0; i < 12; i++) {
		array[i] = state[i] + h / 2 * k2[i];
	}

	lorenzEvolutionFull(array, k3);

	for (size_t i = 0; i < 12; i++) {
		array[i] = state[i] + h / 2 * k3[i];
	}

	lorenzEvolutionFull(array, k4);

	for (size_t i = 0; i < 12; i++) {
		state[i] = state[i] + h / 6 * (k1[i] + 2 * k2[i] + 2 * k3[i] + k4[i]);
	}
}

/*	9 - The Rössler system */
static void rosslerEvolution(double input[], double fgh[]) {
	fgh[0] = -input[1] - input[2];
	fgh[1] = input[0] + userMapValues.parameters[2] * input[1];
	fgh[2] = userMapValues.parameters[0] + input[2] * (input[0] - userMapValues.parameters[1]);

}
static void rosslerEvolutionFull(const  double *state,
										double stateEvolution[])
{
	/* Vector of size DIMENSION + DIMENSION^2 for the evolution of extended state */
	/* This is a f, g and h functions of xdot, ydot, zdot in indices 0, 1 and 2 */
	/* And a hardcoding of the 3x3 evolution of variational equations (delta or Phi_t),
	   written as a 9-vector. Its evolution is given by the Jacobian times delta itself. */

	stateEvolution[0]  = - state[1] - state[2];
	stateEvolution[1]  = state[0] + userMapValues.parameters[2] * state[1];
	stateEvolution[2]  = userMapValues.parameters[0] + state[2] * (state[0] - userMapValues.parameters[1]);

	stateEvolution[3]  = - state[6] - state[9];
	stateEvolution[4]  = - state[7] - state[10];
	stateEvolution[5]  = - state[8] - state[11];
	stateEvolution[6]  = state[3] + userMapValues.parameters[2] * state[6];
	stateEvolution[7]  = state[4] + userMapValues.parameters[2] * state[7];
	stateEvolution[8]  = state[5] + userMapValues.parameters[2] * state[8];
	stateEvolution[9]  = state[2] * state[3] + (state[0] - userMapValues.parameters[1]) * state[9];
	stateEvolution[10] = state[2] * state[4] + (state[0] - userMapValues.parameters[1]) * state[10];
	stateEvolution[11] = state[2] * state[5] + (state[0] - userMapValues.parameters[1]) * state[11];
}
/* This is Newton/Euler method for Rossler (so the easiest one, based on 1st order Taylor series) */
void rosslerEuler(double trajectory[], const float dt) {
	double x, y, z;
	double fgh[3];

	rosslerEvolution(trajectory, fgh);

	x = trajectory[0] + dt * fgh[0];
	y = trajectory[1] + dt * fgh[1];
	z = trajectory[2] + dt * fgh[2];

	trajectory[0] = x;
	trajectory[1] = y;
	trajectory[2] = z;
}
/* This is Runge-Kutta 4th order */
void rosslerRK4(double trajectory[], const float h) {
	double array[3];
	double k1[3], k2[3], k3[3], k4[3];

	rosslerEvolution(trajectory, k1);

	array[0] = trajectory[0] + h / 2 * k1[0];
	array[1] = trajectory[1] + h / 2 * k1[1];
	array[2] = trajectory[2] + h / 2 * k1[2];

	rosslerEvolution(array, k2);

	array[0] = trajectory[0] + h / 2 * k2[0];
	array[1] = trajectory[1] + h / 2 * k2[1];
	array[2] = trajectory[2] + h / 2 * k2[2];

	rosslerEvolution(array, k3); /* Now fgh = k3 */

	array[0] = trajectory[0] + h * k3[0];
	array[1] = trajectory[1] + h * k3[1];
	array[2] = trajectory[2] + h * k3[2];

	rosslerEvolution(array, k4); /* Now fgh = k4 */

	trajectory[0] = trajectory[0] + h / 6 * (k1[0] + 2 * k2[0] + 2 * k3[0] + k4[0]);
	trajectory[1] = trajectory[1] + h / 6 * (k1[1] + 2 * k2[1] + 2 * k3[1] + k4[1]);
	trajectory[2] = trajectory[2] + h / 6 * (k1[2] + 2 * k2[2] + 2 * k3[2] + k4[2]);
}
/*	This is Runge-Kutta 4th order for the full state, when variation equations are needed (Lyapunov) */
void rosslerRK4Full(double *state, const float h)
{
	double array[12];
	double k1[12], k2[12], k3[12], k4[12];

	rosslerEvolutionFull(state, k1);

	for (size_t i = 0; i < 12; i++) {
		array[i] = state[i] + h / 2 * k1[i];
	}

	rosslerEvolutionFull(array, k2);

	for (size_t i = 0; i < 12; i++) {
		array[i] = state[i] + h / 2 * k2[i];
	}

	rosslerEvolutionFull(array, k3);

	for (size_t i = 0; i < 12; i++) {
		array[i] = state[i] + h / 2 * k3[i];
	}

	rosslerEvolutionFull(array, k4);

	for (size_t i = 0; i < 12; i++) {
		state[i] = state[i] + h / 6 * (k1[i] + 2 * k2[i] + 2 * k3[i] + k4[i]);
	}
}
