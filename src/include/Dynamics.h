#ifndef DYNAMICS_H
#define DYNAMICS_H

/*	This header defines functions
   	specific to the handling of Dynamical Systems.
 */

#include "Chaos.h"

typedef void (*evolve_t)(double *);
extern evolve_t dynamics;				/*	Function pointer for the dynamics to be called by actions.
                                            It is set in createDynamics(), located in ConsolidateUserRequest.c.
                                         */

/*	Maps  */
void logistic(double *state);
void logisticFull(double *state);

void logisticExp(double *state);
void logisticExpFull(double *state);

void gauss(double *state);
void gaussFull(double *state);

void tent(double *state);

void circle(double *state);
void circleFull(double *state);

void tinkerbell(double *state);
void tinkerbellFull(double *state);

void henon(double *state);
void henonFull(double *state);


/*	Flows  */
void lorenzEuler(double *state);
void lorenzRK4(double *state);
void lorenzRK4Full(double *state);

void rosslerEuler(double *statet);
void rosslerRK4(double *state);
void rosslerRK4Full(double *state);


#endif /* DYNAMICS_H */
