#include "Chaos.h"

/*
	Functions to handle files in the database:
    	- dbInsertData  inserts the data  file into the database,
		- dbInsertImage inserts the image file into the database,
		- dbCreateDataFile  recreates the data  file from the database (request already done in the past),
		- dbCreateImageFile recreates the image file from the database (request already done in the past),

	The functions are called from ChaosMain.
 */

void dbInsertData() {

	FILE * fp = fopen(dataFile, "rb");
	char sqlInsertData[100] = "INSERT INTO dataHistory VALUES ("; /* Stores the beginning of the query */

	snprintf(sqlInsertData, sizeof(sqlInsertData), "%s%d, ?);", sqlInsertData, rowid);

	/* read the file into binary; first get the size */
	fseek(fp, 0, SEEK_END);
	long int flen = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	/* Initially this was char data[flen + 1] but gave segmentation fault when size was too big */
	char * data = malloc((flen + 1) * sizeof(char));
	long int size = fread(data, 1, flen, fp);
    fclose(fp);

	/* Do the INSERT */
	sqlite3_stmt *res;
	int rc = sqlite3_prepare_v2(database, sqlInsertData, -1, &res , 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot prepare statement: %s\n", sqlite3_errmsg(database));
    }

    sqlite3_bind_blob(res, 1, data, size, SQLITE_STATIC);
    rc = sqlite3_step(res);
    if (rc != SQLITE_DONE) {
        printf("execution failed: %s", sqlite3_errmsg(database));
    }
    sqlite3_finalize(res);

    free(data);
}
void dbInsertImage() {

	FILE * fp = fopen(imageFile, "rb");
	char sqlInsertData[100] = "INSERT INTO imageHistory VALUES ("; /* Stores the beginning of the query */

	snprintf(sqlInsertData, sizeof(sqlInsertData), "%s%d, ?);", sqlInsertData, rowid);

	/* read the file into binary; first get the size */
	fseek(fp, 0, SEEK_END);
	long int flen = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	/* Initially this was char data[flen + 1] but gave segmentation fault when size was too big */
	char * data = malloc((flen+1) * sizeof(char));
    long int size = fread(data, 1, flen, fp);
    fclose(fp);

	/* Do the INSERT */
	sqlite3_stmt *res;
	int rc = sqlite3_prepare_v2(database, sqlInsertData, -1, &res , 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot prepare statement: %s\n", sqlite3_errmsg(database));
    }

    sqlite3_bind_blob(res, 1, data, size, SQLITE_STATIC);
    rc = sqlite3_step(res);
    if (rc != SQLITE_DONE) {
        printf("execution failed: %s", sqlite3_errmsg(database));
    }
    sqlite3_finalize(res);

    free(data);
}

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
