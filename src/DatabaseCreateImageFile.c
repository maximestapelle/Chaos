#include "Chaos.h"

void dbCreateImageFile() {

	FILE * fp = fopen(imageFile, "wb");
	char sqlfetchImage[100] = "SELECT image FROM imageHistory WHERE use_id = "; /* Stores the beginning of the query */
	
	snprintf(sqlfetchImage, sizeof(sqlfetchImage), "%s%d;", sqlfetchImage, rowid);
	
	sqlite3_stmt *res;
	int rc = sqlite3_prepare_v2(database, sqlfetchImage, -1, &res , 0);
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
