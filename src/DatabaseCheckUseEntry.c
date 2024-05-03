#include "Chaos.h"

/*
 * The dbCheckUseEntry function will check if the request from the user has already been made in the past.
 *
 * The function is called from ChaosMain. 
 */

int dbCheckUseEntry() {

	int id = 0;
	int step;
 	sqlite3_stmt *res;

	char sqlCheckUse[500] = "SELECT rowid FROM useHistory WHERE map_id = "; /* Stores the beginning of the query */
	snprintf(sqlCheckUse, sizeof(sqlCheckUse), "%s%d AND action_id = %d AND ", sqlCheckUse, userMap, userAction);
	snprintf(sqlCheckUse, sizeof(sqlCheckUse), "%sN = %ld AND ", sqlCheckUse, N);
	if (userAction != 3) {
		snprintf(sqlCheckUse, sizeof(sqlCheckUse), "%sS = %ld AND ", sqlCheckUse, S);
	}

	for (size_t d = 0; d < dimension; d++) {
		strcat(sqlCheckUse, "var");
		snprintf(sqlCheckUse, sizeof(sqlCheckUse), "%s%zu0 = %f AND ", sqlCheckUse, d + 1, userMapValues.IC[d]);
	}
 	/* Double switch statement for the columns of useHistory to consider */
	switch (userAction) {
		case 1:
			snprintf(sqlCheckUse, sizeof(sqlCheckUse), "%sparam1min = %f AND param1max = %f ", sqlCheckUse, userMapValues.parameterRange[0], userMapValues.parameterRange[1]);
			break;
		case 2: 
			snprintf(sqlCheckUse, sizeof(sqlCheckUse), "%sparam1min = %f AND param1max = %f ", sqlCheckUse, userMapValues.parameterRange[0], userMapValues.parameterRange[1]);
			break;
		case 3:
			snprintf(sqlCheckUse, sizeof(sqlCheckUse), "%sparam10 = %f ", sqlCheckUse, userMapValues.parameters[0]);
			break;
		case 4:
			/**** !!!!! Second range of parameters not yet implemented in main, I'l put 1.0 in the meantime *****/
			snprintf(sqlCheckUse, sizeof(sqlCheckUse), "%sparam1min = %f AND param1max = %f AND param2min = %f AND param2max = %f ", sqlCheckUse, userMapValues.parameterRange[0], userMapValues.parameterRange[1], 1.0F, 1.0F);
			break;
		case 5:
			/**** !!!!! Second range of parameters not yet implemented in main, I'l put 1.0 in the meantime *****/
			snprintf(sqlCheckUse, sizeof(sqlCheckUse), "%sparam1min = %f AND param1max = %f AND param2min = %f AND param2max = %f ", sqlCheckUse, userMapValues.parameterRange[0], userMapValues.parameterRange[1], 1.0F, 1.0F);
			break;
	}
	switch (numberParameters) {
		case 1:
			strcat(sqlCheckUse, ";");
			break;
		case 2:
			if (userAction == 4 || userAction == 5) {
				strcat(sqlCheckUse, ";");
			}
			else {
				snprintf(sqlCheckUse, sizeof(sqlCheckUse), "%sAND param20 = %f ", sqlCheckUse, userMapValues.parameters[1]);
			}
			break;
		default:
			for (size_t p = 2; p <= numberParameters; p++) {
				snprintf(sqlCheckUse, sizeof(sqlCheckUse), "%sAND param%zu0 = %f ", sqlCheckUse, p, userMapValues.parameters[p - 1]);
			}
			strcat(sqlCheckUse, ";");
			break;
	}

	/* Do the Check */
    int rc = sqlite3_prepare_v2(database, sqlCheckUse, -1, &res, 0);
    if (rc != SQLITE_OK) {   
		fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(database));
    }
	step = sqlite3_step(res);
	if (step == SQLITE_ROW) {
	    id = sqlite3_column_int(res, 0);
	}

	return id;

}
