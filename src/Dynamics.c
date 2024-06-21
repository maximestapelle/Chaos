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

evolve_t dynamics;				/*	Function pointer for the dynamics to be called by actions	*/

/*	1 - The logistic map */
void logistic(double *state) {

	state[0] =  userMapValues.parameters[0] * state[0] * (1 - state[0]);
}
void logisticFull(double *state) {

	state[1] = userMapValues.parameters[0] * (1 - 2 * state[0]);
	logistic(state);
}

/*	2 - The exponential logistic map */
void logisticExp(double *state) {

	state[0] = userMapValues.parameters[0] * state[0] * exp(-state[0]);
}
void logisticExpFull(double *state) {

	state[1] = userMapValues.parameters[0] * (1 - state[0]) * exp(-state[0]);
	logisticExp(state);
}

/*	3 - The gauss map */
void gauss(double *state) {

	 state[0] = exp(-userMapValues.parameters[1] * pow(state[0], 2)) + userMapValues.parameters[0];
}
void gaussFull(double *state) {

	state[1] =	-2 * userMapValues.parameters[1] * state[0] *
				exp(- userMapValues.parameters[1] * pow(state[0], 2));
	gauss(state);
}

/*	4 - The tent map */
void tent(double *state) {
	if (state[0] < 0.5) {
		state[0] =  userMapValues.parameters[0] * state[0];
	}
	else {
		state[0] =  userMapValues.parameters[0] * (1 - state[0]);
	}
}

/*	5 - The Circle map */
void circle(double *state) {

	double result;

	result = state[0] + userMapValues.parameters[0] / (2 * M_PI) *
			 ((double) sin(2 * M_PI * state[0])) + userMapValues.parameters[1];
	// Result is modulo 1
	result -= floor(result);

	state[0] = result;
}
void circleFull(double *state) {

	state[1] =  1 + userMapValues.parameters[0] * cos(2 * M_PI * state[0]);
	circle(state);
}

/*	6 - The tinkerbell map */
void tinkerbell(double *state) {

	double x, y;
	x = pow(state[0], 2) - pow(state[1], 2) + userMapValues.parameters[1] * state[0] +
		userMapValues.parameters[0] * state[1];
	y = 2 * state[0] * state[1] + userMapValues.parameters[2] * state[0] +
		userMapValues.parameters[3] * state[1];

	state[0] = x;
	state[1] = y;
}
void tinkerbellFull(double *state) {

	state[2] =  2 * state[0] + userMapValues.parameters[1];
	state[3] = -2 * state[1] + userMapValues.parameters[0];
	state[4] =  2 * state[1] + userMapValues.parameters[2];
	state[5] =  2 * state[0] + userMapValues.parameters[3];
	tinkerbell(state);
}

/*	7 - The Hénon map */
void henon(double *state) {

	double x, y;
	x = 1 - userMapValues.parameters[0] * pow(state[0], 2) + state[1];
	y = userMapValues.parameters[1] * state[0];

	state[0] = x;
	state[1] = y;
}
void henonFull(double *state) {

	state[2] = -2 * userMapValues.parameters[0] * state[0];
	state[3] = 1;
	state[4] = userMapValues.parameters[1];
	state[5] = 0;
	henon(state);
}

