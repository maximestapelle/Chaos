#include "Chaos.h"

/*
    The inputDefaults function will store the remaining default values for the user's choice of dynamics and action.
    The inputChooseDefaults function will ask if the user wants to choose the default values for his choice.
        It will fetch the choice and if Yes, copy the default values into the actual user values.

    They are called from ChaosMain.
 */

int inputDefaults() {

 	sqlite3_stmt *res;
 	int step;
	char sqlfetchVarsAndParams[260] =
        "SELECT b.type, c.LaTeXname, b.defaultValue, b.defaultMinValue, b.defaultMaxValue "
        "FROM map a INNER JOIN mapUsesParameters b ON a.rowid = b.map_id INNER JOIN parameter c "
        "ON c.rowid = b.parameter_id WHERE a.rowid = ? ORDER BY b.type desc, b.priority;";

    int rc = sqlite3_prepare_v2(database, sqlfetchVarsAndParams, -1, &res, 0);
   	if (rc != SQLITE_OK) {
	    fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(database));
        sqlite3_close(database);
    	return 1;
    }
    else sqlite3_bind_int(res, 1, userMap);
    step = sqlite3_step(res);
	unsigned int d = 0;
	int p = 0;
	while (step == SQLITE_ROW) {
		if (strncmp((char *) sqlite3_column_text(res, 0), "V", 1) == 0) {
			strcpy(variablesNames[d], (char *) sqlite3_column_text(res, 1));
			defaultMapValues.IC[d] = (double) sqlite3_column_double(res, 2);
			d++;
		}
		if (strncmp((char *) sqlite3_column_text(res, 0), "P", 1) == 0) {
			strcpy(parametersNames[p], (char *) sqlite3_column_text(res, 1));
			defaultMapValues.parameters[p] = (double) sqlite3_column_double(res, 2);
			/* Fetch parameter range only for the only top priority parameter */
			if (p == 0) {
				defaultMapValues.parameterRange[0] = sqlite3_column_double(res, 3);
				defaultMapValues.parameterRange[1] = sqlite3_column_double(res, 4);
			}
			p++;
		}
	    step = sqlite3_step(res);
	}
	sqlite3_finalize(res);

    /*  Store the number of parameters */
	numberParameters = p;

    /*  Check if there are no inconsistencies in the database */
	if (d != dimension) {
		printf( "Inconsistency!! The dimension of the map and the number of variables do not match in the database"
                ", please correct.\n");
		return 1;
	}


	return 0;
}

bool inputChooseDefaults() {

	char line[30];
	bool choseDefaults = false;

	/*	TODO !!! This could be in the future merged in a clever way to ConsolidateUserRequest,
		that already has such coding to display a summary of the request to stdout (via summaryStdout).
		I leave it like this for now as it is not too big of a deal, but it would be a bit nicer. */

	printf(ANSI_COLOR_GREEN "You have the possibility to choose default values for this application and action. "
							"Those defaults values are:\n\n");
	printf(ANSI_COLOR_CYAN "Initial conditions : ");
	for (size_t d = 0; d < dimension; d++) {
		printf("%s = %.3f", variablesNames[d], defaultMapValues.IC[d]);
		if (d < dimension - 1) printf(", ");
		else printf(".\n");
	}
	printf("Number of iterations : %ld.\n", NDefault);
	if (userAction != 3) {
		printf( "Range of the parameter %s: %.3f to %.3f; Number of divisions of the interval : %ld.\n",
				parametersNames[0], defaultMapValues.parameterRange[0], defaultMapValues.parameterRange[1], SDefault);
		if (numberParameters > 1) printf("Other parameters : ");
	}
	else {
		printf("Parameters : %s = %.3f", parametersNames[0], defaultMapValues.parameters[0]);
		if (numberParameters > 1) printf(", ");
	}
	for (size_t p = 1; p < numberParameters; p++) {
		printf("%s = %.3f", parametersNames[p], defaultMapValues.parameters[p]);
		if (p < numberParameters - 1) printf(", ");
		else printf(".\n");
	}
	printf(ANSI_COLOR_GREEN "\nPlease make your choice [Y/n]:\n\n" ANSI_COLOR_RESET);
	if (fgets(line, sizeof(line), stdin)) {
		if ((line[0] == '\n') || (line[0] == 'Y') || (line[0] == 'y')) {
        	choseDefaults = true;
        	printf(ANSI_COLOR_GREEN "The default values will be used.\n\n" ANSI_COLOR_RESET);
        	memcpy(userMapValues.IC, defaultMapValues.IC, DIMENSION_MAX * sizeof(float));
        	memcpy(userMapValues.parameters, defaultMapValues.parameters, NUMBER_PARAMETERS_MAX * sizeof(float));
        	memcpy(userMapValues.parameterRange, defaultMapValues.parameterRange, 2 * sizeof(float));
        	N = NDefault;
        	S = SDefault;
        }
	}

	return choseDefaults;
}
