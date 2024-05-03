#include "Chaos.h"

/*
 * The dbCreateDataFile function is called if the user request has already been made in the past.
 * It will recreate the data file from the database.
 *
 * The function is called from ChaosMain. 
 */

void dbCreateDataFile() {

	FILE * fp = fopen(dataFile, "wb");
	char sqlfetchData[100] = "SELECT data FROM dataHistory WHERE use_id = "; /* Stores the beginning of the query */
	
	snprintf(sqlfetchData, sizeof(sqlfetchData), "%s%d;", sqlfetchData, rowid);
	
	sqlite3_stmt *res;
	int rc = sqlite3_prepare_v2(database, sqlfetchData, -1, &res , 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot prepare statement: %s\n", sqlite3_errmsg(database));
    }    
    int bytes = 0;
    rc = sqlite3_step(res);
    if (rc == SQLITE_ROW) {
        bytes = sqlite3_column_bytes(res, 0);
    }

	fwrite(sqlite3_column_blob(res, 0), bytes, 1, fp);

    sqlite3_finalize(res);
    fclose(fp);

}
