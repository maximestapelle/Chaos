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
	char buffer[200];

	snprintf(buffer, sizeof (buffer), "%d AND action_id = %d AND N = %zu AND ", userMap, userAction, N);
	strcat(sqlCheckUse, buffer);
	if (userAction != 3) {
		snprintf(buffer, sizeof (buffer), "S = %zu AND ", S);
		strcat(sqlCheckUse, buffer);
	}

	for (size_t d = 0; d < dimension; d++) {
		strcat(sqlCheckUse, "var");
		snprintf(buffer, sizeof (buffer), "%zu0 = %f AND ", d + 1, userMapValues.IC[d]);
		strcat(sqlCheckUse, buffer);
	}
 	/* Double switch statement for the columns of useHistory to consider */
	switch (userAction) {
		case 1:
			snprintf(buffer, sizeof (buffer),   "param1min = %f AND param1max = %f ",
												userMapValues.parameterRange[0], userMapValues.parameterRange[1]);
			strcat(sqlCheckUse, buffer);
			break;
		case 2: 
			snprintf(buffer, sizeof (buffer),   "param1min = %f AND param1max = %f ",
												userMapValues.parameterRange[0], userMapValues.parameterRange[1]);
			strcat(sqlCheckUse, buffer);
			break;
		case 3:
			snprintf(buffer, sizeof (buffer), "param10 = %f ", userMapValues.parameters[0]);
			strcat(sqlCheckUse, buffer);
			break;
		case 4:
			/**** !!!!! Second range of parameters not yet implemented in main, I'l put 1.0 in the meantime *****/
			snprintf(buffer, sizeof (buffer), "param1min = %f AND param1max = %f AND param2min = %f AND param2max = %f "
											, userMapValues.parameterRange[0], userMapValues.parameterRange[1], 1.0F, 1.0F);
			strcat(sqlCheckUse, buffer);
			break;
		case 5:
			/**** !!!!! Second range of parameters not yet implemented in main, I'l put 1.0 in the meantime *****/
			snprintf(buffer, sizeof (buffer), "param1min = %f AND param1max = %f AND param2min = %f AND param2max = %f "
											, userMapValues.parameterRange[0], userMapValues.parameterRange[1], 1.0F, 1.0F);
			strcat(sqlCheckUse, buffer);
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
				snprintf(buffer, sizeof (buffer), "AND param20 = %f ", userMapValues.parameters[1]);
				strcat(sqlCheckUse, buffer);
			}
			break;
		default:
			for (size_t p = 2; p <= numberParameters; p++) {
				snprintf(buffer, sizeof (buffer), "AND param%zu0 = %f ", p, userMapValues.parameters[p - 1]);
				strcat(sqlCheckUse, buffer);
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
