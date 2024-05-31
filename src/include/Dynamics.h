#ifndef DYNAMICS_H
#define DYNAMICS_H

/* This header defines macros, structures, types and functions
   specific to the handling of Dynamical Systems.
*/

#include "Chaos.h"

/* Function model for dynamical systems dx/dt = F(x)
   Plays the role of F or Jacobian of F   */
typedef void (*t_flow)(const double  *,					/* actual state x or extended to x+variational state */
					   double *,						/* return F or Jacobian of F */
					   const double  *);				/* parameters */

/* Structure of a dynamical system */
typedef struct {
	t_flow dynamics;
	t_flow jacobian;
	double *parameters;
} flow;





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
void tinkerbellJacobian(double trajectory[], double jacobian[][dimension]);
void henon(double trajectory[]);
void henonJacobian(double trajectory[], double jacobian[][dimension]);
void lorenzEvolution(double input[], double fgh[]);
void lorenzEvolutionFull(const double *state,
						  	   double stateEvolution[]);
void lorenzEuler(double trajectory[], const float dt);
void lorenzRK4(double trajectory[], const float h);
void lorenzRK4Full(const float   h,
		       	   double *state,
		       	   size_t stateDimension);
void rosslerEvolution(double input[], double fgh[]);
void rosslerEvolutionFull(const double *state,
						  		double stateEvolution[]);
void rosslerEuler(double trajectory[], const float dt);
void rosslerRK4(double trajectory[], const float h);
void rosslerRK4Full(const float h,
		       		double *state,
		       		size_t stateDimension);




#endif /* DYNAMICS_H */