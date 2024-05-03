#include "Chaos.h"

/*
 * The checkDataFile and checkImageFile functions will check if the data and Image files
 * already exist in the /data and /img directories, before recreating them from the database.
 *
 * The function is called from ChaosMain. 
 */

bool checkDataFile() {
	FILE * fp;
	
	if((fp = fopen(dataFile,"r")) != NULL) {
		fclose(fp);
		return true;
	}
	else return false;

}

bool checkImageFile() {
	FILE * fp;
	
	if((fp = fopen(imageFile,"r")) != NULL) {
		fclose(fp);
		return true;
	}
	else return false;

}
