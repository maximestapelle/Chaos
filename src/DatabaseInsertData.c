#include "Chaos.h"

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
