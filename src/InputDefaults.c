#include "Chaos.h"

/*
 * The inputDefaults function will store the remaining default values for the user's choice of dynamics and action.
 * It is called from ChaosMain. 
 */

int inputDefaults() {

 	sqlite3_stmt *res;
 	int step;
	char sqlfetchVarsAndParams[260] = "SELECT b.type, c.LaTeXname, b.defaultValue, b.defaultMinValue, b.defaultMaxValue FROM map a INNER JOIN mapUsesParameters b ON a.rowid = b.map_id INNER JOIN parameter c ON c.rowid = b.parameter_id WHERE a.rowid = ? ORDER BY b.type desc, b.priority;";

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
	
	/* Store the number of parameters */
	numberParameters = p;

	/* Check if there are no inconsistencies in the database */
	if (d != dimension) {
		printf("Inconsistency!! The dimension of the map and the number of variables do not match in the database, please correct.\n");
		return 1;
	}
	

	return 0;
}
