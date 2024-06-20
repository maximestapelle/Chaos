#ifndef DYNAMICS_H
#define DYNAMICS_H

/* This header defines macros, structures, types and functions
   specific to the handling of Dynamical Systems.
*/

#include "Chaos.h"



/* Dynamics functions - those do not use the structure above yet */
void logistic(double trajectory[]);
double logisticDerivative(double trajectory[]);
double logisticLyapunov(double trajectory[]);
void logisticExp(double trajectory[]);
double logisticExpDerivative(double trajectory[]);
double logisticExpLyapunov(double trajectory[]);
void gauss(double trajectory[]);
double gaussDerivative(double trajectory[]);
double gaussLyapunov(double trajectory[]);
void tent(double trajectory[]);
void circle(double trajectory[]);
double circleDerivative(double trajectory[]);
double circleLyapunov(double trajectory[]);
void tinkerbell(double trajectory[]);
void tinkerbellJacobian(double trajectory[], double jacobian[][2]);
void henon(double trajectory[]);
void henonJacobian(double trajectory[], double jacobian[][2]);
void lorenzEvolution(double input[], double fgh[]);
void lorenzEvolutionFull(const double *state,
						  	   double stateEvolution[]);
void lorenzEuler(double trajectory[], const float dt);
void lorenzRK4(double trajectory[], const float h);
void lorenzRK4Full(double *state, const float h);
void rosslerEvolution(double input[], double fgh[]);
void rosslerEvolutionFull(const double *state,
						  		double stateEvolution[]);
void rosslerEuler(double trajectory[], const float dt);
void rosslerRK4(double trajectory[], const float h);
void rosslerRK4Full(double *state, const float h);




#endif /* DYNAMICS_H */
