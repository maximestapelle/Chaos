#include "Chaos.h"

/*
 * The createFileName function creates the full filenames for data and image.
 * It is called from ChaosMain. 
 */

void createFileName() {
	
	/* Based on summaryFileName, we create the data and image full names : */
	strcat(dataFile, actionName);
	strcat(dataFile, " ");
	strcat(dataFile, summaryFileName);
	strcat(dataFile, ".txt");

	strcat(imageFile, actionName);
	strcat(imageFile, " ");
	strcat(imageFile, summaryFileName);
	strcat(imageFile, ".png");

}
