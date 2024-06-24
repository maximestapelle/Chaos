#include "Dynamics.h"

/*
	The consolidateUserRequest function will aggregate the data entered by the user, in order to :
	- display it on the console, as a summary for the user;
	- create a string for data and image filenames;
	- create a legend for the plots.

	The function is called from ChaosMain.

	The file also contains createDynamics(), that sets the ``evolve_t dynamics'' variable defined in Dynamics.h.
		See the function contents for more comments.
 */

void consolidateUserRequest() {

	/* Map name */
	sprintf(summaryStdout, "%sMap name : %s.\n", summaryStdout, mapName);
	sprintf(summaryFileName, "%s%s ", summaryFileName, mapName);
	/* Action requested */
	sprintf(summaryStdout, "%sAction requested : %s.\n", summaryStdout, actionName);

	/* Initial conditions */
	sprintf(summaryStdout, "%sInitial Condition(s): ", summaryStdout);
	sprintf(summaryPlotLegend, "%sInitial Condition(s): ", summaryPlotLegend);
	sprintf(summaryFileName, "%sIC(", summaryFileName);

	for (size_t d = 0; d < dimension; d++) {
		sprintf(summaryStdout, "%s%s = %.3f", summaryStdout, variablesNames[d], userMapValues.IC[d]);
		sprintf(summaryPlotLegend, "%s$%s$ = %.3f", summaryPlotLegend, variablesNames[d], userMapValues.IC[d]);
		sprintf(summaryFileName, "%s%.3f", summaryFileName, userMapValues.IC[d]);
		if (d < dimension - 1) {
			sprintf(summaryStdout, "%s, ", summaryStdout);
			sprintf(summaryPlotLegend, "%s, ", summaryPlotLegend);
			sprintf(summaryFileName, "%s,", summaryFileName);
		}
		else {
			sprintf(summaryStdout, "%s.\n", summaryStdout);
			sprintf(summaryPlotLegend, "%s.", summaryPlotLegend);
			sprintf(summaryFileName, "%s) ", summaryFileName);
		}
	}

	/* Special characters for the Legend in Python */
	sprintf(summaryPlotLegend, "%s\'+\'\\n\'+r\'", summaryPlotLegend);

	/* Parameters */
	/* First one has special treatment */
	if (userAction != 3) {
		sprintf(summaryStdout, "%sVarying parameter %s: from %.3f to %.3f. ", summaryStdout,
				parametersNames[0], userMapValues.parameterRange[0], userMapValues.parameterRange[1]);
		sprintf(summaryStdout, "%sNumber of divisions of the range: %ld.\n", summaryStdout, S);
		sprintf(summaryFileName, "%sParameter range(%.3f,%.3f) ", summaryFileName,
				userMapValues.parameterRange[0], userMapValues.parameterRange[1]);
		sprintf(summaryFileName, "%sDivisions %ld ", summaryFileName, S);
		if (numberParameters > 1) {
			sprintf(summaryStdout, "%sFixed parameter(s): ", summaryStdout);
			sprintf(summaryPlotLegend, "%sFixed parameter(s): ", summaryPlotLegend);
			sprintf(summaryFileName, "%sParameter(s)(", summaryFileName);
		}
	}
	else {
		sprintf(summaryStdout, "%sParameter(s): %s = %.3f", summaryStdout,
				parametersNames[0], userMapValues.parameters[0]);
		sprintf(summaryPlotLegend, "%sParameter(s): $%s$ = %.3f", summaryPlotLegend,
				parametersNames[0], userMapValues.parameters[0]);
		sprintf(summaryFileName, "%sParameter(s)(%.3f", summaryFileName, userMapValues.parameters[0]);
		if (numberParameters > 1) {
			sprintf(summaryStdout, "%s, ", summaryStdout);
			sprintf(summaryPlotLegend, "%s, ", summaryPlotLegend);
			sprintf(summaryFileName, "%s,", summaryFileName);
		}
		else {
			sprintf(summaryStdout, "%s.\n", summaryStdout);
			sprintf(summaryPlotLegend, "%s.\n", summaryPlotLegend);
			sprintf(summaryFileName, "%s) ", summaryFileName);
		}
	}
	/* All the others */
	for (size_t p = 1; p < numberParameters; p++) {
		sprintf(summaryStdout, "%s%s = %.3f", summaryStdout, parametersNames[p], userMapValues.parameters[p]);
		sprintf(summaryPlotLegend, "%s$%s$ = %.3f", summaryPlotLegend, parametersNames[p], userMapValues.parameters[p]);
		sprintf(summaryFileName, "%s%.3f", summaryFileName, userMapValues.parameters[p]);
		if (p < numberParameters - 1) {
			sprintf(summaryStdout, "%s, ", summaryStdout);
			sprintf(summaryPlotLegend, "%s, ", summaryPlotLegend);
			sprintf(summaryFileName, "%s,", summaryFileName);
		}
		else {
			sprintf(summaryStdout, "%s.\n", summaryStdout);
			sprintf(summaryPlotLegend, "%s.", summaryPlotLegend);
			sprintf(summaryFileName, "%s) ", summaryFileName);
		}
	}

	/* Special characters for the Legend in Python */
	if (numberParameters > 1) sprintf(summaryPlotLegend, "%s\'+\'\\n\'+r\'", summaryPlotLegend);

	/* Number of iterations */
	sprintf(summaryStdout, "%sNumber of iterations: %ld.", summaryStdout, N);
	sprintf(summaryPlotLegend, "%sNumber of iterations: %ld.", summaryPlotLegend, N);
	sprintf(summaryFileName, "%sIterations %ld", summaryFileName, N);

	/*	Attractor : in case an estimation of the capacity dimension was requested  */
	if (requestCapacityDimension)
		sprintf(summaryFileName, "%s + CapDim", summaryFileName);

}

void createDynamics() {

	/*  This will contain pointers to all functions that evolve the state
		with the dynamics:
			- The first "column" if the evolution of the state;
			- The second one is the full evolution or "extended state", ie state+Jacobian.
		For now, Runge-Kutta 4th order is used for all flows.
	 */
	evolve_t allDynamics[NUMBER_OF_SYSTEMS][2];

	allDynamics[0][0] = logistic;
	allDynamics[0][1] = logisticFull;

	allDynamics[1][0] = logisticExp;
	allDynamics[1][1] = logisticExpFull;

	allDynamics[2][0] = gauss;
	allDynamics[2][1] = gaussFull;

	allDynamics[3][0] = tent;
	allDynamics[3][1] = tent;

	allDynamics[4][0] = circle;
	allDynamics[4][1] = circleFull;

	allDynamics[5][0] = tinkerbell;
	allDynamics[5][1] = tinkerbellFull;

	allDynamics[6][0] = henon;
	allDynamics[6][1] = henonFull;

	allDynamics[7][0] = lorenzRK4;
	allDynamics[7][1] = lorenzRK4Full;

	allDynamics[8][0] = rosslerRK4;
	allDynamics[8][1] = rosslerRK4Full;

	/*	Lyapunov uses the extended state, other actions use the normal one.	*/
	if (userAction == 2)
		dynamics = allDynamics[userMap - 1][1];
	else
		dynamics = allDynamics[userMap - 1][0];

}
