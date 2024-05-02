#include "Chaos.h"

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