/*	8 - The Lorenz system */
static void lorenz(const double input[], double fgh[]) {
	fgh[0] = userMapValues.parameters[1] * (input[1] - input[0]);
	fgh[1] = input[0] * (userMapValues.parameters[0] - input[2]) - input[1];
	fgh[2] = input[0] * input[1] - userMapValues.parameters[2] * input[2];

}
static void lorenzFull(const double *state,
									  double stateEvolution[])
{
	/* Vector of size DIMENSION + DIMENSION^2 for the evolution of extended state */
	/* This is a f, g and h functions of xdot, ydot, zdot in indices 0, 1 and 2 */
	/* And a hardcoding of the 3x3 evolution of variational equations (delta or Phi_t),
	   written as a 9-vector. Its evolution is given by the Jacobian times delta itself. */

	lorenz(state, stateEvolution);

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
void lorenzEuler(double *state) {
	double x, y, z;
	float dt = h;
	double fgh[3];

	lorenz(state, fgh);

	x = state[0] + dt * fgh[0];
	y = state[1] + dt * fgh[1];
	z = state[2] + dt * fgh[2];

	state[0] = x;
	state[1] = y;
	state[2] = z;
}
/*	This is Runge-Kutta 4th order */
void lorenzRK4(double *state) {
	double array[3];
	double k1[3], k2[3], k3[3], k4[3];

	lorenz(state, k1);

	array[0] = state[0] + h / 2 * k1[0];
	array[1] = state[1] + h / 2 * k1[1];
	array[2] = state[2] + h / 2 * k1[2];

	lorenz(array, k2);

	array[0] = state[0] + h / 2 * k2[0];
	array[1] = state[1] + h / 2 * k2[1];
	array[2] = state[2] + h / 2 * k2[2];

	lorenz(array, k3); /* Now fgh = k3 */

	array[0] = state[0] + h * k3[0];
	array[1] = state[1] + h * k3[1];
	array[2] = state[2] + h * k3[2];

	lorenz(array, k4); /* Now fgh = k4 */

	state[0] = state[0] + h / 6 * (k1[0] + 2 * k2[0] + 2 * k3[0] + k4[0]);
	state[1] = state[1] + h / 6 * (k1[1] + 2 * k2[1] + 2 * k3[1] + k4[1]);
	state[2] = state[2] + h / 6 * (k1[2] + 2 * k2[2] + 2 * k3[2] + k4[2]);
}
/*	This is Runge-Kutta 4th order for the full state, when variation equations are needed (Lyapunov) */
void lorenzRK4Full(double *state)
{
	double array[12];
	double k1[12], k2[12], k3[12], k4[12];

	lorenzFull(state, k1);

	for (size_t i = 0; i < 12; i++) {
		array[i] = state[i] + h / 2 * k1[i];
	}

	lorenzFull(array, k2);

	for (size_t i = 0; i < 12; i++) {
		array[i] = state[i] + h / 2 * k2[i];
	}

	lorenzFull(array, k3);

	for (size_t i = 0; i < 12; i++) {
		array[i] = state[i] + h / 2 * k3[i];
	}

	lorenzFull(array, k4);

	for (size_t i = 0; i < 12; i++) {
		state[i] = state[i] + h / 6 * (k1[i] + 2 * k2[i] + 2 * k3[i] + k4[i]);
	}
}

/*	9 - The Rössler system */
static void rossler(const double input[], double fgh[]) {
	fgh[0] = -input[1] - input[2];
	fgh[1] = input[0] + userMapValues.parameters[2] * input[1];
	fgh[2] = userMapValues.parameters[0] + input[2] * (input[0] - userMapValues.parameters[1]);

}
static void rosslerFull(const  double *state,
										double stateEvolution[])
{
	/* Vector of size DIMENSION + DIMENSION^2 for the evolution of extended state */
	/* This is a f, g and h functions of xdot, ydot, zdot in indices 0, 1 and 2 */
	/* And a hardcoding of the 3x3 evolution of variational equations (delta or Phi_t),
	   written as a 9-vector. Its evolution is given by the Jacobian times delta itself. */

	rossler(state, stateEvolution);

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
void rosslerEuler(double *state) {
	double x, y, z;
	float dt = h;
	double fgh[3];

	rossler(state, fgh);

	x = state[0] + dt * fgh[0];
	y = state[1] + dt * fgh[1];
	z = state[2] + dt * fgh[2];

	state[0] = x;
	state[1] = y;
	state[2] = z;
}
/* This is Runge-Kutta 4th order */
void rosslerRK4(double *state) {
	double array[3];
	double k1[3], k2[3], k3[3], k4[3];

	rossler(state, k1);

	array[0] = state[0] + h / 2 * k1[0];
	array[1] = state[1] + h / 2 * k1[1];
	array[2] = state[2] + h / 2 * k1[2];

	rossler(array, k2);

	array[0] = state[0] + h / 2 * k2[0];
	array[1] = state[1] + h / 2 * k2[1];
	array[2] = state[2] + h / 2 * k2[2];

	rossler(array, k3); /* Now fgh = k3 */

	array[0] = state[0] + h * k3[0];
	array[1] = state[1] + h * k3[1];
	array[2] = state[2] + h * k3[2];

	rossler(array, k4); /* Now fgh = k4 */

	state[0] = state[0] + h / 6 * (k1[0] + 2 * k2[0] + 2 * k3[0] + k4[0]);
	state[1] = state[1] + h / 6 * (k1[1] + 2 * k2[1] + 2 * k3[1] + k4[1]);
	state[2] = state[2] + h / 6 * (k1[2] + 2 * k2[2] + 2 * k3[2] + k4[2]);
}
/*	This is Runge-Kutta 4th order for the full state, when variation equations are needed (Lyapunov) */
void rosslerRK4Full(double *state)
{
	double array[12];
	double k1[12], k2[12], k3[12], k4[12];

	rosslerFull(state, k1);

	for (size_t i = 0; i < 12; i++) {
		array[i] = state[i] + h / 2 * k1[i];
	}

	rosslerFull(array, k2);

	for (size_t i = 0; i < 12; i++) {
		array[i] = state[i] + h / 2 * k2[i];
	}

	rosslerFull(array, k3);

	for (size_t i = 0; i < 12; i++) {
		array[i] = state[i] + h / 2 * k3[i];
	}

	rosslerFull(array, k4);

	for (size_t i = 0; i < 12; i++) {
		state[i] = state[i] + h / 6 * (k1[i] + 2 * k2[i] + 2 * k3[i] + k4[i]);
	}
}
