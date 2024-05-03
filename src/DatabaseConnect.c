#include "Chaos.h"

//Database connection
int dbConnect() {

	int rc = sqlite3_open(databaseName, &database);
	if (rc)	{
    	fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(database));
    	sqlite3_close(database);
    	return 1;
    }

	return 0;
}
