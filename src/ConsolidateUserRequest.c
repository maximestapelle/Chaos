#include "Chaos.h"

/*
 * The consolidateUserRequest function will aggregate the data entered by the user, in order to :
 * - display it on the console, as a summary for the user;
 * - create a string for data and image filenames;
 * - create a legend for the plots.
 *
 * The function is called from ChaosMain. 
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
		sprintf(summaryStdout, "%sVarying parameter %s: from %.3f to %.3f. ", summaryStdout, parametersNames[0], userMapValues.parameterRange[0], userMapValues.parameterRange[1]);
		sprintf(summaryStdout, "%sNumber of divisions of the range: %ld.\n", summaryStdout, S);
		sprintf(summaryFileName, "%sParameter range(%.3f,%.3f) ", summaryFileName, userMapValues.parameterRange[0], userMapValues.parameterRange[1]);
		sprintf(summaryFileName, "%sDivisions %ld ", summaryFileName, S);
		if (numberParameters > 1) {
			sprintf(summaryStdout, "%sFixed parameter(s): ", summaryStdout);
			sprintf(summaryPlotLegend, "%sFixed parameter(s): ", summaryPlotLegend);
			sprintf(summaryFileName, "%sParameter(s)(", summaryFileName);
		}
	}
	else {
		sprintf(summaryStdout, "%sParameter(s): %s = %.3f", summaryStdout, parametersNames[0], userMapValues.parameters[0]);
		sprintf(summaryPlotLegend, "%sParameter(s): $%s$ = %.3f", summaryPlotLegend, parametersNames[0], userMapValues.parameters[0]);
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

}
