#include "Chaos.h"

/*
    The inputMaps function will propose the different dynamical systems and fetch the user's choice.
        It will store some values for the chosen dynamical system from the database.

    The inputActions function will propose the possible actions for the user, and fetch the user's choice.
        It will also display some default values stored in the database, once the user's choice is known.

        Changes : 5 May 2024 "active" flag added to the database, on action and action/map levels.

    They are the first functions to be called for User Input
 */


int inputMaps() {

 	sqlite3_stmt *res;
 	int step;
 	char line[30];
	char sqlfetchMaps[100] = "SELECT rowid, shortName, longName, formula, dimension, type FROM map ORDER BY rowid;";
	char sqlverifyMap[80]  = "SELECT dimension, space, type, LaTeXformula, shortName FROM map WHERE rowid = ?;";

	//Print title
	printf( ANSI_COLOR_GREEN "\nPlease enter the number corresponding to the application of your choice:"
            ANSI_COLOR_RESET "\n\n");

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
				printf( "I'm afraid what you entered doesn't correspond to one of our maps, "
                        "please choose something else...\n");
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

int inputActions(
        unsigned int *NMin,
        long unsigned int *NMax,
        unsigned int *SMin,
        long unsigned int *SMax) {

 	sqlite3_stmt *res;
 	int step;
 	char line[30];
	char sqlfetchActions[250] =
        "SELECT A.rowid, A.name, A.description, CASE WHEN A.rowid IN (SELECT b.action_id FROM "
        "map a INNER JOIN actionConcernsMap b ON a.rowid = b.map_id WHERE a.rowid = ? AND "
        "active) THEN 'Y' ELSE 'N' END FROM action A WHERE active ORDER BY A.rowid;";
	char sqlverifyAction[250] =
        "SELECT b.N, b.Nmin, b.Nmax, b.S, b.Smin, b.SMax, c.name FROM map a INNER JOIN "
        "actionConcernsMap b ON a.rowid = b.map_id INNER JOIN action c ON "
        "b.action_id = c.rowid WHERE a.rowid = ? AND c.rowid = ? AND b.active;";

	printf(ANSI_COLOR_GREEN "\nYou can now choose the action you want the program to perform:" ANSI_COLOR_RESET "\n\n");

	//Fetch possible actions for the map
    int rc = sqlite3_prepare_v2(database, sqlfetchActions, -1, &res, 0);
    if (rc == SQLITE_OK) {
		sqlite3_bind_int(res, 1, userMap);
    }
    else {
		fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(database));
		sqlite3_close(database);
		return 1;
    }
	step = sqlite3_step(res);
	while (step == SQLITE_ROW) {
		if (strncmp((char *) sqlite3_column_text(res, 3), "N", 1) == 0) {
			printf(ANSI_COLOR_RED "**Not available for this map** ");
		}
		printf(ANSI_COLOR_CYAN "%d. ", sqlite3_column_int(res, 0));
		printf("%s", sqlite3_column_text(res, 1));
		printf(ANSI_COLOR_GRAY" - %s\n", sqlite3_column_text(res, 2));
		step = sqlite3_step(res);
	}
    sqlite3_finalize(res);
	printf(ANSI_COLOR_RESET "\n");

CHOOSE_ACTION:
	if (fgets(line, sizeof(line), stdin)) {
		if (sscanf(line, "%d", &userAction) != 1) {
			printf(ANSI_COLOR_GREEN "Please enter a valid number.\n" ANSI_COLOR_RESET);
			goto CHOOSE_ACTION;
		}
		else {
            /*  Verify the action and fetch the default values for the action */
			rc = sqlite3_prepare_v2(database, sqlverifyAction, -1, &res, 0);
			if (rc == SQLITE_OK) {
				sqlite3_bind_int(res, 1, userMap);
				sqlite3_bind_int(res, 2, userAction);
			}
			else {
				fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(database));
				sqlite3_close(database);
				return 1;
			}
			step = sqlite3_step(res);
			if (step != SQLITE_ROW) {
				printf( ANSI_COLOR_GREEN "I'm afraid that is not a valid action for this map, please try again.\n"
                        ANSI_COLOR_RESET);
				sqlite3_finalize(res);
				goto CHOOSE_ACTION;
			}
            /*  Put the needed parameters into memory */
			NDefault = sqlite3_column_int(res, 0);
			*NMin = sqlite3_column_int(res, 1);
			*NMax = sqlite3_column_int(res, 2);
			if (userAction != 3) {
				SDefault = sqlite3_column_int(res, 3);
				*SMin = sqlite3_column_int(res, 4);
				*SMax = sqlite3_column_int(res, 5);
			}
			strcpy(actionName, (char *) sqlite3_column_text(res, 6));
			sqlite3_finalize(res);
		}
	}
	printf("\n");

	return 0;
}
