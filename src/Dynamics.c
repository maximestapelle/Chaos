#include "Chaos.h"
#include "Utilities.h"
#include "Dynamics.h"

/* Contains the dynamics of each dynamical system.
 *
 * All functions work on the same array trajectory, of size DIMENSION_MAX:
 *	- the 2nd and 3rd components are ignored for 1D dynamical systems;
 *	- the 3rd component is ignored for 2D dynamical systems.
 * It works the same with the parameters : array of size NUMBER_PARAMETERS_MAX.
 *
 * For flows, we also have here the derivatives (Jacobian) and implementation of the 
 * Euler and Runge-Kutta 4th order methods.
 *
 * Dynamics are used by the functions attractor(), bifurcation() and bifurcation2D() */

void logistic(double trajectory[]) {
	trajectory[0] =  userMapValues.parameters[0] * trajectory[0] * (1 - trajectory[0]);
}
double logisticDerivative(double trajectory[]) {
	return userMapValues.parameters[0] * (1 - 2 * trajectory[0]);
}
double logisticLyapunov(double trajectory[]) {
	return log(fabs(userMapValues.parameters[0])) + log(fabs(1 - 2 * trajectory[0]));
}

void logisticExp(double trajectory[]) {
	trajectory[0] = userMapValues.parameters[0] * trajectory[0] * exp(-trajectory[0]);
}
double logisticExpDerivative(double trajectory[]) {
	return userMapValues.parameters[0] * (1 - trajectory[0]) * exp(-trajectory[0]);
}
double logisticExpLyapunov(double trajectory[]) {
	return log(fabs(userMapValues.parameters[0])) + log(fabs(1 - trajectory[0])) - trajectory[0];
}

void gauss(double trajectory[]) {
	 trajectory[0] = exp(- userMapValues.parameters[1] * pow(trajectory[0], 2)) + userMapValues.parameters[0];
}
double gaussDerivative(double trajectory[]) {
	return -2 * userMapValues.parameters[1] * trajectory[0] * exp(- userMapValues.parameters[1] * pow(trajectory[0], 2));
}
double gaussLyapunov(double trajectory[]) {
	return - userMapValues.parameters[1] * pow(trajectory[0], 2) + log(fabs(2 * userMapValues.parameters[1] * trajectory[0]));
}

void tent(double trajectory[]) {
	if (trajectory[0] < 0.5) {
		trajectory[0] =  userMapValues.parameters[0] * trajectory[0];
	}
	else {
		trajectory[0] =  userMapValues.parameters[0] * (1 - trajectory[0]);
	}
}

void circle(double trajectory[]) {
	double result;
	
	result = trajectory[0] + userMapValues.parameters[0] / (2 * M_PI) * ((double) sin(2 * M_PI * trajectory[0])) + userMapValues.parameters[1];
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



void tinkerbell(double trajectory[]) {
	double x, y;
	x = pow(trajectory[0], 2) - pow(trajectory[1], 2) + userMapValues.parameters[1] * trajectory[0] + userMapValues.parameters[0] * trajectory[1];
	y = 2 * trajectory[0] * trajectory[1] + userMapValues.parameters[2] * trajectory[0] + userMapValues.parameters[3] * trajectory[1];
	
	trajectory[0] = x;
	trajectory[1] = y;
}
void tinkerbellJacobian(double trajectory[], double jacobian[][2]) {
	jacobian[0][0] = 2 * trajectory[0] + userMapValues.parameters[1];
	jacobian[0][1] = -2 * trajectory[1] + userMapValues.parameters[0];
	jacobian[1][0] = 2 * trajectory[1] + userMapValues.parameters[2];
	jacobian[1][1] = 2 * trajectory[0] + userMapValues.parameters[3];

}

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

void lorenzEvolution(double input[], double fgh[]) {
	fgh[0] = userMapValues.parameters[1] * (input[1] - input[0]);
	fgh[1] = input[0] * (userMapValues.parameters[0] - input[2]) - input[1];
	fgh[2] = input[0] * input[1] - userMapValues.parameters[2] * input[2];
	
}
/* This is Newton/Euler method for Lorenz (so the easiest one, based on 1st order Taylor series) */
void lorenz(double trajectory[], const float dt) {
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
/* This is Runge-Kutta 4th order */
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

void rosslerEvolution(double input[], double fgh[]) {
	fgh[0] = -input[1] - input[2];
	fgh[1] = input[0] + userMapValues.parameters[2] * input[1];
	fgh[2] = userMapValues.parameters[0] + input[2] * (input[0] - userMapValues.parameters[1]);
	
}
/* This is Newton/Euler method for Rossler (so the easiest one, based on 1st order Taylor series) */
void rossler(double trajectory[], const float dt) {
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
