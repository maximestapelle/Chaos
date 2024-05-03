#include "Chaos.h"

/*
 * The inputMaps function will propose the different dynamical systems and fetch the user's choice.
 * It will store some values for the chosen dynamical system from the database.
 *
 * The function is called from ChaosMain. 
 */

int inputMaps() {

 	sqlite3_stmt *res;
 	int step;
 	char line[30];
	char sqlfetchMaps[100] = "SELECT rowid, shortName, longName, formula, dimension, type FROM map ORDER BY rowid;";
	char sqlverifyMap[80]  = "SELECT dimension, space, type, LaTeXformula, shortName FROM map WHERE rowid = ?;";

	//Print title
	printf(ANSI_COLOR_GREEN "\nPlease enter the number corresponding to the application of your choice:" ANSI_COLOR_RESET "\n\n");
	
    //Fetch map names
    int rc = sqlite3_prepare_v2(database, sqlfetchMaps, -1, &res, 0);
   	if (rc != SQLITE_OK) {
	    fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(database));
        sqlite3_close(database);
    	return 1;
    }
    step = sqlite3_step(res);
	while (step == SQLITE_ROW) {
		printf(ANSI_COLOR_CYAN "%d. %s ", sqlite3_column_int(res, 0), sqlite3_column_text(res, 1));
		printf("(%dD, %s) ", sqlite3_column_int(res, 4), sqlite3_column_text(res, 5));
		printf(ANSI_COLOR_GRAY "- %s\t", sqlite3_column_text(res, 2));
		printf(ANSI_COLOR_BLUE "%s\n", sqlite3_column_text(res, 3));
		step = sqlite3_step(res);
	}
	printf(ANSI_COLOR_RESET "\n");
	sqlite3_finalize(res);

	//Read Choice of map
CHOOSE_MAP:
	if (fgets(line, sizeof(line), stdin)) {
		if (sscanf(line, "%d", &userMap) != 1) {
			printf("Please enter a valid number.\n");
			goto CHOOSE_MAP;
		}
		else {
			// Verify the user's choice
			rc = sqlite3_prepare_v2(database, sqlverifyMap, -1, &res, 0);
			if (rc == SQLITE_OK) {   
				sqlite3_bind_int(res, 1, userMap);
			}
			else {        
				fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(database));
				return 1;
			}
			step = sqlite3_step(res);
			if (step != SQLITE_ROW) {
				printf("I'm afraid what you entered doesn't correspond to one of our maps, please choose something else...\n");
				sqlite3_finalize(res);
				goto CHOOSE_MAP;
			} 
			/* Put the needed parameters into memory */
			dimension = sqlite3_column_int(res, 0);
			strcpy(space, (char *) sqlite3_column_text(res, 1));
			strcpy(type, (char *) sqlite3_column_text(res, 2));
			strcpy(LaTeXformula, (char *) sqlite3_column_text(res, 3));
			strcpy(mapName, (char *) sqlite3_column_text(res, 4));
			sqlite3_finalize(res);
			/* Dirty hardcoding of Complex to Real */
			if (strcmp(space, "C") == 0) dimension = 2;
		}		
	}

	return 0;
}
