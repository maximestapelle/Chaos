#include "Chaos.h"

/*
	Two functions to handle the history of requests:
		- The dbCheckUseEntry function will check if the request from the user has already been made in the past.
		- if the user request has not been made in the past, the dbCreateUseEntry function is called.
		  It will create an entry in the database, as a historic.

	The functions are called from ChaosMain.
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
 	/*	Double switch statement for the columns of useHistory to consider */
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

	/*	Do the Check */
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

void dbCreateUseEntry() {

	char sqlCreateUse[500] = "INSERT INTO useHistory (map_id, action_id, N, ";  /* Stores the beginning of the query */
	char sqlValues[200] = ""; 													/* Stores the values */
	char buffer[200];

	snprintf(sqlValues, sizeof(sqlValues), "%d, %d, %ld, ", userMap, userAction, N);
	if (userAction != 3) {
		strcat(sqlCreateUse, "S, ");
		snprintf(buffer, sizeof (buffer), "%zu, ", S);
		strcat(sqlValues, buffer);
	}

	for (size_t d = 1; d <= dimension; d++) {
		strcat(sqlCreateUse, "var");
		snprintf(buffer, sizeof (buffer), "%zu0, ", d);
		strcat(sqlCreateUse, buffer);
		snprintf(buffer, sizeof (buffer), "%f, ", userMapValues.IC[d - 1]);
		strcat(sqlValues, buffer);
	}
 	/*	Double switch statement for the columns of useHistory to consider */
	switch (userAction) {
		case 1:
			strcat(sqlCreateUse, "param1min, param1max");
			snprintf(buffer, sizeof (buffer), "%f, %f",
						userMapValues.parameterRange[0], userMapValues.parameterRange[1]);
			strcat(sqlValues, buffer);
			break;
		case 2:
			strcat(sqlCreateUse, "param1min, param1max");
			snprintf(buffer, sizeof (buffer), "%f, %f",
						userMapValues.parameterRange[0], userMapValues.parameterRange[1]);
			strcat(sqlValues, buffer);
			break;
		case 3:
			strcat(sqlCreateUse, "param10");
			snprintf(buffer, sizeof (buffer), "%f", userMapValues.parameters[0]);
			strcat(sqlValues, buffer);
			break;
		case 4:
			strcat(sqlCreateUse, "param1min, param1max, param2min, param2max");
			/**** !!!!! Second range of parameters not yet implemented in main, I'l put 1.0 in the meantime *****/
			snprintf(buffer, sizeof (buffer),   "%f, %f, %f, %f", userMapValues.parameterRange[0],
												userMapValues.parameterRange[1], 1.0F, 1.0F);
			strcat(sqlValues, buffer);
			break;
		case 5:
			strcat(sqlCreateUse, "param1min, param1max, param2min, param2max");
			/**** !!!!! Second range of parameters not yet implemented in main, I'l put 1.0 in the meantime *****/
			snprintf(buffer, sizeof (buffer),   "%f, %f, %f, %f", userMapValues.parameterRange[0],
												userMapValues.parameterRange[1], 1.0F, 1.0F);
			strcat(sqlValues, buffer);
			break;
	}
	switch (numberParameters) {
		case 1:
			strcat(sqlCreateUse, ") VALUES (");
			strcat(sqlValues, ")");
			break;
		case 2:
			if (userAction == 4 || userAction == 5) {
				strcat(sqlCreateUse, ") VALUES (");
				strcat(sqlValues, ")");
			}
			else {
				strcat(sqlCreateUse, ", param20) VALUES (");
				snprintf(buffer, sizeof (buffer), ", %f)", userMapValues.parameters[1]);
				strcat(sqlValues, buffer);
			}
			break;
		default:
			for (size_t p = 2; p <= numberParameters; p++) {
				strcat(sqlCreateUse, ", param");
				snprintf(buffer, sizeof (buffer), "%zu0", p);
				strcat(sqlCreateUse, buffer);
				snprintf(buffer, sizeof (buffer), ", %f", userMapValues.parameters[p - 1]);
				strcat(sqlValues, buffer);
			}
			strcat(sqlCreateUse, ") VALUES (");
			strcat(sqlValues, ")");
			break;
	}

	strcat(sqlCreateUse, sqlValues);
	strcat(sqlCreateUse, ";");

	/*	Do the INSERT */
	char *zErrMsg = 0;
    int rc = sqlite3_exec(database, sqlCreateUse, NULL, 0, &zErrMsg);
   	if (rc != SQLITE_OK) {
	    fprintf(stderr, "SQL error: %s\n", zErrMsg);
    	sqlite3_free(zErrMsg);
        sqlite3_close(database);
    }

	/*	Return the rowid */
	rowid = sqlite3_last_insert_rowid(database);
}
