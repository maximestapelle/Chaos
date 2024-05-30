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
					   const double  *);					/* parameters */

/* Structure of a dynamical system */
typedef struct {
	t_flow dynamics;
	t_flow jacobian;
	double *parameters;
} flow;





/* Dynamics functions - those do not use the structure above yet */
void logistic(float trajectory[]);
float logisticDerivative(float trajectory[]);
float logisticLyapunov(float trajectory[]);
void logisticExp(float trajectory[]);
float logisticExpDerivative(float trajectory[]);
float logisticExpLyapunov(float trajectory[]);
void gauss(float trajectory[]);
float gaussDerivative(float trajectory[]);
float gaussLyapunov(float trajectory[]);
void tent(float trajectory[]);
void circle(float trajectory[]);
float circleDerivative(float trajectory[]);
float circleLyapunov(float trajectory[]);
void tinkerbell(float trajectory[]);
void tinkerbellJacobian(float trajectory[], float jacobian[][dimension]);
void henon(float trajectory[]);
void henonJacobian(float trajectory[], float jacobian[][dimension]);
void lorenzEvolution(float input[], float fgh[]);
void lorenz(float trajectory[], const float dt);
void lorenzRK4(float trajectory[], const float h);
void rosslerEvolution(float input[], float fgh[]);
void rossler(float trajectory[], const float dt);
void rosslerRK4(float trajectory[], const float h);




#endif /* DYNAMICS_H */