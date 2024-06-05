#include "Chaos.h"

/*
 * The dbCreateUseEntry function is called if the user request has not been made in the past.
 * It will create an entry in the database, as a historic.
 *
 * The function is called from ChaosMain. 
 */

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
 	/* Double switch statement for the columns of useHistory to consider */
	switch (userAction) {
		case 1:
			strcat(sqlCreateUse, "param1min, param1max");
			snprintf(buffer, sizeof (buffer), "%f, %f", userMapValues.parameterRange[0], userMapValues.parameterRange[1]);
			strcat(sqlValues, buffer);
			break;
		case 2: 
			strcat(sqlCreateUse, "param1min, param1max");
			snprintf(buffer, sizeof (buffer), "%f, %f", userMapValues.parameterRange[0], userMapValues.parameterRange[1]);
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

	/* Do the INSERT */
	char *zErrMsg = 0;
    int rc = sqlite3_exec(database, sqlCreateUse, NULL, 0, &zErrMsg);
   	if (rc != SQLITE_OK) {
	    fprintf(stderr, "SQL error: %s\n", zErrMsg);
    	sqlite3_free(zErrMsg);
        sqlite3_close(database);
    }
	
	/* Return the rowid */
	rowid = sqlite3_last_insert_rowid(database);

}
